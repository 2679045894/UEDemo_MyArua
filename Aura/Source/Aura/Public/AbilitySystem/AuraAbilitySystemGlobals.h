// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemGlobals.h"
#include "AuraAbilitySystemGlobals.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UAuraAbilitySystemGlobals : public UAbilitySystemGlobals
{
	GENERATED_BODY()
	//当游戏需要创建 Gameplay Effect 时，GAS 内部会调用这个函数来获取上下文对象：
	virtual FGameplayEffectContext* AllocGameplayEffectContext() const override;
};
