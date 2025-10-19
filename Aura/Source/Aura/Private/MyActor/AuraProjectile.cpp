// Fill out your copyright notice in the Description page of Project Settings.


#include "MyActor/AuraProjectile.h"

// Sets default values
AAuraProjectile::AAuraProjectile()
{
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void AAuraProjectile::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AAuraProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

