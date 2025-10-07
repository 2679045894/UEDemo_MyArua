// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgeController/OverplayWidgetController.h"

#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAttributeSet.h"


void UOverplayWidgetController::BroadcastInitialValues()
{
	if (UAuraAttributeSet* AruaAttributeSet=Cast<UAuraAttributeSet>(AttributeSet))
	{
		//传入的值进行广播，在蓝图中调用该委托的时候会提供一个参数(GetHealth)
		OnHealthChanged.Broadcast(AruaAttributeSet->GetHealth());
		OnMaxHealthChanged.Broadcast(AruaAttributeSet->GetMaxHealth());
		OnManaChanged.Broadcast(AruaAttributeSet->GetMana());
		OnMaxManaChanged.Broadcast(AruaAttributeSet->GetMaxMana());
	}
}
// 绑定属性变化委托，建立属性变化时的回调机制。自动调用
//告诉 GAS："当这些属性发生变化时，请自动调用我指定的函数"。
//初始化时调用一次(建立绑定关系)
void UOverplayWidgetController::BindCallbacksToDependencies()
{
	if (UAuraAttributeSet* AruaAttributeSet=Cast<UAuraAttributeSet>(AttributeSet))
	{
		//AruaAttributeSet->GetHealthAttribute()为什么不直接gethealth
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
			AruaAttributeSet->GetHealthAttribute()).AddLambda([this](const FOnAttributeChangeData &Data)
			{
				OnHealthChanged.Broadcast(Data.NewValue);
			});
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
			AruaAttributeSet->GetMaxHealthAttribute()).AddLambda([this](const FOnAttributeChangeData &Data)
			{
				OnMaxHealthChanged.Broadcast(Data.NewValue);
			});
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
			AruaAttributeSet->GetManaAttribute()).AddLambda([this](const FOnAttributeChangeData &Data)
			{
				OnManaChanged.Broadcast(Data.NewValue);
			});
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
			AruaAttributeSet->GetMaxManaAttribute()).AddLambda([this](const FOnAttributeChangeData &Data)
			{
				OnHealthChanged.Broadcast(Data.NewValue);
			});
	}
	//获取委托，通过lambda表达式添加绑定函数
	Cast<UAuraAbilitySystemComponent>(AbilitySystemComponent)->EffectAssetTags.AddLambda(
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
}






