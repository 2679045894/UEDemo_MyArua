// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MagicCircle.generated.h"

UCLASS(Blueprintable,BlueprintType)
class AURA_API AMagicCircle : public AActor
{
	GENERATED_BODY()
	
public:	
	AMagicCircle();
	UPROPERTY(BlueprintReadWrite,EditDefaultsOnly,Category = "Components")
	TObjectPtr<UDecalComponent> MagicCircleDecal;

protected:

	virtual void BeginPlay() override;

public:	

};
