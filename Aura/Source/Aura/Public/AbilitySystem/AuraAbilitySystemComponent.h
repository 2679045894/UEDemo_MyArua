// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AuraAbilitySystemComponent.generated.h"

//广播标签
DECLARE_MULTICAST_DELEGATE_OneParam(FEffectAssetTags,FGameplayTagContainer&)
DECLARE_MULTICAST_DELEGATE(FAbilitiesGiven);
DECLARE_DELEGATE_OneParam(FForEachAbility,const FGameplayAbilitySpec&);
DECLARE_MULTICAST_DELEGATE(FAbilityGivenDelegate);
DECLARE_MULTICAST_DELEGATE_ThreeParams(FAbilityStatusChanged,const FGameplayTag&,const FGameplayTag&,int32/*Level*/)
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
	FAbilitiesGiven AbilitiesGivenDelegate;
	FAbilityStatusChanged AbilityStatusChangedDelegate;
	bool bStartupAbilitiesGiven=false;

	void ForEachAbility(const FForEachAbility&Delegate);

	static FGameplayTag GetAbilityTagFromSpec(const FGameplayAbilitySpec& AbilitySpec);
	static FGameplayTag GetInputTagFromSpec(const FGameplayAbilitySpec& AbilitySpec);
	static FGameplayTag GetStatusTagFromSpec(const FGameplayAbilitySpec& AbilitySpec);
	FGameplayAbilitySpec* GetSpecFromAbilityTag(const FGameplayTag& AbilityTag);

	//客户端更新UI
	virtual void OnRep_ActivateAbilities() override;

	//升级属性
	void UpgradeAttribute(const FGameplayTag& AttributeTag);
	//服务器升级属性
	UFUNCTION(Server,Reliable)
	void ServerUpgradeAttribute(const FGameplayTag& AttributeTag);

	UFUNCTION()
	void UpdateAbilityStatuses(int32 Level);

	//ASC是在服务器运行，我们再增加一个客户端执行的函数，用于广播到每个客户端
	UFUNCTION(Client,Reliable)
	void ClientUpdateAbilityStatus(const FGameplayTag& AbilityTag,const FGameplayTag& StatusTag);

	UFUNCTION(Server, Reliable)
	void ServerSpendSpellPoint(const FGameplayTag& AbilityTag);

	UFUNCTION(Server, Reliable)
	void ServerDemotionSpellPoint(const FGameplayTag& AbilityTag);
};
