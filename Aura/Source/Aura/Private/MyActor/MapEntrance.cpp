// Fill out your copyright notice in the Description page of Project Settings.


#include "MyActor/MapEntrance.h"

#include "Game/MyGameModeBase.h"
#include "Interaction/PlayerInterface.h"
#include "Kismet/GameplayStatics.h"

AMapEntrance::AMapEntrance(const FObjectInitializer& ObjectInitializer):Super(ObjectInitializer)
{
	
}

void AMapEntrance::LoadActor_Implementation()
{
}

void AMapEntrance::OnSphereComponentOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->Implements<UPlayerInterface>())
	{
		if (AMyGameModeBase* AuraGameplayMode=Cast<AMyGameModeBase>(UGameplayStatics::GetGameMode(this)))
		{
			AuraGameplayMode->SaveWorldState(GetWorld(),DestinationMap.ToSoftObjectPath().GetAssetName());
		}
		IPlayerInterface::Execute_SaveProgress(OtherActor,DestinationPlayerStartTag);

		UGameplayStatics::OpenLevelBySoftObjectPtr(this,DestinationMap);
	}
}
