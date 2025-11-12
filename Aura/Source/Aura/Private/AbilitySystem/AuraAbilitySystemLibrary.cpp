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

void UAuraAbilitySystemLibrary::GiveStartupAbilities(const UObject* WorldContext, UAbilitySystemComponent* ASC,ECharacterClass CharacterClass)
{
	UCharacterClassInfo* CharacterClassInfo=GetCharacterClassInfo(WorldContext);
	if (CharacterClassInfo==nullptr) return;
	AMyGameModeBase* AuraGameMode=Cast<AMyGameModeBase>(UGameplayStatics::GetGameMode(WorldContext));
	if (!AuraGameMode) return;
	for (TSubclassOf<UGameplayAbility> AbilityClass:GetCharacterClassInfo(WorldContext)->CommonAbilitiesClass)
	{
		//定义所有职业共有的基础能力
		FGameplayAbilitySpec AbilitySpec=FGameplayAbilitySpec(AbilityClass,1);
		ASC->GiveAbility(AbilitySpec);
	}
	for (TSubclassOf<UGameplayAbility> AbilityClass:CharacterClassInfo->GetCharacterClassDefaultInfo(CharacterClass).StartupAbilitiesClass)
	{
		if (ICombatInterface* CombatInterface=Cast<ICombatInterface>(ASC->GetAvatarActor()))
		{
			FGameplayAbilitySpec AbilitySpec=FGameplayAbilitySpec(AbilityClass,CombatInterface->GetPlayerLevel());
			ASC->GiveAbility(AbilitySpec);
		}
	}
}

UCharacterClassInfo* UAuraAbilitySystemLibrary::GetCharacterClassInfo(const UObject* WorldContext)
{
	//通过访问MyGameMode中的CharacterClassInfo
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
}

void UAuraAbilitySystemLibrary::GetLivePlayerWithinRadius(const UObject* WorldContextObject,
	TArray<AActor*>& OutOverlappingActors, const TArray<AActor*>& ActorsToIgnore, float Radius,
	const FVector& SphereOrigin)
{
	//创建一个碰撞查询配置
	FCollisionQueryParams SphereParams;
	//添加忽略对象
	SphereParams.AddIgnoredActors(ActorsToIgnore);
	TArray<FOverlapResult> OverlapResults;
	//LogAndReturnNull：当无法获取有效的 World 时，不会让游戏崩溃，而是优雅地返回 nullptr。
	if (UWorld *World=GEngine->GetWorldFromContextObject(WorldContextObject,EGetWorldErrorMode::LogAndReturnNull))
	{
		//执行球形范围检测
		World->OverlapMultiByObjectType(OverlapResults,//输出：存储检测结果
			SphereOrigin,
			FQuat::Identity,// 输入：旋转（无旋转）
			FCollisionObjectQueryParams::InitType::AllDynamicObjects,// 输入：检测对象类型
			FCollisionShape::MakeSphere(Radius),// 输入：碰撞形状
			SphereParams);//输入：额外参数
	}
	for (FOverlapResult& OverlapResult:OverlapResults)
	{
		//将检测到的未死亡的敌人或玩家添加到数组中
		const bool CombatInterface=OverlapResult.GetActor()->Implements<UCombatInterface>();
		if (CombatInterface&&!ICombatInterface::Execute_IsDead(OverlapResult.GetActor()))
		{
			//AddUnique：如果当前数组中不存在这个对象，才进行add
			OutOverlappingActors.AddUnique(OverlapResult.GetActor());
		}
	}
}




