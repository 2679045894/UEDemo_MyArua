// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilitys/AuraDamageGameplayAbility.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystem/AuraAbilitySystemLibrary.h"

void UAuraDamageGameplayAbility::CauseDamage(AActor* TargetActor)
{
	//创建伤害效果规格
	FGameplayEffectSpecHandle SpecHandle=MakeOutgoingGameplayEffectSpec(DamageEffectClass,1.f);
	//遍历该技能的伤害类型，根据等级配置对应类型伤害
	for (auto Pair:DamageTypes)
	{
		const float ScaledDamage=Pair.Value.GetValueAtLevel(GetAbilityLevel());
		UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle,Pair.Key,ScaledDamage);
	}
	//将效果应用到角色上
	GetAbilitySystemComponentFromActorInfo()->ApplyGameplayEffectSpecToTarget(*SpecHandle.Data.Get(),
		UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor));
}

FTaggedMontage UAuraDamageGameplayAbility::GetRandomTaggedMontageFromArray(
	const TArray<FTaggedMontage>& TaggedMontages)
{
	if (TaggedMontages.Num()>0)
	{
		int32 RandomIndex=FMath::RandRange(0, TaggedMontages.Num()-1);
		return TaggedMontages[RandomIndex];
	}
	return FTaggedMontage();
}

float UAuraDamageGameplayAbility::GetDamageByDamageType(float InLevel, const FGameplayTag& DamageType)
{
	return DamageTypes[DamageType].GetValueAtLevel(InLevel);
}

FDamageEffectParams UAuraDamageGameplayAbility::MakeDamageEffectParamsFromClassDefault(AActor* TargetActor)
{
	FDamageEffectParams DamageEffectParams;
	DamageEffectParams.WorldContextObject=GetAvatarActorFromActorInfo();
	DamageEffectParams.DamageGameplayEffectClass=DamageEffectClass;
	DamageEffectParams.SourceASC=GetAbilitySystemComponentFromActorInfo();
	DamageEffectParams.TargetASC=UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
	for (auto& Pair:DamageTypes)
	{
		float ScaledDamage=Pair.Value.GetValueAtLevel(GetAbilityLevel());
		DamageEffectParams.DamageTypes.Add(Pair.Key,ScaledDamage);
	}
	DamageEffectParams.AbilityLevel=GetAbilityLevel();
	DamageEffectParams.DeBuffChance=DeBuffChance;
	DamageEffectParams.DeBuffDamage=DeBuffDamage;
	DamageEffectParams.DeBuffDuration=DeBuffDuration;
	DamageEffectParams.DeBuffFrequency=DeBuffFrequency;
	DamageEffectParams.DeBuffDamageType=DebuffDamageType;
	return DamageEffectParams;
}


