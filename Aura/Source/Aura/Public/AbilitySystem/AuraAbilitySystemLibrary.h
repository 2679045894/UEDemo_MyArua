// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Data/CharacterClassInfo.h"
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
};
