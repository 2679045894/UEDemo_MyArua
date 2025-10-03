// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgeController/AuraWidgetController.h"
#include "GameplayEffectTypes.h"
#include "OverplayWidgetController.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHealthChangedSignature, float, NewHealth);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMaxHealthChangeSignature,float,NewMaxHealth);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnManaChangeSignature,float,NewMana);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMaxManaChangedSignature,float,NewMaxMana);
/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class AURA_API UOverplayWidgetController : public UAuraWidgetController
{
	GENERATED_BODY()
public:
	//确保 UI 控件在创建时就能显示正确的初始属性值
	virtual void BroadcastInitialValues() override;
	//绑定属性变化委托，建立属性变化时的回调机制。
	virtual void BindCallbacksToDependencies() override;
	//多播委托定义  可在蓝图中bind
	UPROPERTY(BlueprintAssignable,Category="GAS|Attributes")
	FOnHealthChangedSignature OnHealthChanged;

	UPROPERTY(BlueprintAssignable,Category="GAS|Attributes")
	FOnMaxHealthChangeSignature OnMaxHealthChanged;

	UPROPERTY(BlueprintAssignable,Category="GAS|Attributes")
	FOnManaChangeSignature OnManaChanged;

	UPROPERTY(BlueprintAssignable,Category="GAS|Attributes")
	FOnMaxManaChangedSignature OnMaxManaChanged;

	
protected:
	//FOnAttributeChangeData 是一个 非 UObject 类型 的结构体，因此不能加宏
	void HealthChanged(const FOnAttributeChangeData& Data) const;
	void MaxHealthChanged(const FOnAttributeChangeData& Data) const;
	void ManaChanged(const FOnAttributeChangeData& Data) const;
	void MaxManaChanged(const FOnAttributeChangeData& Data) const;

};

