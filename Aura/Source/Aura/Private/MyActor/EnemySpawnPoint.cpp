// Fill out your copyright notice in the Description page of Project Settings.


#include "MyActor/EnemySpawnPoint.h"

#include "Character/EnemyCharacter.h"

void AEnemySpawnPoint::BeginPlay()
{
	Super::BeginPlay();
	//SpawnEnemy();
}

void AEnemySpawnPoint::SpawnEnemy() const
{
	check(EnemyClass)
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	AEnemyCharacter* EnemyCharacter=GetWorld()->SpawnActorDeferred<AEnemyCharacter>(
		EnemyClass,
		GetActorTransform());
	EnemyCharacter->SetLevel(EnemyLevel);
	EnemyCharacter->SetCharacterClass(CharacterClass);
	EnemyCharacter->FinishSpawning(GetActorTransform());
	EnemyCharacter->SpawnDefaultController();
}

