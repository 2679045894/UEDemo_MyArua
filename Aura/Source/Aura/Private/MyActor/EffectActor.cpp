// Fill out your copyright notice in the Description page of Project Settings.


#include "MyActor/EffectActor.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "GameFramework/Pawn.h"           // 包含APawn的头文件
#include "AbilitySystem/AruaAttributeSet.h"
#include "Character/AruaCharacter.h"

AEffectActor::AEffectActor()
{
	PrimaryActorTick.bCanEverTick = false;
	SceneComponent=CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(SceneComponent);
}


void AEffectActor::BeginPlay()
{
	Super::BeginPlay();
}

void AEffectActor::ApplyEffectToActor(AActor* TargetActor, TSubclassOf<UGameplayEffect> GameplayEffectClass)
{
	//直接通过AACtor获取ASC
	UAbilitySystemComponent* TargetASC=UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
	if (TargetASC==nullptr)return;

	check(GameplayEffectClass);
	//创建效果上下文  记录了效果的完整传递路径，确保效果能够正确计算、显示和追溯。
	FGameplayEffectContextHandle EffectContextHandle=TargetASC->MakeEffectContext();
	//设置效果来源
	EffectContextHandle.AddSourceObject(this);
	//创建GameplayEffect规格
	FGameplayEffectSpecHandle EffectSpecHandle=TargetASC->MakeOutgoingSpec(GameplayEffectClass,1.f,EffectContextHandle);
	
	//应用效果到目标自身，同时返回一个变量(用于记录该效果)
	const FActiveGameplayEffectHandle ActiveEffectHandle=TargetASC->ApplyGameplayEffectSpecToSelf(*EffectSpecHandle.Data.Get());
	//获取当前效果的持续策略
	const UGameplayEffect* GameplayEffect = EffectSpecHandle.Data.Get()->Def.Get();
	//判断当前的效果政策是否等于无限
	const bool bIsInfinite = GameplayEffect->DurationPolicy == EGameplayEffectDurationType::Infinite;
	//如果当前时无限效果策略并且离开碰撞范围时可以移除效果，则在效果集上添加这个效果以及作用的ASC
	if (bIsInfinite&&InfiniteEffectRemovalPolicy==EEffectRemovalPolicy::RemoveOnEndOverlap)
	{
		ActiveEffectHandles.Add(ActiveEffectHandle,TargetASC);
	}
}

void AEffectActor::OnOverlap(AActor* TargetActor)
{
	if (InstantEffectApplicationPolicy==EEffectApplicationPolicy::ApplyOnOverlap)
	{
		ApplyEffectToActor(TargetActor,InstantGameplayEffectClass);
	}
	if (DurationEffectApplicationPolicy==EEffectApplicationPolicy::ApplyOnOverlap)
	{
		ApplyEffectToActor(TargetActor,DurationGameplayEffectClass);
	}
	if (InfiniteEffectApplicationPolicy==EEffectApplicationPolicy::ApplyOnOverlap)
	{
		ApplyEffectToActor(TargetActor,InfiniteGameplayEffectClass);
	}
}

void AEffectActor::OnEndOverlap(AActor* TargetActor)
{
	if (InstantEffectApplicationPolicy==EEffectApplicationPolicy::ApplyOnEndOverlap)
	{
		ApplyEffectToActor(TargetActor,InstantGameplayEffectClass);
	}
	if (DurationEffectApplicationPolicy==EEffectApplicationPolicy::ApplyOnEndOverlap)
	{
		ApplyEffectToActor(TargetActor,DurationGameplayEffectClass);
	}
	if (InfiniteEffectApplicationPolicy==EEffectApplicationPolicy::ApplyOnEndOverlap)
	{
		ApplyEffectToActor(TargetActor,InfiniteGameplayEffectClass);
	}
	if (InfiniteEffectRemovalPolicy==EEffectRemovalPolicy::RemoveOnEndOverlap)
	{
		UAbilitySystemComponent* TargetASC=UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
		if (!IsValid(TargetASC))return;
		// 创建一个临时数组，用于存储需要移除的效果句柄，因为无法在遍历的同时进行删除操作
		TArray<FActiveGameplayEffectHandle> HandlesToRemove;
		// 遍历所有活跃的效果句柄映射（Key:效果句柄, Value:对应的ASC）
		for (TTuple<FActiveGameplayEffectHandle,UAbilitySystemComponent*> HandlePair:ActiveEffectHandles)
		{
			// 检查当前遍历到的ASC是否与目标ASC匹配
			if (TargetASC==HandlePair.Value)
			{
				// 从目标的ASC中移除这个游戏效果
				TargetASC->RemoveActiveGameplayEffect(HandlePair.Key,1);
				// 将这个句柄添加到待移除列表
				HandlesToRemove.Add(HandlePair.Key);
			}
		}
		// 遍历待移除的句柄列表，从ActiveEffectHandles映射中移除它们
		for (FActiveGameplayEffectHandle& Handle:HandlesToRemove)
		{
			// 查找并确认移除指定的句柄
			ActiveEffectHandles.FindAndRemoveChecked(Handle);
		}
	}

}






