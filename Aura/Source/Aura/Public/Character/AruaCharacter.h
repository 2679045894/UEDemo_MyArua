// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/MyCharacterBase.h"
#include "AruaCharacter.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API AAruaCharacter : public AMyCharacterBase
{
	GENERATED_BODY()
public:
	AAruaCharacter();

	virtual void BeginPlay() override;

};
