// Fill out your copyright notice in the Description page of Project Settings.


#include "AuraAssetManager.h"

#include "AuraGameplayTags.h"
#include "AbilitySystemGlobals.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"

UAuraAssetManager::UAuraAssetManager()
{
}

UAuraAssetManager& UAuraAssetManager::Get()
{
	check(GEngine);
	//在外部配置ini文件
	UAuraAssetManager* AuraAssetManager=Cast<UAuraAssetManager>(GEngine->AssetManager);
	return *AuraAssetManager;
}


void UAuraAssetManager::StartInitialLoading()
{
	Super::StartInitialLoading();
	FAuraGameplayTags::InitializeNativeGameplayTags();

	UAbilitySystemGlobals::Get().InitGlobalData();
}
