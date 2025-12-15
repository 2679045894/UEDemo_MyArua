// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"

struct FAuraGameplayTags
{
public:
	
	//单例访问方法，返回全局唯一实例的引用
	static const FAuraGameplayTags& Get(){return GameplayTags;}
	//静态初始化方法
	static void InitializeNativeGameplayTags();

	//单个标签
	FGameplayTag Attributes_Primary_Strength;
	FGameplayTag Attributes_Primary_Intelligence;
	FGameplayTag Attributes_Primary_Resilience;
	FGameplayTag Attributes_Primary_Vigor;
	
	FGameplayTag Attributes_Secondary_Armor;
	FGameplayTag Attributes_Secondary_ArmorPenetration;
	FGameplayTag Attributes_Secondary_BlockChance;
	FGameplayTag Attributes_Secondary_CriticalHitChancel;
	FGameplayTag Attributes_Secondary_CriticalHitDamage;
	FGameplayTag Attributes_Secondary_CriticalHitResistance;
	FGameplayTag Attributes_Secondary_HealthRegeneration;
	FGameplayTag Attributes_Secondary_ManaRegeneration;
	FGameplayTag Attributes_Secondary_MaxHealth;
	FGameplayTag Attributes_Secondary_MaxMana;

	FGameplayTag Attributes_Resistance_Fire;
	FGameplayTag Attributes_Resistance_Lightning;
	FGameplayTag Attributes_Resistance_Arcane;
	FGameplayTag Attributes_Resistance_Physical;

	FGameplayTag Attributes_Meta_IncomingXP;

	FGameplayTag InputTag_LMB;
	FGameplayTag InputTag_RMB;
	FGameplayTag InputTag_1;
	FGameplayTag InputTag_2;
	FGameplayTag InputTag_3;
	FGameplayTag InputTag_4;
	FGameplayTag InputTag_Passive_1;
	FGameplayTag InputTag_Passive_2;

	FGameplayTag Damage;
	FGameplayTag Damage_Fire;
	FGameplayTag Damage_Lightning;
	FGameplayTag Damage_Arcane;
	FGameplayTag Damage_Physical;

	//伤害类型与对应伤害抗性
	TMap<FGameplayTag,FGameplayTag> DamageTypesToResistance;
	
	FGameplayTag Effect_HitReact;

	FGameplayTag Abilities_None;
	FGameplayTag Abilities_Attack;
	FGameplayTag Abilities_Summon;

	FGameplayTag CombatSocket_Weapon;
	FGameplayTag CombatSocket_RightHand;
	FGameplayTag CombatSocket_LeftHand;
	FGameplayTag CombatSocket_Tail;

	FGameplayTag Montage_Attack_1;
	FGameplayTag Montage_Attack_2;
	FGameplayTag Montage_Attack_3;
	FGameplayTag Montage_Attack_4;

	FGameplayTag Abilities_Fire_FireBolt;
	FGameplayTag Abilities_Lightning_Electrocute;

	FGameplayTag Cooldown_Fire_FireBolt;

	FGameplayTag Abilities_HitReact;
	FGameplayTag Abilities_Status_Locked;
	FGameplayTag Abilities_Status_Eligible;//可解锁
	FGameplayTag Abilities_Status_Unlocked;//已解锁
	FGameplayTag Abilities_Status_Equipped;

	FGameplayTag Abilities_Type_Offensive;
	FGameplayTag Abilities_Type_Passive;
	FGameplayTag Abilities_Type_None;
	
	FGameplayTag DeBuff_Burn;
	FGameplayTag DeBuff_Stun;
	FGameplayTag DeBuff_Arcane;
	FGameplayTag DeBuff_Physical;
	TMap<FGameplayTag,FGameplayTag> DeBuffToResistance;

	FGameplayTag DeBuff_Chance;
	FGameplayTag DeBuff_Damage;
	FGameplayTag DeBuff_Duration;
	FGameplayTag DeBuff_Frequency;

	FGameplayTag Player_Block_InputPressed;//阻挡键位按下输入
	FGameplayTag Player_Block_InputHold;//阻挡键位悬停输入
	FGameplayTag Player_Block_InputReleased;//阻挡键位抬起输入
	FGameplayTag Player_Block_CursorTrace;//阻挡鼠标拾取事件

	//被动技能
	FGameplayTag Abilities_Passive_HaloOfProtection;
	FGameplayTag Abilities_Passive_LifeSiphon;
	FGameplayTag Abilities_Passive_ManaSiphon;
	
private:
	//静态单例实例，整个程序生命周期内存在
	//全局唯一标签容器
	static FAuraGameplayTags GameplayTags;

protected:
};
