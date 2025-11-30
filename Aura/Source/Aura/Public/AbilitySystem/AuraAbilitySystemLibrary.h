// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Data/CharacterClassInfo.h"
#include "Engine/OverlapResult.h"
#include "Interaction/CombatInterface.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "UI/HUD/AuraHUD.h"
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
	static bool MakeWidgetControllerParams(const UObject* WorldObject,FWidgetControllerParams &OutWCParams,AAuraHUD*& OutAuraHUD);
	//// 自动绑定调用者作为 WorldContextObject
	UFUNCTION(BlueprintPure,meta = (DefaultToSelf = "WorldContextObject"))
	static UOverplayWidgetController* GetOverlayWidgetController(const UObject* WorldContextObject);

	UFUNCTION(BlueprintPure,meta = (DefaultToSelf = "WorldContextObject"))
	static UAttributeMenuWidgetController* GetAttributeMenuController(const UObject* WorldContextObject);

	UFUNCTION(BlueprintPure,meta = (DefaultToSelf = "WorldContextObject"))
	static USpellMenuWidgetController* GetSpellMenuController(const UObject* WorldContextObject);
public:
	UFUNCTION(BlueprintCallable)
	static void InitializeDefaultAttributes(const UObject* WorldContext,ECharacterClass CharacterClass,float Level,UAbilitySystemComponent* ASC);

	UFUNCTION()
	static void GiveStartupAbilities(const UObject* WorldContext,UAbilitySystemComponent* ASC,ECharacterClass CharacterClass);

	UFUNCTION()
	static UCharacterClassInfo* GetCharacterClassInfo(const UObject* WorldContext);

	UFUNCTION()
	static UAbilityInfo* GetActiveAbilityInfo(const UObject* WorldContext);

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
		const TArray<AActor*>& ActorsToIgnore,float Radius,const FVector& SphereOrigin);
	
	UFUNCTION(BlueprintPure,BlueprintCallable)
	static bool IsNotFriend(AActor* FirstActor,AActor* SecondActor);

	UFUNCTION(BlueprintCallable)
	static int32 GetXPRewardForClassAndLevel(const UObject* WorldContextObject,ECharacterClass CharacterClass,int32 CharacterLevel);
};
