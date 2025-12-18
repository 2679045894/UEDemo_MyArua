// Fill out your copyright notice in the Description page of Project Settings.


#include "MyActor/MagicCircle.h"

#include "Components/DecalComponent.h"

// Sets default values
AMagicCircle::AMagicCircle()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	MagicCircleDecal=CreateDefaultSubobject<UDecalComponent>("DecalComponent");
	MagicCircleDecal->SetupAttachment(GetRootComponent());
}

// Called when the game starts or when spawned
void AMagicCircle::BeginPlay()
{
	Super::BeginPlay();
	
}


