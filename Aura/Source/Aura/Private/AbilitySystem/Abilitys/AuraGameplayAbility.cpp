// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilitys/AuraGameplayAbility.h"

#include "WorldPartition/Cook/WorldPartitionCookPackage.h"

FString UAuraGameplayAbility::GetDescription(int32 Level)
{
	return FString::Printf(TEXT("%s,<Level>%d</>"),L"默认技能名称-火球术",Level);
}

FString UAuraGameplayAbility::GetNextLevelDescription(int32 Level)
{
	return FString::Printf(TEXT("下一等级：<Level>%d</> 造成更多伤害。"),Level);
}

FString UAuraGameplayAbility::GetLockedDescription(int32 Level)
{
	return FString::Printf(TEXT("技能将在角色达到<Level>%d</>时解锁"),Level);
}
