// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/MyCharacterBase.h"

// Sets default values
AMyCharacterBase::AMyCharacterBase()
{

	PrimaryActorTick.bCanEverTick = true;
	Weapon=CreateDefaultSubobject<USkeletalMeshComponent>("Weapon");
	Weapon->SetupAttachment(GetMesh(),FName("WeaponHandSocket"));
	Weapon->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

// Called when the game starts or when spawned
void AMyCharacterBase::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AMyCharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AMyCharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

UAuraAbilitySystemComponent* AMyCharacterBase::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void AMyCharacterBase::InitialAbilityActorInfo()
{
	
}

void AMyCharacterBase::InitializePrimaryAttribute()
{
	if (UAuraAbilitySystemComponent* ASC=GetAbilitySystemComponent())
	{
		//获取效果上下文
		FGameplayEffectContextHandle EffectContextHandle=ASC->MakeEffectContext();
		//添加源对象
		EffectContextHandle.AddSourceObject(this);
		//创建效果范围
		FGameplayEffectSpecHandle SpecHandle=ASC->MakeOutgoingSpec(DefaultPrimaryAttribute,1.f,EffectContextHandle);
		if (SpecHandle.IsValid())
		{
			//应用效果
			ASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
		}
	}
}

