// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgeController/AuraWidgetController.h"

#include "AbilitySystem/Data/AbilityInfo.h"


void UAuraWidgetController::SetWidgetController(const FWidgetControllerParams& WCParams)
{
	PlayerState=WCParams.PlayerState;
	PlayerController=WCParams.PlayerController;
	AbilitySystemComponent=WCParams.AbilitySystemComponent;
	AttributeSet=WCParams.AttributeSet;
}

void UAuraWidgetController::BroadcastInitialValues()
{
}

void UAuraWidgetController::BindCallbacksToDependencies()
{
}

AMyPlayerController* UAuraWidgetController::GetAuraPlayerController()
{
	if (AuraPlayerController==nullptr)
	{
		AuraPlayerController=Cast<AMyPlayerController>(PlayerController);
	}
	return AuraPlayerController;
}

AMyPlayerState* UAuraWidgetController::GetAuraPlayerState()
{
	if (AuraPlayerState==nullptr)
	{
		AuraPlayerState=Cast<AMyPlayerState>(PlayerState);
	}
	return AuraPlayerState;
}

UAuraAbilitySystemComponent* UAuraWidgetController::GetAuraAbilitySystemComponent()
{
	if (AuraAbilitySystemComponent==nullptr)
	{
		AuraAbilitySystemComponent=Cast<UAuraAbilitySystemComponent>(AbilitySystemComponent);
	}
	return AuraAbilitySystemComponent;
}

UAuraAttributeSet* UAuraWidgetController::GetAuraAttributeSet()
{
	if (AuraAttributeSet==nullptr)
	{
		AuraAttributeSet=Cast<UAuraAttributeSet>(AttributeSet);
	}
	return AuraAttributeSet;
}

void UAuraWidgetController::BroadcastAbilityInfo()
{
	if (!GetAuraAbilitySystemComponent()->bStartupAbilitiesGiven) return;

	FForEachAbility BroadcastDelegate;
	BroadcastDelegate.BindLambda([this](const FGameplayAbilitySpec& AbilitySpec)
	{
		FAuraAbilityInfo Info = AbilityInfo->FindAbilityInfoForTag(GetAuraAbilitySystemComponent()->GetAbilityTagFormSpec(AbilitySpec));
		Info.InputTag = AuraAbilitySystemComponent->GetInputTagFormSpec(AbilitySpec);
		AbilityInfoDelegate.Broadcast(Info);
	});
	GetAuraAbilitySystemComponent()->ForEachAbility(BroadcastDelegate);
}

