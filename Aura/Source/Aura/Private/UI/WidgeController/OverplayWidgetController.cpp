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
			AruaAttributeSet->GetHealthAttribute()).AddUObject(this,&UOverplayWidgetController::HealthChanged);
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
			AruaAttributeSet->GetMaxHealthAttribute()).AddUObject(this,&UOverplayWidgetController::MaxHealthChanged);
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
			AruaAttributeSet->GetManaAttribute()).AddUObject(this,&UOverplayWidgetController::ManaChanged);
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
			AruaAttributeSet->GetMaxManaAttribute()).AddUObject(this,&UOverplayWidgetController::MaxManaChanged);
	}
	//获取委托，通过lambda表达式添加绑定函数
	Cast<UAuraAbilitySystemComponent>(AbilitySystemComponent)->EffectAssetTags.AddLambda(
		[](FGameplayTagContainer& TagContainer)
		{
			for (auto Tag :TagContainer)
			{
				GEngine->AddOnScreenDebugMessage(-1,5.0f,FColor::Red,
	FString::Printf(TEXT("%s"), *Tag.ToString()));
			}
		}
	);
}

//广播新值
void UOverplayWidgetController::HealthChanged(const FOnAttributeChangeData& Data) const
{
	OnHealthChanged.Broadcast(Data.NewValue);
}

void UOverplayWidgetController::MaxHealthChanged(const FOnAttributeChangeData& Data) const
{
	OnMaxHealthChanged.Broadcast(Data.NewValue);
}

void UOverplayWidgetController::ManaChanged(const FOnAttributeChangeData& Data) const
{
	OnManaChanged.Broadcast(Data.NewValue);
}

void UOverplayWidgetController::MaxManaChanged(const FOnAttributeChangeData& Data) const
{
	OnMaxManaChanged.Broadcast(Data.NewValue);
}





