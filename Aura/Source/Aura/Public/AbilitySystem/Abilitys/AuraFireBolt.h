// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilitys/AuraProjectileSpell.h"
#include "AuraFireBolt.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UAuraFireBolt : public UAuraProjectileSpell
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable)
	void SpawnProjectiles(const FVector& ProjectileTargetLocation,const FName SocketName,
		const FGameplayTag& SocketTag,bool bOverridePitch,float PitchOverride,AActor* HomingTarget);
protected:
	float GetManaCost(float InLevel=1.f)const;
	float GetCooldown(float InLevel=1.f)const;

	virtual FString GetDescription(int32 Level) override;
	virtual FString GetNextLevelDescription(int32 Level) override;
	FString GetDescriptionAtLevel(int32 Level,const FString& Title);
public:
	//攻击角度
	UPROPERTY(EditDefaultsOnly,Category="FireBolt")
	float ProjectileSpread=90.f;

	UPROPERTY(EditDefaultsOnly,Category="FireBolt")
	int32 MaxNumProjectiles=5;

	UPROPERTY(EditDefaultsOnly,Category="FireBolt")
	float HomingAccelerationMin=1600.f;

	UPROPERTY(EditDefaultsOnly,Category="FireBolt")
	float HomingAccelerationMax=3200.f;

	UPROPERTY(EditDefaultsOnly,Category="FireBolt")
	bool bLaunchHomingProjectiles=true;

};




