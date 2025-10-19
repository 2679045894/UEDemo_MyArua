// Fill out your copyright notice in the Description page of Project Settings.


#include "MyActor/AuraProjectile.h"

// Sets default values
AAuraProjectile::AAuraProjectile()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates=true;
	SphereComponent=CreateDefaultSubobject<USphereComponent>(FName("SphereComponent"));
	SetRootComponent(SphereComponent);
	SphereComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	SphereComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	SphereComponent->SetCollisionResponseToChannel(ECC_WorldDynamic,ECR_Overlap);
	SphereComponent->SetCollisionResponseToChannel(ECC_WorldStatic,ECR_Overlap);
	SphereComponent->SetCollisionResponseToChannel(ECC_Pawn,ECR_Overlap);

	ProjectileMovementComponent=CreateDefaultSubobject<UProjectileMovementComponent>(FName("ProjectileMovementComponent"));
	ProjectileMovementComponent->InitialSpeed=450.f;
	ProjectileMovementComponent->MaxSpeed=550.f;
	ProjectileMovementComponent->ProjectileGravityScale=0.f;
}


void AAuraProjectile::BeginPlay()
{
	Super::BeginPlay();
	
}


