// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilitys/AuraBeamSpell.h"

#include "AuraGameplayTags.h"
#include "AbilitySystem/AuraAbilitySystemLibrary.h"
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
		if (ICombatInterface* CombatInterface=Cast<ICombatInterface>(MouseHitActor))
		{
			if (!CombatInterface->GetOnDeathDelegate().IsAlreadyBound(this,&UAuraBeamSpell::PrimaryTargetDied))
			{
				CombatInterface->GetOnDeathDelegate().AddDynamic(this,&UAuraBeamSpell::PrimaryTargetDied);
			}
		}
	}
}

void UAuraBeamSpell::StoreAdditionalTargets(TArray<AActor*>& OutAdditionalTargets)
{
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(GetAvatarActorFromActorInfo());
	ActorsToIgnore.Add(MouseHitActor);
	TArray<AActor*> OverlappingActors;
	UAuraAbilitySystemLibrary::GetLivePlayerWithinRadius(
		GetWorld(),
		OverlappingActors,
		ActorsToIgnore,
		850.f,
		MouseHitLocation);
	int32 NumAdditionalTargets=5;
	UAuraAbilitySystemLibrary::GetClosestTargets(OverlappingActors,OutAdditionalTargets,NumAdditionalTargets,MouseHitLocation);
	for (AActor* Actor:OutAdditionalTargets)
	{
		if (ICombatInterface* CombatInterface=Cast<ICombatInterface>(Actor))
		{
			if (!CombatInterface->GetOnDeathDelegate().IsAlreadyBound(this,&UAuraBeamSpell::AdditionalTargetDied))
			{
				CombatInterface->GetOnDeathDelegate().AddDynamic(this,&UAuraBeamSpell::AdditionalTargetDied);
			}
		}
	}
}

void UAuraBeamSpell::OnEndAbility(TArray<AActor*> AdditionalTargets)
{
	if (ICombatInterface* CombatInterface=Cast<ICombatInterface>(MouseHitActor))
	{
		CombatInterface->GetOnDeathDelegate().RemoveDynamic(this,&UAuraBeamSpell::PrimaryTargetDied);
	}
	for (AActor* Actor:AdditionalTargets)
	{
		if (ICombatInterface* CombatInterface=Cast<ICombatInterface>(Actor))
		{
			CombatInterface->GetOnDeathDelegate().RemoveDynamic(this,&UAuraBeamSpell::AdditionalTargetDied);
		}
	}
}

FString UAuraBeamSpell::GetDescription(int32 Level)
{
	return GetDescriptionAtLevel(Level,AbilityName);
}

FString UAuraBeamSpell::GetNextLevelDescription(int32 Level)
{
	return GetDescriptionAtLevel(Level+1,TEXT("下一等级"));
}

FString UAuraBeamSpell::GetDescriptionAtLevel(int32 Level, const FString& Title)
{
	float Cooldown = GetCooldown(Level);
	float ManaCost=FMath::Abs(GetManaCost(Level));
	int32 Damage=GetDamageByDamageType(Level,FAuraGameplayTags::Get().Damage_Lightning);
	return FString::Printf(TEXT(
		"<Title>%s</>\n"
		"等级:<Level>%i</>\n"
		"技能冷却<Cooldown>%.1f</>\n"
		"蓝量消耗<ManaCost>%.1f</>\n"
		"<Default>引导一道高能激光束，对路径上的所有敌人每秒造成</> <Damage>%i</> <Default>点能量伤害。激光可穿透敌人，并对护盾造成额外伤害。持续引导超过4秒会使技能过载。</>"
	),*Title,Level,Cooldown,ManaCost,Damage);
}

