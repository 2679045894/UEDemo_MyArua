// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Data/CharacterClassInfo.h"
#include "Engine/OverlapResult.h"
#include "Interaction/CombatInterface.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "UI/WidgeController/AttributeMenuWidgetController.h"
#include "AuraAbilitySystemLibrary.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UAuraAbilitySystemLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	UFUNCTION(BlueprintPure)
	static UOverplayWidgetController* GetOverlayWidgetController(const UObject* WorldContextObject);

	UFUNCTION(BlueprintPure)
	static UAttributeMenuWidgetController* GetAttributeMenuController(const UObject* WorldContextObject);
public:
	UFUNCTION(BlueprintCallable)
	static void InitializeDefaultAttributes(const UObject* WorldContext,ECharacterClass CharacterClass,float Level,UAbilitySystemComponent* ASC);

	UFUNCTION()
	static void GiveStartupAbilities(const UObject* WorldContext,UAbilitySystemComponent* ASC,ECharacterClass CharacterClass);

	UFUNCTION()
	static UCharacterClassInfo* GetCharacterClassInfo(const UObject* WorldContext);

	UFUNCTION()
	static bool IsBlockedHit(const FGameplayEffectContextHandle& ContextHandle);

	UFUNCTION()
	static bool IsCriticalHit(const FGameplayEffectContextHandle& ContextHandle);

	UFUNCTION()
	static void SetIsBlockedHit(FGameplayEffectContextHandle& ContextHandle,bool bInIsBlockedHit);

	UFUNCTION()
	static void SetIsCriticalHit(FGameplayEffectContextHandle& ContextHandle,bool bInIsCriticalHit);

	UFUNCTION(BlueprintCallable)
	static void GetLivePlayerWithinRadius(const UObject* WorldContextObject,TArray<AActor*>& OutOverlappingActors,
		const TArray<AActor*>& ActorsToIgnore,float Radius,const FVector& SphereOrigin)
	{
		//创建一个碰撞查询配置
		FCollisionQueryParams SphereParams;
		SphereParams.AddIgnoredActors(ActorsToIgnore);
		TArray<FOverlapResult> OverlapResults;
		if (UWorld *World=GEngine->GetWorldFromContextObject(WorldContextObject,EGetWorldErrorMode::LogAndReturnNull))
		{
			World->OverlapMultiByObjectType(OverlapResults,SphereOrigin,
				FQuat::Identity,
				FCollisionObjectQueryParams::InitType::AllDynamicObjects,
				FCollisionShape::MakeSphere(Radius),
				SphereParams);
		}
		for (FOverlapResult& OverlapResult:OverlapResults)
		{
			const bool CombatInterface=OverlapResult.GetActor()->Implements<UCombatInterface>();
			if (CombatInterface&&!ICombatInterface::Execute_IsDead(OverlapResult.GetActor()))
			{
				OutOverlappingActors.AddUnique(OverlapResult.GetActor());
			}
		}
	}
};
