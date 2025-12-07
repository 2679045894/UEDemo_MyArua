// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AuraAbilityTypes.h"
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

	//获取当前GE是否成功应用负面效果
	UFUNCTION(BlueprintPure, Category="RPGAbilitySystemLibrary|GameplayEffects")
	static bool IsSuccessfulDeBuff(const FGameplayEffectContextHandle& EffectContextHandle);

	//获取当前GE负面效果伤害
	UFUNCTION(BlueprintPure, Category="RPGAbilitySystemLibrary|GameplayEffects")
	static float GetDeBuffDamage(const FGameplayEffectContextHandle& EffectContextHandle);

	//获取当前GE负面效果持续时间
	UFUNCTION(BlueprintPure, Category="RPGAbilitySystemLibrary|GameplayEffects")
	static float GetDeBuffDuration(const FGameplayEffectContextHandle& EffectContextHandle);

	//获取当前GE负面效果触发间隔
	UFUNCTION(BlueprintPure, Category="RPGAbilitySystemLibrary|GameplayEffects")
	static float GetDeBuffFrequency(const FGameplayEffectContextHandle& EffectContextHandle);

	//获取当前GE负面效果伤害类型
	UFUNCTION(BlueprintPure, Category="RPGAbilitySystemLibrary|GameplayEffects")
	static FGameplayTag GetDeBuffDamageType(const FGameplayEffectContextHandle& EffectContextHandle);


	UFUNCTION()
	static void SetIsBlockedHit(FGameplayEffectContextHandle& ContextHandle,bool bInIsBlockedHit);

	UFUNCTION()
	static void SetIsCriticalHit(FGameplayEffectContextHandle& ContextHandle,bool bInIsCriticalHit);

	//UPARAM(ref) 使得参数按引用传递，函数内部的修改会反映到调用者
	UFUNCTION(BlueprintPure, Category="RPGAbilitySystemLibrary|GameplayEffects")
	static void SetIsSuccessfulDeBuff(UPARAM(ref) FGameplayEffectContextHandle& ContextHandle,bool bInIsSuccessfulDeBuff);

	UFUNCTION(BlueprintPure, Category="RPGAbilitySystemLibrary|GameplayEffects")
	static void SetDeBuff(UPARAM(ref) FGameplayEffectContextHandle& ContextHandle,FGameplayTag& InDamageType,float InDamage,float InDuration,float InFrequency);
	
	UFUNCTION(BlueprintCallable)
	static void GetLivePlayerWithinRadius(const UObject* WorldContextObject,TArray<AActor*>& OutOverlappingActors,
		const TArray<AActor*>& ActorsToIgnore,float Radius,const FVector& SphereOrigin);
	
	UFUNCTION(BlueprintPure,BlueprintCallable)
	static bool IsNotFriend(AActor* FirstActor,AActor* SecondActor);

	UFUNCTION(BlueprintCallable)
	static int32 GetXPRewardForClassAndLevel(const UObject* WorldContextObject,ECharacterClass CharacterClass,int32 CharacterLevel);

	UFUNCTION(BlueprintCallable)
	static FGameplayEffectContextHandle ApplyDamageEffect(const FDamageEffectParams& DamageEffectParams);
};
