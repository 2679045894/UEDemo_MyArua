// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgeController/OverplayWidgetController.h"

#include "AbilitySystem/AuraAttributeSet.h"


void UOverplayWidgetController::BroadcastInitialValues()
{
	if (UAuraAttributeSet* AruaAttributeSet=Cast<UAuraAttributeSet>(AttributeSet))
	{
		OnHealthChanged.Broadcast(AruaAttributeSet->GetHealth());
		OnMaxHealthChanged.Broadcast(AruaAttributeSet->GetMaxHealth());
		OnManaChanged.Broadcast(AruaAttributeSet->GetMana());
		OnMaxManaChanged.Broadcast(AruaAttributeSet->GetMaxMana());
	}
}
// 绑定属性变化委托，建立属性变化时的回调机制。自动调用
//告诉 GAS："当这些属性发生变化时，请自动调用我指定的函数"。
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
}


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





