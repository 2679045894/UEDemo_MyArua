// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AuraAbilitySystemComponent.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UAuraAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()
public:
	//初始化并设置 Ability System 的 Actor 信息，并绑定关键的委托。
	void AbilityActorInfoSet();
protected:
	//当任何 Gameplay Effect 应用到拥有此 ASC 的 Actor 时触发的回调函数。
	void EffectApplied(UAbilitySystemComponent* ASC,const FGameplayEffectSpec& EffectSpec,FActiveGameplayEffectHandle ActiveEffectHandle);
};
