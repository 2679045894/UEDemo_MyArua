// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilitys/AuraGameplayAbility.h"

#include "AbilitySystem/AuraAttributeSet.h"
#include "WorldPartition/Cook/WorldPartitionCookPackage.h"
float UAuraGameplayAbility::GetManaCost(float InLevel) const
{
	float ManaCost = 0.f;
	if (UGameplayEffect* CostEffect=GetCostGameplayEffect())
	{
		for (FGameplayModifierInfo Info:CostEffect->Modifiers)
		{
			if (Info.Attribute==UAuraAttributeSet::GetManaAttribute())
			{
				Info.ModifierMagnitude.GetStaticMagnitudeIfPossible(InLevel,ManaCost);
				break;
			}
		}
	}
	return ManaCost;
}

float UAuraGameplayAbility::GetCooldown(float InLevel) const
{
	float Cooldown = 0.f;
	if (UGameplayEffect* CooldownEffect=GetCooldownGameplayEffect())
	{
		CooldownEffect->DurationMagnitude.GetStaticMagnitudeIfPossible(InLevel,Cooldown);
	}
	return Cooldown;
}

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

FString UAuraGameplayAbility::GetDescriptionAtLevel(int32 Level, const FString& Title)
{
	return FString::Printf(TEXT("未配置"));
}

FString UAuraGameplayAbility::GetLockedDescription(int32 Level)
{
	return FString::Printf(TEXT("当等级达到<Level> %i </>即可解锁"), Level);
}
