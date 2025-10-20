// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/AuraAbilitySystemComponent.h"


#include "AuraGameplayTags.h"
#include "AbilitySystem/Abilitys/AuraGameplayAbility.h"


void UAuraAbilitySystemComponent::AbilityActorInfoSet()
{
	//将委托和函数绑定()
	OnGameplayEffectAppliedDelegateToSelf.AddUObject(this,&UAuraAbilitySystemComponent::ClientEffectApplied);
}

//回调函数
void UAuraAbilitySystemComponent::ClientEffectApplied_Implementation(UAbilitySystemComponent* ASC, const FGameplayEffectSpec& EffectSpec,
	FActiveGameplayEffectHandle ActiveEffectHandle)
{
	//标签容器，用于存储某个效果/对象的所有附加的标签
	FGameplayTagContainer TagContainer;
	EffectSpec.GetAllAssetTags(TagContainer);
	EffectAssetTags.Broadcast(TagContainer);
}

//赋予所有在StartupAbilities中的技能
void UAuraAbilitySystemComponent::AddCharacterAbilities(const TArray<TSubclassOf<UGameplayAbility>>& StartupAbilities)
{
	for (TSubclassOf<UGameplayAbility> AbilityClass : StartupAbilities)
	{
		//1.创建技能规格
		FGameplayAbilitySpec AbilitySpec=FGameplayAbilitySpec(AbilityClass,1);
		//将技能规格中的Ability进行转换
		if (UAuraGameplayAbility* AuraAbility=Cast<UAuraGameplayAbility>(AbilitySpec.Ability))
		{
			//2.绑定输入标签(向技能的动态标签集合中添加一个标签)（通过某个按键标签(StartupInputTag)触发技能）
			AbilitySpec.DynamicAbilityTags.AddTag(AuraAbility->StartupInputTag);
			//3.赋予技能
			GiveAbility(AbilitySpec);
		}
	}
}

void UAuraAbilitySystemComponent::AbilityInputTagHeld(const FGameplayTag &InputTag)
{
	if (!InputTag.IsValid())return;
	//循环遍历所有可激活的技能
	for (FGameplayAbilitySpec& AbilitySpec:GetActivatableAbilities())
	{
		//检查输入标签匹配
		if (AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag))
		{
			//标记输入按下状态 - 通知技能系统该技能对应的输入已被按下
			AbilitySpecInputPressed(AbilitySpec);
			//检查技能是否已激活 - 判断当前技能是否处于未激活状态
			if (!AbilitySpec.IsActive())
			{
				//如果技能未激活，则尝试激活该技能。
				TryActivateAbility(AbilitySpec.Handle);
			}
		}
	}
}

void UAuraAbilitySystemComponent::AbilityInputTagReleased(const FGameplayTag &InputTag)
{
	if (!InputTag.IsValid())return;

	for (FGameplayAbilitySpec& AbilitySpec:GetActivatableAbilities())
	{
		if (AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag))
		{
			//通知技能系统该技能对应的输入已被释放
			AbilitySpecInputReleased(AbilitySpec);
		}
	}
}
