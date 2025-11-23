// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Data/AbilityInfo.h"

FAuraAbilityInfo UAbilityInfo::FindAbilityInfoForTag(const FGameplayTag& AbilityTag,bool bLogNotFound) const
{
	for (FAuraAbilityInfo Ability : AbilityInformation)
	{
		if (Ability.AbilityTag.MatchesTagExact(AbilityTag))
		{
			return Ability;
		}
	}
	if (bLogNotFound)
	{
		UE_LOG(LogTemp,Warning,TEXT("无法通过技能标签[%s]在技能数据[%s]查找到对应的技能数据"),*AbilityTag.ToString(),*GetNameSafe(this));
	}
	return FAuraAbilityInfo();
}
