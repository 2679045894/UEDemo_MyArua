// Fill out your copyright notice in the Description page of Project Settings.


#include "AuraGameplayTags.h"
#include "GameplayTags.h"

//静态成员变量的定义（普通成员无需定义）
FAuraGameplayTags FAuraGameplayTags::GameplayTags;

void FAuraGameplayTags::InitializeNativeGameplayTags()
{
	//向UE的标签管理器注册原生标签
	GameplayTags.Attributes_Primary_Strength=UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Primary.Strength"));
	GameplayTags.Attributes_Primary_Intelligence=UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Primary.Intelligence"));
	GameplayTags.Attributes_Primary_Resilience=UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Primary.Resilience"));
	GameplayTags.Attributes_Primary_Vigor=UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Primary.Vigor"));
	
	GameplayTags.Attributes_Secondary_Armor=UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Secondary.Armor"));
	GameplayTags.Attributes_Secondary_ArmorPenetration=UGameplayTagsManager::Get().AddNativeGameplayTag("Attributes.Secondary.ArmorPenetration");
	GameplayTags.Attributes_Secondary_BlockChance=UGameplayTagsManager::Get().AddNativeGameplayTag("Attributes.Secondary.BlockChance");
	GameplayTags.Attributes_Secondary_CriticalHitChancel=UGameplayTagsManager::Get().AddNativeGameplayTag("Attributes.Secondary.CriticalHitChancel");
	GameplayTags.Attributes_Secondary_CriticalHitDamage=UGameplayTagsManager::Get().AddNativeGameplayTag("Attributes.Secondary.CriticalHitDamage");
	GameplayTags.Attributes_Secondary_CriticalHitResistance=UGameplayTagsManager::Get().AddNativeGameplayTag("Attributes.Secondary.CriticalHitResistance");
	GameplayTags.Attributes_Secondary_HealthRegeneration=UGameplayTagsManager::Get().AddNativeGameplayTag("Attributes.Secondary.HealthRegeneration");
	GameplayTags.Attributes_Secondary_ManaRegeneration=UGameplayTagsManager::Get().AddNativeGameplayTag("Attributes.Secondary.ManaRegeneration");
	GameplayTags.Attributes_Secondary_MaxHealth=UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Secondary.MaxHealth"));
	GameplayTags.Attributes_Secondary_MaxMana=UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Secondary.MaxMana"));

	GameplayTags.InputTag_LMB=UGameplayTagsManager::Get().AddNativeGameplayTag(FName("InputTag.LMB"));
	GameplayTags.InputTag_RMB=UGameplayTagsManager::Get().AddNativeGameplayTag(FName("InputTag.RMB"));
	GameplayTags.InputTag_1=UGameplayTagsManager::Get().AddNativeGameplayTag(FName("InputTag.1"));
	GameplayTags.InputTag_2=UGameplayTagsManager::Get().AddNativeGameplayTag(FName("InputTag.2"));
	GameplayTags.InputTag_3=UGameplayTagsManager::Get().AddNativeGameplayTag(FName("InputTag.3"));
	GameplayTags.InputTag_4=UGameplayTagsManager::Get().AddNativeGameplayTag(FName("InputTag.4"));
	GameplayTags.InputTag_Passive_1=UGameplayTagsManager::Get().AddNativeGameplayTag(FName("InputTag.Passive.1"));
	GameplayTags.InputTag_Passive_2=UGameplayTagsManager::Get().AddNativeGameplayTag(FName("InputTag.Passive.2"));

	GameplayTags.Damage=UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Damage"));
	GameplayTags.Damage_Fire=UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Damage.Fire"));
	GameplayTags.Damage_Lightning=UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Damage.Lightning"));
	GameplayTags.Damage_Arcane=UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Damage.Arcane"));
	GameplayTags.Damage_Physical=UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Damage.Physical"));

	GameplayTags.Attributes_Resistance_Fire=UGameplayTagsManager::Get().AddNativeGameplayTag("Attributes.Resistance.Fire");
	GameplayTags.Attributes_Resistance_Lightning=UGameplayTagsManager::Get().AddNativeGameplayTag("Attributes.Resistance.Lightning");
	GameplayTags.Attributes_Resistance_Arcane=UGameplayTagsManager::Get().AddNativeGameplayTag("Attributes.Resistance.Arcane");
	GameplayTags.Attributes_Resistance_Physical=UGameplayTagsManager::Get().AddNativeGameplayTag("Attributes.Resistance.Physical");

	GameplayTags.Attributes_Meta_IncomingXP=UGameplayTagsManager::Get().AddNativeGameplayTag("Attributes.Meta.IncomingXP");
	
	GameplayTags.DamageTypesToResistance.Add(GameplayTags.Damage_Fire,GameplayTags.GameplayTags.Attributes_Resistance_Fire);
	GameplayTags.DamageTypesToResistance.Add(GameplayTags.Damage_Lightning,GameplayTags.Attributes_Resistance_Lightning);
	GameplayTags.DamageTypesToResistance.Add(GameplayTags.Damage_Arcane,GameplayTags.Attributes_Resistance_Arcane);
	GameplayTags.DamageTypesToResistance.Add(GameplayTags.Damage_Physical,GameplayTags.Attributes_Resistance_Physical);

	GameplayTags.Effect_HitReact=UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Effect.HitReact"));

	GameplayTags.Abilities_None=UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.None"));
	GameplayTags.Abilities_Attack=UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Attack"));
	GameplayTags.Abilities_Summon=UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Summon"));

	GameplayTags.CombatSocket_LeftHand=UGameplayTagsManager::Get().AddNativeGameplayTag(FName("CombatSocket.LeftHand"));
	GameplayTags.CombatSocket_RightHand=UGameplayTagsManager::Get().AddNativeGameplayTag(FName("CombatSocket.RightHand"));
	GameplayTags.CombatSocket_Weapon=UGameplayTagsManager::Get().AddNativeGameplayTag(FName("CombatSocket.Weapon"));
	GameplayTags.CombatSocket_Tail=UGameplayTagsManager::Get().AddNativeGameplayTag(FName("CombatSocket.Tail"));

	GameplayTags.Montage_Attack_1=UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Montage.Attack.1"));
	GameplayTags.Montage_Attack_2=UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Montage.Attack.2"));
	GameplayTags.Montage_Attack_3=UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Montage.Attack.3"));
	GameplayTags.Montage_Attack_4=UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Montage.Attack.4"));

	GameplayTags.Abilities_Fire_FireBolt=UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Fire.FireBolt"));
	GameplayTags.Abilities_Lightning_Electrocute=UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Lightning.Electrocute"));
	
	GameplayTags.Cooldown_Fire_FireBolt=UGameplayTagsManager::Get().AddNativeGameplayTag("Cooldown.Fire.FireBolt");

	GameplayTags.Abilities_HitReact=UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.HitReact"));
	GameplayTags.Abilities_Status_Locked=UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Status.Locked"));
	GameplayTags.Abilities_Status_Eligible=UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Status.Eligible"));
	GameplayTags.Abilities_Status_Unlocked=UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Status.Unlocked"));
	GameplayTags.Abilities_Status_Equipped=UGameplayTagsManager::Get().AddNativeGameplayTag("Abilities.Status.Equipped");

	GameplayTags.Abilities_Type_Offensive=UGameplayTagsManager::Get().AddNativeGameplayTag("Abilities.Type.Offensive");
	GameplayTags.Abilities_Type_Passive=UGameplayTagsManager::Get().AddNativeGameplayTag("Abilities.Type.Passive");
	GameplayTags.Abilities_Type_None=UGameplayTagsManager::Get().AddNativeGameplayTag("Abilities.Type.None");

	GameplayTags.DeBuff_Burn=UGameplayTagsManager::Get().AddNativeGameplayTag("DeBuff.Burn");
	GameplayTags.DeBuff_Stun=UGameplayTagsManager::Get().AddNativeGameplayTag("DeBuff.Stun");
	GameplayTags.DeBuff_Arcane=UGameplayTagsManager::Get().AddNativeGameplayTag("DeBuff.Arcane");
	GameplayTags.DeBuff_Physical=UGameplayTagsManager::Get().AddNativeGameplayTag("DeBuff.Physical");
	GameplayTags.DeBuffToResistance.Add(GameplayTags.DeBuff_Burn,GameplayTags.Attributes_Resistance_Fire);
	GameplayTags.DeBuffToResistance.Add(GameplayTags.DeBuff_Stun,GameplayTags.Attributes_Resistance_Lightning);
	GameplayTags.DeBuffToResistance.Add(GameplayTags.DeBuff_Arcane,GameplayTags.Attributes_Resistance_Arcane);
	GameplayTags.DeBuffToResistance.Add(GameplayTags.DeBuff_Physical,GameplayTags.Attributes_Resistance_Physical);

	GameplayTags.DeBuff_Chance=UGameplayTagsManager::Get().AddNativeGameplayTag("DeBuff.Chance");
	GameplayTags.DeBuff_Damage=UGameplayTagsManager::Get().AddNativeGameplayTag("DeBuff.Damage");
	GameplayTags.DeBuff_Duration=UGameplayTagsManager::Get().AddNativeGameplayTag("DeBuff.Duration");
	GameplayTags.DeBuff_Frequency=UGameplayTagsManager::Get().AddNativeGameplayTag("DeBuff.Frequency");

	GameplayTags.Player_Block_InputPressed=UGameplayTagsManager::Get().AddNativeGameplayTag("Player.Block.InputPressed");
	GameplayTags.Player_Block_InputHold=UGameplayTagsManager::Get().AddNativeGameplayTag("Player.Block.InputHold");
	GameplayTags.Player_Block_InputReleased=UGameplayTagsManager::Get().AddNativeGameplayTag("Player.Block.InputReleased");
	GameplayTags.Player_Block_CursorTrace=UGameplayTagsManager::Get().AddNativeGameplayTag("Player.Block.CursorTrace");
	
}



