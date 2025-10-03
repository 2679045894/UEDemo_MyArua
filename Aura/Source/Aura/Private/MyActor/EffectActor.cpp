// Fill out your copyright notice in the Description page of Project Settings.


#include "MyActor/EffectActor.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "GameFramework/Pawn.h"           // 包含APawn的头文件
#include "AbilitySystem/AruaAttributeSet.h"

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
	FGameplayEffectSpecHandle SpecHandle=TargetASC->MakeOutgoingSpec(GameplayEffectClass,1.f,EffectContextHandle);
	//应用效果到目标自身
	TargetASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
}






