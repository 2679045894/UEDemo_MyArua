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

void AMyCharacterBase::InitializeDefaultAttributes() const
{
	ApplyEffectToSelf(DefaultPrimaryAttribute,1.f);
	ApplyEffectToSelf(DefaultSecondaryAttributes,1.f);
}

void AMyCharacterBase::ApplyEffectToSelf(TSubclassOf<UGameplayEffect> GameplayEffectClass, float Level) const
{
	if (UAuraAbilitySystemComponent* TargetASC=Cast<UAuraAbilitySystemComponent>(GetAbilitySystemComponent()))
	{
		FGameplayEffectContextHandle EffectContextHandle=TargetASC->MakeEffectContext();
		EffectContextHandle.AddSourceObject(this);
		FGameplayEffectSpecHandle EffectSpec=TargetASC->MakeOutgoingSpec(GameplayEffectClass,Level,EffectContextHandle);
		if (EffectSpec.IsValid())
		{
			TargetASC->ApplyGameplayEffectSpecToSelf(*EffectSpec.Data.Get());
			GEngine->AddOnScreenDebugMessage(-1,5.f,FColor::Black,TEXT("成功"));
		}
	}
}

