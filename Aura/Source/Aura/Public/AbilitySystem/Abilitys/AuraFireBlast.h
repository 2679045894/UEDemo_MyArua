// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilitys/AuraProjectileSpell.h"
#include "MyActor/AuraFireBall.h"
#include "AuraFireBlast.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UAuraFireBlast : public UAuraProjectileSpell
{
	GENERATED_BODY()
public:
	UPROPERTY(EditDefaultsOnly,Category="FireBlast")
	TSubclassOf<AAuraFireBall> FireBallClass;
	UPROPERTY(EditDefaultsOnly,Category="FireBlast")
	int32 NumFireBalls=3;
	UFUNCTION(BlueprintCallable)
	TArray<AAuraFireBall*> SpawnFireBall();
protected:
	virtual FString GetDescription(int32 Level) override;
	virtual FString GetNextLevelDescription(int32 Level) override;
	virtual FString GetDescriptionAtLevel(int32 Level, const FString& Title) override;
};
