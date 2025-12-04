// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AuraGameplayTags.h"
#include "UI/WidgeController/AuraWidgetController.h"
#include "SpellMenuWidgetController.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FSpellGlobeSlectedSignature,bool,bSpendPointsEnable,bool,bEquipEnabled,bool,bDemotionPointsEnabled,int32,Level);

struct FSelectedAbility
{
	FGameplayTag Ability=FGameplayTag();
	FGameplayTag Status=FGameplayTag();
	int32 Level=0;
};

UCLASS(Blueprintable,BlueprintType)
class AURA_API USpellMenuWidgetController : public UAuraWidgetController
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable)
	virtual void BroadcastInitialValues() override;
	virtual void BindCallbacksToDependencies() override;

	UPROPERTY(BlueprintAssignable)
	FSpellGlobeSlectedSignature OnSpellGlobeSelectedDelegate;
	FSelectedAbility SelectedAbility={FAuraGameplayTags::Get().Abilities_None,FAuraGameplayTags::Get().Abilities_Status_Locked};

	//保存当前技能可分配点数
	int32 CurrentSpellPoints=0;

	UFUNCTION(BlueprintCallable)
	FGameplayTag SpellGlobeSelected(const FGameplayTag& AbilityTag);
	
	UFUNCTION()
	static void ShouldEnableButtons(const FGameplayTag& AbilityStatus,bool HasSpellPoints,bool& bShouldEnableSpellPoints,bool& bShouldEnableEquip,bool& bShouldDemotionPoints);

	void BroadcastSpellGlobeSelected()const;

	UFUNCTION(BlueprintCallable)
	void SpendPointButtonPressed(const FGameplayTag &AbilityTag);

	UFUNCTION(BlueprintCallable)
	void DemotionPointButtonPressed(const FGameplayTag &AbilityTag);
};
