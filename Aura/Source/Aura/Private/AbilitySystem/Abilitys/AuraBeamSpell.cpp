// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilitys/AuraBeamSpell.h"

void UAuraBeamSpell::StoreMouseDataInfo(const FHitResult& HitResult)
{
	if (HitResult.bBlockingHit)
	{
		MouseHitLocation=HitResult.Location;
		MouseHitActor=HitResult.GetActor();
	}
	else
	{
		CancelAbility(CurrentSpecHandle,CurrentActorInfo,CurrentActivationInfo,true);
	}
}

void UAuraBeamSpell::StoreOwnerVariables()
{
	OwnerPlayerController=CurrentActorInfo->PlayerController.Get();
}
