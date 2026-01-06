// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/EnemyCharacter.h"
#include "Engine/TargetPoint.h"
#include "EnemySpawnPoint.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API AEnemySpawnPoint : public ATargetPoint
{
	GENERATED_BODY()
public:
	virtual void BeginPlay() override;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Enemy Class")
	TSubclassOf<AEnemyCharacter> EnemyClass;
	UFUNCTION()
	void SpawnEnemy() const;
	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite,Category="Info")
	int32 EnemyLevel=1;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy Class")
	ECharacterClass CharacterClass = ECharacterClass::Warrior;
};