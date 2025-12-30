// Fill out your copyright notice in the Description page of Project Settings.


#include "MyActor/EnemySpawnVolume.h"

#include "Interaction/PlayerInterface.h"

// Sets default values
AEnemySpawnVolume::AEnemySpawnVolume()
{
	PrimaryActorTick.bCanEverTick = false;
	BoxComponent=CreateDefaultSubobject<UBoxComponent>(FName("BoxComponent"));
	BoxComponent->SetupAttachment(RootComponent);
	BoxComponent->SetCollisionEnabled(ECollisionEnabled::Type::QueryOnly);
	BoxComponent->SetCollisionObjectType(ECC_WorldStatic);
	BoxComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	BoxComponent->SetCollisionResponseToChannel(ECC_Pawn,ECR_Overlap);
}

void AEnemySpawnVolume::LoadActor_Implementation()
{
	if (bReached)
	{
		Destroy();
	}
}


void AEnemySpawnVolume::OnBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                     UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!OtherActor->Implements<UPlayerInterface>())return;
	bReached=true;
	for (auto& SpawnPoint : SpawnPoints)
	{
		if (IsValid(SpawnPoint))
		{
			SpawnPoint->SpawnEnemy();
		}
	}
	BoxComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AEnemySpawnVolume::BeginPlay()
{
	Super::BeginPlay();
	BoxComponent->OnComponentBeginOverlap.AddDynamic(this,&AEnemySpawnVolume::OnBoxOverlap);
}

