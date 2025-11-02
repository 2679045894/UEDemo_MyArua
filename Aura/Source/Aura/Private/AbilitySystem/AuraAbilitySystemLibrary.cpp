// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/AuraAbilitySystemLibrary.h"

#include "AuraAbilityTypes.h"
#include "Game/MyGameModeBase.h"
#include "kismet/GameplayStatics.h"
#include "Player/MyPlayerState.h"
#include "Character/AuraCharacter.h"
#include "UI/HUD/AuraHUD.h"

UOverplayWidgetController* UAuraAbilitySystemLibrary::GetOverlayWidgetController(const UObject* WorldContextObject)
{
	if (APlayerController* PlayerController=UGameplayStatics::GetPlayerController(WorldContextObject,0))
	{
		if (AAuraHUD* AuraHUD=Cast<AAuraHUD>(PlayerController->GetHUD()))
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

UAttributeMenuWidgetController* UAuraAbilitySystemLibrary::GetAttributeMenuController(const UObject* WorldContextObject)
{
	if (APlayerController* PlayerController=UGameplayStatics::GetPlayerController(WorldContextObject,0))
	{
		if (AAuraHUD* AuraHUD=Cast<AAuraHUD>(PlayerController->GetHUD()))
		{
			AMyPlayerState* PS=PlayerController->GetPlayerState<AMyPlayerState>();
			UAuraAbilitySystemComponent* ASC=PS->GetAbilitySystemComponent();
			UAuraAttributeSet* AS=PS->GetAttributeSet();
			const FWidgetControllerParams WCParams(PlayerController,PS,ASC,AS);
			return AuraHUD->GetAttributeMenuWidgetController(WCParams);
		}
	}
	return nullptr;
}

void UAuraAbilitySystemLibrary::InitializeDefaultAttributes(const UObject* WorldContextObject, ECharacterClass CharacterClass, float Level, UAbilitySystemComponent* ASC)
{
	AMyGameModeBase* AuraGameMode = Cast<AMyGameModeBase>(UGameplayStatics::GetGameMode(WorldContextObject));
	if (AuraGameMode == nullptr) return;

	//初始化对象(用于context)
	AActor* AvatarActor = ASC->GetAvatarActor();

	UCharacterClassInfo* CharacterClassInfo = AuraGameMode->CharacterClassInfo;
	FCharacterClassDefault ClassDefaultInfo = CharacterClassInfo->GetCharacterClassDefaultInfo(CharacterClass);

	FGameplayEffectContextHandle PrimaryAttributesContextHandle = ASC->MakeEffectContext();
	PrimaryAttributesContextHandle.AddSourceObject(AvatarActor);
	const FGameplayEffectSpecHandle PrimaryAttributesSpecHandle = ASC->MakeOutgoingSpec(
		ClassDefaultInfo.PrimaryAttributeClass, Level, PrimaryAttributesContextHandle);
	ASC->ApplyGameplayEffectSpecToSelf(*PrimaryAttributesSpecHandle.Data.Get());

	FGameplayEffectContextHandle SecondaryAttributesContextHandle = ASC->MakeEffectContext();
	SecondaryAttributesContextHandle.AddSourceObject(AvatarActor);
	const FGameplayEffectSpecHandle SecondaryAttributesSpecHandle = ASC->MakeOutgoingSpec(CharacterClassInfo->SecondaryAttributeClass, Level, SecondaryAttributesContextHandle);
	ASC->ApplyGameplayEffectSpecToSelf(*SecondaryAttributesSpecHandle.Data.Get());

	FGameplayEffectContextHandle VitalAttributesContextHandle = ASC->MakeEffectContext();
	VitalAttributesContextHandle.AddSourceObject(AvatarActor);
	const FGameplayEffectSpecHandle VitalAttributesSpecHandle = ASC->MakeOutgoingSpec(CharacterClassInfo->VitalAttributeClass, Level, VitalAttributesContextHandle);
	ASC->ApplyGameplayEffectSpecToSelf(*VitalAttributesSpecHandle.Data.Get());
	
}

void UAuraAbilitySystemLibrary::GiveStartupAbilities(const UObject* WorldContext, UAbilitySystemComponent* ASC)
{
	AMyGameModeBase* AuraGameMode=Cast<AMyGameModeBase>(UGameplayStatics::GetGameMode(WorldContext));
	if (!AuraGameMode) return;
	for (TSubclassOf<UGameplayAbility> AbilityClass:GetCharacterClassInfo(WorldContext)->CommonAbilitiesClass)
	{
		//定义所有职业共有的基础能力
		FGameplayAbilitySpec AbilitySpec=FGameplayAbilitySpec(AbilityClass,1);
		ASC->GiveAbility(AbilitySpec);
	}
}

UCharacterClassInfo* UAuraAbilitySystemLibrary::GetCharacterClassInfo(const UObject* WorldContext)
{
	if (AMyGameModeBase* AuraGameMode=Cast<AMyGameModeBase>(UGameplayStatics::GetGameMode(WorldContext)))
	{
		return AuraGameMode->CharacterClassInfo;
	}
	return nullptr;
}

bool UAuraAbilitySystemLibrary::IsBlockedHit(const FGameplayEffectContextHandle& ContextHandle)
{
	if (const FAuraGameplayContext* AuraContext=static_cast<const FAuraGameplayContext*>(ContextHandle.Get()))
	{
		return AuraContext->IsBlockedHit();
	}
	return false;
}

bool UAuraAbilitySystemLibrary::IsCriticalHit(const FGameplayEffectContextHandle& ContextHandle)
{
	if (const FAuraGameplayContext* AuraContext=static_cast<const FAuraGameplayContext*>(ContextHandle.Get()))
	{
		return AuraContext->IsCriticalHit();
	}
	return false;
}

void UAuraAbilitySystemLibrary::SetIsBlockedHit(FGameplayEffectContextHandle& ContextHandle, bool bInIsBlockedHit)
{
	if (FAuraGameplayContext* AuraContext=static_cast<FAuraGameplayContext*>(ContextHandle.Get()))
	{
		AuraContext->SetIsBlockedHit(bInIsBlockedHit);
	}
}

void UAuraAbilitySystemLibrary::SetIsCriticalHit(FGameplayEffectContextHandle& ContextHandle, bool bInIsCriticalHit)
{
	if (FAuraGameplayContext* AuraContext=static_cast<FAuraGameplayContext*>(ContextHandle.Get()))
	{
		AuraContext->SetIsCriticalHit(bInIsCriticalHit);
	}
	else
	{
		UE_LOG(LogTemp,Warning,TEXT("fa"));
	}
}




