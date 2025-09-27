// Fill out your copyright notice in the Description page of Project Settings.


#include "MyActor/EffectActor.h"

#include "AbilitySystemInterface.h"
#include "GameFramework/Pawn.h"           // 包含APawn的头文件
#include "GameFramework/PlayerState.h" 
#include "AbilitySystem/AruaAttributeSet.h"


AEffectActor::AEffectActor()
{
	PrimaryActorTick.bCanEverTick = false;
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	SetRootComponent(StaticMeshComponent);
	SphereComponent->SetupAttachment(GetRootComponent());
	SphereComponent->OnComponentBeginOverlap.AddDynamic(this,&AEffectActor::OnOverlap);
}


void AEffectActor::BeginPlay()
{
	Super::BeginPlay();
	
}

void AEffectActor::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//判断当前触碰到的角色是否继承IAbilitySystemInterface接口，并将其转换为接口
	if (IAbilitySystemInterface* ASCInterface=Cast<IAbilitySystemInterface>(OtherActor))
	{
		
		/*if (UAbilitySystemComponent* ASCComp=ASCInterface->GetAbilitySystemComponent())
		{
			if (const UAruaAttributeSet* AruaAttributeSet=Cast<UAruaAttributeSet>(ASCComp->GetAttributeSet(UAruaAttributeSet::StaticClass())))
			{
				GEngine->AddOnScreenDebugMessage(-1,5.0f,FColor::Red,TEXT("1.2"));
			}
		}*/
		//使用通过接口获取当前玩家的ASC，以此获取注册的属性集（通过当前类）   在Unreal Engine的源码中，所有UObject派生类都有这个StaticClass()静态方法：
		const UAruaAttributeSet* AruaAttributeSet=
			Cast<UAruaAttributeSet>(ASCInterface->GetAbilitySystemComponent()->GetAttributeSet(UAruaAttributeSet::StaticClass()));
		//???????????????????????为什么还要再转换和const_cast???????????????????????????????????????????????????//
		if (UAruaAttributeSet* MutableAruaAttribute=const_cast<UAruaAttributeSet*>(AruaAttributeSet))
		{
			MutableAruaAttribute->SetHealth(AruaAttributeSet->GetHealth()+25.f);
			GEngine->AddOnScreenDebugMessage(-1,5.0f,FColor::Red,FString::Printf(TEXT("%f"),MutableAruaAttribute->GetHealth()));
			Destroy();
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(-1,5.0f,FColor::Red,TEXT("2"));
		}

	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1,5.0f,FColor::Red,TEXT("1"));
	}
	
}

/*void AEffectActor::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (IAbilitySystemInterface* ASCInterface = Cast<IAbilitySystemInterface>(OtherActor))
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, TEXT("✓ 实现了IAbilitySystemInterface"));
        
		if (UAbilitySystemComponent* ASCComp = ASCInterface->GetAbilitySystemComponent())
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, TEXT("1.1 - 找到ASC"));
            
			// 详细调试：先检查GetAttributeSet方法是否返回任何东西
			const UAttributeSet* BaseAttributeSet = ASCComp->GetAttributeSet(UAruaAttributeSet::StaticClass());
			if (BaseAttributeSet)
			{
				GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, 
					FString::Printf(TEXT("GetAttributeSet返回: %s"), *BaseAttributeSet->GetClass()->GetName()));
			}
			else
			{
				GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("GetAttributeSet返回nullptr"));
			}
            
			const UAruaAttributeSet* AruaAttributeSet = Cast<UAruaAttributeSet>(BaseAttributeSet);
			if (AruaAttributeSet)
			{
				GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, TEXT("1.2 - 成功获取UAruaAttributeSet"));
			}
			else
			{
				GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("1.2 - Cast到UAruaAttributeSet失败"));
			}
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("ASC为nullptr"));
		}
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("未实现IAbilitySystemInterface"));
	}
}*/





