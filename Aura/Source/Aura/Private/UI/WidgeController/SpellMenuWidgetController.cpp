// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgeController/SpellMenuWidgetController.h"

void USpellMenuWidgetController::BroadcastInitialValues()
{
	SpellPointChanged.Broadcast(GetAuraPlayerState()->GetSpellPoints());
	BroadcastAbilityInfo();
}

void USpellMenuWidgetController::BindCallbacksToDependencies()
{
	GetAuraAbilitySystemComponent()->AbilityStatusChangedDelegate.AddLambda(
		[this](const FGameplayTag& AbilityTag,const FGameplayTag& StatusTag)
	{
		if (AbilityInfo)
		{
			FAuraAbilityInfo Info=AbilityInfo->FindAbilityInfoForTag(AbilityTag);
			Info.StatusTag=StatusTag;
			AbilityInfoDelegate.Broadcast(Info);
		}
	});

	GetAuraPlayerState()->OnSpellPointsChangedDelegate.AddLambda([this](const int32 SpellPoints)
	{
		SpellPointChanged.Broadcast(SpellPoints);
	});
}
