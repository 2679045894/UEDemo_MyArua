// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "Game/LoadScreenSaveGame.h"
#include "AuraAbilitySystemComponent.generated.h"

//广播标签
DECLARE_MULTICAST_DELEGATE_OneParam(FEffectAssetTags,FGameplayTagContainer&)
DECLARE_MULTICAST_DELEGATE(FAbilitiesGiven);
DECLARE_DELEGATE_OneParam(FForEachAbility,const FGameplayAbilitySpec&);
DECLARE_MULTICAST_DELEGATE(FAbilityGivenDelegate);
DECLARE_MULTICAST_DELEGATE_ThreeParams(FAbilityStatusChanged,const FGameplayTag&,const FGameplayTag&,int32/*Level*/)
DECLARE_MULTICAST_DELEGATE_FourParams(FAbilityEquipped,const FGameplayTag&,const FGameplayTag&,const FGameplayTag&,const FGameplayTag&)
DECLARE_MULTICAST_DELEGATE_OneParam(FDeactivatePassiveAbility,const FGameplayTag&);
DECLARE_MULTICAST_DELEGATE_TwoParams(FActivePassiveEffect,const FGameplayTag&,const bool)
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
	void AbilityInputPressed(const FGameplayTag& InputTag);
	void AbilityInputTagHeld(const FGameplayTag& InputTag);
	void AbilityInputTagReleased(const FGameplayTag& InputTag);

	//通知所有监听者"技能初始化应用已完成"。
	FAbilitiesGiven AbilitiesGivenDelegate;
	FAbilityStatusChanged AbilityStatusChangedDelegate;
	bool bStartupAbilitiesGiven=false;

	//启动被动技能特效委托
	FActivePassiveEffect ActivePassiveEffectDelegate;
	
	FAbilityEquipped AbilityEquippedDelegate;

	void ForEachAbility(const FForEachAbility&Delegate);

	static FGameplayTag GetAbilityTagFromSpec(const FGameplayAbilitySpec& AbilitySpec);
	static FGameplayTag GetInputTagFromSpec(const FGameplayAbilitySpec& AbilitySpec);
	static FGameplayTag GetStatusTagFromSpec(const FGameplayAbilitySpec& AbilitySpec);
	FGameplayTag GetAbilityTypeTagFromSpec(const FGameplayAbilitySpec& AbilitySpec) const;
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

	bool GetDescriptionByAbilityTag(const FGameplayTag& AbilityTag,FString& OutDescription,FString& OutNextLevelDescription);

	//判断当前技能实例是否处于目标技能装备插槽
	static bool AbilityHasSlot(FGameplayAbilitySpec* Spec,const FGameplayTag& Slot);

	//清除技能的装配的插槽（清除GA的输入标签）
	void ClearSlot(FGameplayAbilitySpec* Spec);

	void ClearAbilitiesOfSlot(const FGameplayTag& Slot);

	UFUNCTION(Server, Reliable) //在服务器处理技能装配，传入技能标签和装配的技能标签
	void ServerEquipAbility(const FGameplayTag& AbilityTag,const FGameplayTag& SlotTag);

	UFUNCTION(Server, Reliable)
	void ClientEquipAbility(const FGameplayTag& AbilityTag,const FGameplayTag& Status,const FGameplayTag& Slot,const FGameplayTag& PreviousSlot);

	//取消被动效果
	FDeactivatePassiveAbility DeactivatePassiveAbilityDelegate;
	
	//目标装备插槽是否为空
	bool SlotIsEmpty(const FGameplayTag& SlotTag);
	//获取目标插槽现在装配的技能（Spec）
	FGameplayAbilitySpec* GetSpecWithSlot(const FGameplayTag& SlotTag);
	
	bool IsPassiveAbility(const FGameplayAbilitySpec& Spec) const;
	//技能是否被设置到装配插槽
	bool AbilityHasAnySlot(FGameplayAbilitySpec* Spec)const;

	void AssignSlotToAbility(FGameplayAbilitySpec& Spec,const FGameplayTag& SlotTag);

	UFUNCTION(NetMulticast,Unreliable)
	void MulticastActivatePassiveEffect(const FGameplayTag& AbilityTag,const bool bActive);

	UFUNCTION()
	void AddCharacterAbilitiesFromSaveDate(ULoadScreenSaveGame* SaveObject);
	
};
