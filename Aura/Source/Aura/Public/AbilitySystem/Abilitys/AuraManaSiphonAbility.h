// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilitys/AuraPassiveAbility.h"
#include "AuraManaSiphonAbility.generated.h"

/**
 * 
 */
UCLASS(BlueprintType,Blueprintable)
class AURA_API UAuraManaSiphonAbility : public UAuraPassiveAbility
{
	GENERATED_BODY()
public:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	UFUNCTION()
	virtual FString GetDescription(int32 Level) override;
	UFUNCTION()
	virtual FString GetDescriptionAtLevel(int32 Level, const FString& Title) override;
	UFUNCTION()
	virtual FString GetNextLevelDescription(int32 Level) override;
};
