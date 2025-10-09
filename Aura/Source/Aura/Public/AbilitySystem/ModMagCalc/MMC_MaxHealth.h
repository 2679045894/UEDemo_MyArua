// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayModMagnitudeCalculation.h"
#include "MMC_MaxHealth.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UMMC_MaxHealth : public UGameplayModMagnitudeCalculation
{
	GENERATED_BODY()
public:
	UMMC_MaxHealth();

	//为Gameplay Effect提供动态的、可定制的数值计算逻辑。
	virtual float CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const override;

private:
	//创建一个属性捕获
	FGameplayEffectAttributeCaptureDefinition VigorDef;
	
};
