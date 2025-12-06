// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilitys/AuraFireBolt.h"

#include "AuraGameplayTags.h"
#include "AbilitySystem/AuraAttributeSet.h"

float UAuraFireBolt::GetManaCost(float InLevel) const
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

float UAuraFireBolt::GetCooldown(float InLevel) const
{
	float Cooldown = 0.f;
	if (UGameplayEffect* CooldownEffect=GetCostGameplayEffect())
	{
		CooldownEffect->DurationMagnitude.GetStaticMagnitudeIfPossible(InLevel,Cooldown);
	}
	return Cooldown;
}

FString UAuraFireBolt::GetDescription(int32 Level)
{
	return GetDescriptionAtLevel(Level,L"火球术");
}

FString UAuraFireBolt::GetNextLevelDescription(int32 Level)
{
	return GetDescriptionAtLevel(Level+1,L"下一等级");
}

FString UAuraFireBolt::GetDescriptionAtLevel(int32 Level, const FString& Title)
{
	float Cooldown = GetCooldown(Level);
	float ManaCost=GetManaCost(Level);
	int32 Damage=GetDamageByDamageType(Level,FAuraGameplayTags::Get().Damage_Fire);

	return FString::Printf(TEXT(
		"<Title>%s</>\n"
		"等级:<Level>%i</>\n"
		"技能冷却<Cooldown>%.1f</>\n"
		"蓝量消耗<ManaCost>%.1f</>\n"

		"<Default>发射 %i 颗火球，在发生撞击时产生爆炸，并造成</> <Damage>%i</> <Default>点火焰伤害，并有一定几率燃烧。</>"),
		*Title,
		Level,
		Cooldown,
		ManaCost,
		FMath::Min(Level,NumProjectiles),
		Damage
		);
}
