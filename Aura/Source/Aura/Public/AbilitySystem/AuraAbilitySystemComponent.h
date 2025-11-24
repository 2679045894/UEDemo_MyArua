// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AuraAbilitySystemComponent.generated.h"

//广播标签
DECLARE_MULTICAST_DELEGATE_OneParam(FEffectAssetTags,FGameplayTagContainer&)
DECLARE_MULTICAST_DELEGATE_OneParam(FAbilityGiven,UAuraAbilitySystemComponent*)
DECLARE_DELEGATE_OneParam(FForEachAbility,const FGameplayAbilitySpec&);
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
	FEffectAssetTags EffectAssetTags;
protected:
	//当任何 Gameplay Effect 应用到拥有此 ASC 的 Actor 时触发的回调函数。
	UFUNCTION(Client,Reliable)
	void ClientEffectApplied(UAbilitySystemComponent* ASC,const FGameplayEffectSpec& EffectSpec,FActiveGameplayEffectHandle ActiveEffectHandle);
public:
	void AddCharacterAbilities(const TArray<TSubclassOf<UGameplayAbility>> &StartupAbilities);
	void AddCharacterPassiveAbilities(const TArray<TSubclassOf<UGameplayAbility>> &StartupPassiveAbilities);

	void AbilityInputTagHeld(const FGameplayTag& InputTag);
	void AbilityInputTagReleased(const FGameplayTag& InputTag);

	//通知所有监听者"技能初始化应用已完成"。
	FAbilityGiven AbilityGivenDelegate;
	bool bStartupAbilitiesGiven=false;

	void ForEachAbility(const FForEachAbility&Delegate);

	static FGameplayTag GetAbilityTagFormSpec(const FGameplayAbilitySpec& AbilitySpec);
	static FGameplayTag GetInputTagFormSpec(const FGameplayAbilitySpec& AbilitySpec);

	//客户端更新UI
	virtual void OnRep_ActivateAbilities() override;
};
