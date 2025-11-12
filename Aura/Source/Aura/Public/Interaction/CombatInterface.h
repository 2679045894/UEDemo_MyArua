// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "CombatInterface.generated.h"

class UAnimMontage;
UINTERFACE(MinimalAPI, Blueprintable, BlueprintType)
class UCombatInterface : public UInterface
{
	GENERATED_BODY()
};


class AURA_API ICombatInterface
{
	GENERATED_BODY()

public:
	virtual int32 GetPlayerLevel();

	UFUNCTION(BlueprintCallable,BlueprintNativeEvent)
	FVector GetCombatSocketLocation();
	
	UFUNCTION(BlueprintImplementableEvent,BlueprintCallable)
	void UpdateFacingTarget(const FVector& TargetLocation);

	UFUNCTION(BlueprintNativeEvent,BlueprintCallable)
	UAnimMontage* GetHitReactMontage();

	UFUNCTION()
	virtual void Die()=0;

	UFUNCTION(BlueprintNativeEvent,BlueprintCallable)
	bool IsDead();

	UFUNCTION(BlueprintNativeEvent,BlueprintCallable)
	AActor* GetAvatar();
};
