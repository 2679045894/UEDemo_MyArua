// Fill out your copyright notice in the Description page of Project Settings.


#include "MyActor/MagicCircle.h"

#include "Components/DecalComponent.h"

// Sets default values
AMagicCircle::AMagicCircle()
{
 	
	PrimaryActorTick.bCanEverTick = true;
	SceneComponent=CreateDefaultSubobject<USceneComponent>(FName("Root"));
	SetRootComponent(SceneComponent);
	MagicCircleDecal=CreateDefaultSubobject<UDecalComponent>("DecalComponent");
	MagicCircleDecal->SetupAttachment(GetRootComponent());
}

// Called when the game starts or when spawned
void AMagicCircle::BeginPlay()
{
	Super::BeginPlay();
	
}


