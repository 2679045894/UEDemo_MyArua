// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/AruaAttributeSet.h"
#include "AbilitySystemComponent.h"
#include "Net/UnrealNetwork.h"

UAruaAttributeSet::UAruaAttributeSet()
{
	
}

void UAruaAttributeSet::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	////函数中定义哪些属性需要在网络中进行同步复制。
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	//doRep_lifetime——notify
	//COND_None：无条件复制,
	//REPNOTIFY_Always：无论属性值是否实际发生变化，每次收到复制数据时都会调用 OnRep_Health 函数
	DOREPLIFETIME_CONDITION_NOTIFY(UAruaAttributeSet,Health,COND_None,REPNOTIFY_Always);

	DOREPLIFETIME_CONDITION_NOTIFY(UAruaAttributeSet,MaxHealth,COND_None,REPNOTIFY_Always);

	DOREPLIFETIME_CONDITION_NOTIFY(UAruaAttributeSet,Mana,COND_None,REPNOTIFY_Always);

	DOREPLIFETIME_CONDITION_NOTIFY(UAruaAttributeSet,MaxMana,COND_None,REPNOTIFY_Always);
}

//变量(Health)更新时调用这个函数
void UAruaAttributeSet::OnRep_Health(const FGameplayAttributeData& OldHeath) const
{
	//这个函数必须包含这个宏
	//作用：处理属性预测（Prediction）的回滚
	//	   管理属性修饰符的重新应用
	//	   确保GAS内部状态的一致性
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAruaAttributeSet,Health,OldHeath)
}

void UAruaAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAruaAttributeSet,MaxHealth,OldMaxHealth);
}

void UAruaAttributeSet::Rep_Mana(const FGameplayAttributeData& OldMana) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAruaAttributeSet,Mana,OldMana);
}

void UAruaAttributeSet::Rep_MaxMana(const FGameplayAttributeData& OldMana) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAruaAttributeSet,MaxMana,OldMana);
}

