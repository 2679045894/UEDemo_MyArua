// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "kismet/GameplayStatics.h"
#include "Player/MyPlayerState.h"
#include "UI/HUD/AuraHUD.h"

UOverplayWidgetController* UAuraAbilitySystemLibrary::GetOverlayWidgetController(const UObject* WorldContextObject)
{
	if (APlayerController* PlayerController=UGameplayStatics::GetPlayerController(WorldContextObject,0))
	{
		if (AAuraHUD* AuraHUD=Cast<AAuraHUD>(PlayerController))
		{
			AMyPlayerState* PS=PlayerController->GetPlayerState<AMyPlayerState>();
			UAuraAbilitySystemComponent* ASC=PS->GetAbilitySystemComponent();
			UAuraAttributeSet* AS=PS->GetAttributeSet();
			const FWidgetControllerParams WCParams(PlayerController, PS, ASC, AS);
			return  AuraHUD->GetOverPlayWidgetController(WCParams);
		}
	}
	return nullptr;
}
