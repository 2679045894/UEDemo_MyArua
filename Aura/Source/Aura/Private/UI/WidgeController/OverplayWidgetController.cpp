// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgeController/OverplayWidgetController.h"

#include "AuraGameplayTags.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "UI/WidgeController/AuraWidgetController.h"
#include "Player/MyPlayerState.h"


void UOverplayWidgetController::BroadcastInitialValues()
{
	OnHealthChanged.Broadcast(GetAuraAttributeSet()->GetHealth());
	OnMaxHealthChanged.Broadcast(GetAuraAttributeSet()->GetMaxHealth());
	OnManaChanged.Broadcast(GetAuraAttributeSet()->GetMana());
	OnMaxManaChanged.Broadcast(GetAuraAttributeSet()->GetMaxMana());
}

// 绑定属性变化委托，建立属性变化时的回调机制。自动调用
//告诉 GAS："当这些属性发生变化时，请自动调用我指定的函数"。
//初始化时调用一次(建立绑定关系)
void UOverplayWidgetController::BindCallbacksToDependencies()
{
	//AruaAttributeSet->GetHealthAttribute()为什么不直接gethealth
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		GetAuraAttributeSet()->GetHealthAttribute()).AddLambda([this](const FOnAttributeChangeData &Data)
		{
			OnHealthChanged.Broadcast(Data.NewValue);
		});
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		GetAuraAttributeSet()->GetMaxHealthAttribute()).AddLambda([this](const FOnAttributeChangeData &Data)
		{
			OnMaxHealthChanged.Broadcast(Data.NewValue);
		});
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		GetAuraAttributeSet()->GetManaAttribute()).AddLambda([this](const FOnAttributeChangeData &Data)
		{
			OnManaChanged.Broadcast(Data.NewValue);
		});
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		GetAuraAttributeSet()->GetMaxManaAttribute()).AddLambda([this](const FOnAttributeChangeData &Data)
		{
			OnMaxManaChanged.Broadcast(Data.NewValue);
		});
	if (GetAuraAbilitySystemComponent())
	{
		if (GetAuraAbilitySystemComponent()->bStartupAbilitiesGiven)
		{
			//如果执行到此处时，技能的初始化工作已经完成，则直接调用初始化回调
			BroadcastAbilityInfo();
		}
		else
		{
			//如果执行到此处，技能初始化还未完成，将通过绑定委托，监听广播的形式触发初始化完成回调
			GetAuraAbilitySystemComponent()->AbilitiesGivenDelegate.AddUObject(this,&UOverplayWidgetController::BroadcastAbilityInfo);
		}
	}
	//获取委托，通过lambda表达式添加绑定函数
	GetAuraAbilitySystemComponent()->EffectAssetTags.AddLambda(
		[this/*!!!!*/](FGameplayTagContainer& TagContainer)
		{
			for (auto Tag :TagContainer)
			{
				//这相当于一个过滤器，过滤出Message的子标签，因为hi有这些标签才有对应的行命名信息，如果不是Message下的标签，则会报空
				FGameplayTag MessageTag=FGameplayTag::RequestGameplayTag(FName("Message"));
				if (Tag.MatchesTag(MessageTag))
				{	//模板函数
					FUIWidgetRow* Row=GetDataTableRowByTag<FUIWidgetRow>(MessageWidgetDataTable,Tag);
					MessageWidgetRowDelegate.Broadcast(*Row);
				}
			}
		}
	);

	GetAuraAbilitySystemComponent()->AbilityEquippedDelegate.AddUObject(this,&UOverplayWidgetController::OnAbilityEquipped);
	GetAuraPlayerState()->OnXPChangedDelegate.AddUObject(this,&UOverplayWidgetController::OnXPChanged);
	GetAuraPlayerState()->OnLevelChangedDelegate.AddLambda([this](int32 NewLevel)
	{
		OnPlayerStateChangedDelegate.Broadcast(NewLevel);
	});
}


void UOverplayWidgetController::OnXPChanged(int32 NewXP)
{
	ULevelUpInfo* LevelUpInfo=GetAuraPlayerState()->LevelUpInfo;
	check(LevelUpInfo);
	int32 Level=LevelUpInfo->FindLevelForXP(NewXP);
	int32 MaxLevel=LevelUpInfo->LevelUpInformation.Num();
	if (Level<=MaxLevel&&Level>0)
	{
		const int32 LevelUpRequirement=LevelUpInfo->LevelUpInformation[Level].LevelUpRequirement;
		const int32 PreviousLevelUpRequirement=LevelUpInfo->LevelUpInformation[Level-1].LevelUpRequirement;
		const float XPPercent=static_cast<float>(NewXP-PreviousLevelUpRequirement)/(LevelUpRequirement-PreviousLevelUpRequirement);
		OnXPPercentChangedDelegate.Broadcast(XPPercent);
	}
}

void UOverplayWidgetController::OnAbilityEquipped(const FGameplayTag& AbilityTag,const FGameplayTag& Status,
	 const FGameplayTag& Slot,const FGameplayTag& PreSlot)
{
	if (bWaitingForEquipSelection&&Count==2)
	{
		FAuraGameplayTags GameplayTags=FAuraGameplayTags::Get();
		FAuraAbilityInfo LastSlotInfo;
		LastSlotInfo.StatusTag=GameplayTags.Abilities_Status_Unlocked;
		LastSlotInfo.InputTag=PreSlot;
		LastSlotInfo.AbilityTag=GameplayTags.Abilities_None;
		AbilityInfoDelegate.Broadcast(LastSlotInfo);
		Count=0;
	}

	FAuraAbilityInfo Info=AbilityInfo->FindAbilityInfoForTag(AbilityTag);
	Info.InputTag=Slot;
	Info.StatusTag=Status;
	AbilityInfoDelegate.Broadcast(Info);
}











