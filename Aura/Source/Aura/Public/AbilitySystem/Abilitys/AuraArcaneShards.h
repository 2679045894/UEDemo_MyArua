// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilitys/AuraProjectileSpell.h"
#include "AuraArcaneShards.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UAuraArcaneShards : public UAuraProjectileSpell
{
	GENERATED_BODY()
public:
	virtual FString GetNextLevelDescription(int32 Level) override;
	virtual FString GetDescription(int32 Level) override;
	virtual FString GetDescriptionAtLevel(int32 Level, const FString& Title) override;
};