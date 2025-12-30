// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnemySpawnPoint.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Actor.h"
#include "Interaction/SaveInterface.h"
#include "EnemySpawnVolume.generated.h"

UCLASS()
class AURA_API AEnemySpawnVolume : public AActor,public ISaveInterface
{
	GENERATED_BODY()
	
public:	
	AEnemySpawnVolume();

	virtual void LoadActor_Implementation() override;

	UPROPERTY(EditDefaultsOnly)
	bool bReached=false;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TObjectPtr<UBoxComponent> BoxComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<AEnemySpawnPoint*> SpawnPoints;

	UFUNCTION()
	void OnBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

protected:
	virtual void BeginPlay() override;

};
