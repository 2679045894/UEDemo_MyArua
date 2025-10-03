// Fill out your copyright notice in the Description page of Project Settings.


#include "MyActor/EffectActor.h"

#include "AbilitySystemInterface.h"
#include "GameFramework/Pawn.h"           // 包含APawn的头文件
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
		
		//使用通过接口获取当前玩家的ASC，以此获取注册的属性集（通过当前类）   在Unreal Engine的源码中，所有UObject派生类都有这个StaticClass()静态方法：
		const UAruaAttributeSet* AruaAttributeSet=
			Cast<UAruaAttributeSet>(ASCInterface->GetAbilitySystemComponent()->GetAttributeSet(UAruaAttributeSet::StaticClass()));
		//???????????????????????为什么还要再转换和const_cast???????????????????????????????????????????????????//
		if (UAruaAttributeSet* MutableAruaAttribute=const_cast<UAruaAttributeSet*>(AruaAttributeSet))
		{
			GEngine->AddOnScreenDebugMessage(-1,5.0f,FColor::Red,FString::Printf(TEXT("%f %f"),AruaAttributeSet->GetHealth(),AruaAttributeSet->GetMana()));
			MutableAruaAttribute->SetHealth(AruaAttributeSet->GetHealth()+25.f);
			MutableAruaAttribute->SetMana(AruaAttributeSet->GetMana()-10.f);
			GEngine->AddOnScreenDebugMessage(-1,5.0f,FColor::Red,FString::Printf(TEXT("%f %f"),AruaAttributeSet->GetHealth(),AruaAttributeSet->GetMana()));
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






