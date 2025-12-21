// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilitys/AuraArcaneShards.h"

#include "AuraGameplayTags.h"

FString UAuraArcaneShards::GetNextLevelDescription(int32 Level)
{
	return GetDescriptionAtLevel(Level+1,L"下一等级");
}

FString UAuraArcaneShards::GetDescription(int32 Level)
{
	return GetDescriptionAtLevel(Level,AbilityName);
}

FString UAuraArcaneShards::GetDescriptionAtLevel(int32 Level, const FString& Title)
{
	float Cooldown = GetCooldown(Level);
	float ManaCost=FMath::Abs(GetManaCost(Level));
	int32 Damage=GetDamageByDamageType(Level,FAuraGameplayTags::Get().Damage_Arcane);
	return FString::Printf(TEXT(
		"<Title>%s</>\n"
		"等级:<Level>%i</>\n"
		"技能冷却<Cooldown>%.1f</>\n"
		"蓝量消耗<ManaCost>%.1f</>\n"
		"<Default>首次激活技能使用魔法光圈划定攻击位置，再次释放在魔法光圈位置生成</><NumProjectiles>%i</><Default>奥术碎片，攻击附近敌人，造成</><Damage>%i</><Default>点奥术伤害</>"),
		*Title,
		Level,
		Cooldown,
		ManaCost,
		FMath::Min(Level,NumProjectiles),
		Damage);
}