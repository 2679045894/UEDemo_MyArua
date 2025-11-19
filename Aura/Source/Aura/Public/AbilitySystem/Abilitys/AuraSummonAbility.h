// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilitys/AuraGameplayAbility.h"
#include "AuraSummonAbility.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UAuraSummonAbility : public UAuraGameplayAbility
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable)
	TArray<FVector> GetSpawnLocations();

	//召唤的数量
	UPROPERTY(EditDefaultsOnly,Category="Summoning")
	int32 NumMinions=5;

	UPROPERTY(EditDefaultsOnly,Category="Summoning")
	TArray<TSubclassOf<APawn>> MinionClasses;

	UPROPERTY(EditDefaultsOnly,Category="Summoning")
	float MinSpawnDistance=50.f;

	UPROPERTY(EditDefaultsOnly,Category="Summoning")
	float MaxSpawnDistance=250.f;

	//召唤物在召唤师前面的角度范围·
	UPROPERTY(EditDefaultsOnly,Category="Summoning")
	float SpawnSpread=90.f;

	UFUNCTION(BlueprintPure)
	TSubclassOf<APawn> GetRandomMinionClass();
};
