// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MyActor/AuraProjectile.h"
#include "AuraFireBall.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API AAuraFireBall : public AAuraProjectile
{
	GENERATED_BODY()
public:
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintImplementableEvent)
	void StartOutgongTimeLine();

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<AActor> ReturnToActor;
};
