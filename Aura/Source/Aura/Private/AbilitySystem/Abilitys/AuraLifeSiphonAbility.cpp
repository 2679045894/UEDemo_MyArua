// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilitys/AuraLifeSiphonAbility.h"

void UAuraLifeSiphonAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

FString UAuraLifeSiphonAbility::GetDescription(int32 Level)
{
	return GetDescriptionAtLevel(Level,AbilityName);
}

FString UAuraLifeSiphonAbility::GetDescriptionAtLevel(int32 Level, const FString& Title)
{
	return FString::Printf(TEXT(
"<Title>%s</>\n"
"等级:<Level>%i</>\n"
"<Default>基于角色的活力值，每秒恢复血量值。</>"),
*Title,
Level
);
}

FString UAuraLifeSiphonAbility::GetNextLevelDescription(int32 Level)
{
	return FString::Printf(TEXT("被动技能无需升级"));
}
