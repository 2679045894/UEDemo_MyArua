// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilitys/AuraProjectileSpell.h"
#include "AuraBeamSpell.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UAuraBeamSpell : public UAuraProjectileSpell
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable)
	void StoreMouseDataInfo(const FHitResult& HitResult);

	UFUNCTION(BlueprintCallable)
	void StoreOwnerVariables();
	
	UPROPERTY(BlueprintReadWrite,Category="Beam")
	FVector MouseHitLocation;

	UPROPERTY(BlueprintReadWrite,Category="Beam")
	TObjectPtr<AActor> MouseHitActor;

	UPROPERTY(BlueprintReadWrite,Category="Beam")
	TObjectPtr<APlayerController> OwnerPlayerController;

	UFUNCTION(BlueprintCallable)
	void TraceFirstTarget(const FVector& BeamTargetLocation);

	UPROPERTY(EditDefaultsOnly,Category="Beam")
	int32 MaxNumShockTargets=5;
	
	UFUNCTION(BlueprintCallable)
	void StoreAdditionalTargets(TArray<AActor*>& OutAdditionalTargets);

	UFUNCTION(BlueprintImplementableEvent)
	void PrimaryTargetDied(AActor* DeadActor);

	UFUNCTION(BlueprintImplementableEvent)
	void AdditionalTargetDied(AActor* DeadActor);

	UFUNCTION(BlueprintCallable)
	void OnEndAbility(TArray<AActor*> AdditionalTargets);
	
	virtual FString GetDescription(int32 Level) override;

	virtual FString GetNextLevelDescription(int32 Level) override;
	
	virtual FString GetDescriptionAtLevel(int32 Level, const FString& Title) override;
};
