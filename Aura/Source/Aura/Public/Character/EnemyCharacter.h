// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/MyCharacterBase.h"
#include "Interaction/EnemyInterface.h"
#include "EnemyCharacter.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API AEnemyCharacter : public AMyCharacterBase,public IEnemyInterface
{
	GENERATED_BODY()

	virtual void BeginPlay() override;
public:
	AEnemyCharacter();
	virtual void HighlightActor() override;
	virtual void UnHighlightActor() override;
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Highlighting")
	bool bHighlight;
};
