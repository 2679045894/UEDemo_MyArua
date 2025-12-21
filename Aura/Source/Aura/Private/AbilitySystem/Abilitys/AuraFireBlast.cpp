// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilitys/AuraFireBlast.h"

FString UAuraFireBlast::GetDescription(int32 Level)
{
	return GetDescriptionAtLevel(Level,AbilityName);
}

FString UAuraFireBlast::GetNextLevelDescription(int32 Level)
{
	return GetDescriptionAtLevel(Level+1,TEXT("下一等级"));
}

FString UAuraFireBlast::GetDescriptionAtLevel(int32 Level, const FString& Title)
{
	return Super::GetDescriptionAtLevel(Level, Title);
}
