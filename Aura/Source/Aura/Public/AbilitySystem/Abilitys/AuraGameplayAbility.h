// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "AuraGameplayAbility.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UAuraGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly)
	FString AbilityName="Default";
	UPROPERTY(EditAnywhere)
	FGameplayTag StartupInputTag;
	float GetManaCost(float InLevel=1.f)const;
	float GetCooldown(float InLevel=1.f)const;

	virtual FString GetDescription(int32 Level);
	virtual FString GetNextLevelDescription(int32 Level);

	virtual FString GetDescriptionAtLevel(int32 Level,const FString& Title);
	//锁定技能描述
	static FString GetLockedDescription(int32 Level);
};
