// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/AuraAbilitySystemComponent.h"


void UAuraAbilitySystemComponent::AbilityActorInfoSet()
{
	//将委托和函数绑定()
	OnGameplayEffectAppliedDelegateToSelf.AddUObject(this,&UAuraAbilitySystemComponent::EffectApplied);
}

//回调函数
void UAuraAbilitySystemComponent::EffectApplied(UAbilitySystemComponent* ASC, const FGameplayEffectSpec& EffectSpec,
	FActiveGameplayEffectHandle ActiveEffectHandle)
{
	//标签容器，用于存储某个效果/对象的所有附加的标签
	FGameplayTagContainer TagContainer;
	EffectSpec.GetAllAssetTags(TagContainer);
	EffectAssetTags.Broadcast(TagContainer);
	/*for (auto Tag: TagContainer)
	{
		GEngine->AddOnScreenDebugMessage(-1,5.0f,FColor::Red,
			FString::Printf(TEXT("%s"), *Tag.ToString()));
	}*/
}