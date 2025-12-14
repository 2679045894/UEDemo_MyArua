// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilitys/AuraBeamSpell.h"

#include "GameFramework/Character.h"
#include "Kismet/KismetArrayLibrary.h"
#include "Kismet/KismetSystemLibrary.h"

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

void UAuraBeamSpell::TraceFirstTarget(const FVector& BeamTargetLocation)
{
	if (OwnerPlayerController->GetCharacter()->Implements<UCombatInterface>())
	{
		if (USkeletalMeshComponent* Weapon=ICombatInterface::Execute_GetWeapon(OwnerPlayerController->GetCharacter()))
		{
			TArray<AActor*> ActorsToIgnore;
			FHitResult HitResult;
			ActorsToIgnore.Add(OwnerPlayerController->GetCharacter());
			FVector WeaponLocation=Weapon->GetSocketLocation(FName("TipSocket"));
			UKismetSystemLibrary::SphereTraceSingle(
				GetWorld(),
				WeaponLocation,
				BeamTargetLocation,
				10.f,
				TraceTypeQuery1,
				false,
				ActorsToIgnore,
				EDrawDebugTrace::ForDuration,
				HitResult,
				true
			);
			if (HitResult.bBlockingHit)
			{
				MouseHitLocation=HitResult.ImpactPoint;
				MouseHitActor=HitResult.GetActor();
			}
		}

	}
}
