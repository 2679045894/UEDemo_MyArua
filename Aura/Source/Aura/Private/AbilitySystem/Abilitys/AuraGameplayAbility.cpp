// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilitys/AuraGameplayAbility.h"

#include "WorldPartition/Cook/WorldPartitionCookPackage.h"


FString UAuraGameplayAbility::GetDescription(int32 Level)
{
	//const int32 ScaleDamage=DamageTypes[FAuraGameplayTags::Get().Damage_Fire].GetValueAtLevel(Level);
	if (Level==1)
	{
		return FString::Printf(TEXT("未配置"));
	}
	else
	{
		return FString::Printf(TEXT("未配置"));
	}
}

FString UAuraGameplayAbility::GetNextLevelDescription(int32 Level)
{
	//const int32 ScaleDamage=DamageTypes[FAuraGameplayTags::Get().Damage_Fire].GetValueAtLevel(Level+1);
	return FString::Printf(TEXT("未配置"));
}

FString UAuraGameplayAbility::GetLockedDescription(int32 Level)
{
	return FString::Printf(TEXT("当等级达到<Level> %i </>即可解锁"), Level);
}
