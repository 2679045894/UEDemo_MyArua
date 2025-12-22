// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MVVMViewModelBase.h"
#include "MVVM_LoadScreen.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UMVVM_LoadScreen : public UMVVMViewModelBase
{
	GENERATED_BODY()
public:
	void SetWidgetName(const FString& InSlotName);
	FString GetWidgetName()const {return WidgetName;}
private:
	/**
	 * FieldNotify:这是 MVVM（Model-View-ViewModel）模式的关键特性
	 *			   当这个属性的值改变时，会自动发送通知，绑定到这个属性的 UI 元素会自动更新
	 * meta=(AllowPrivateAccess)
	 * 即使属性是私有的（在 private: 部分），Unreal 的反射系统和蓝图也能访问它
	 */
	UPROPERTY(BlueprintReadOnly,FieldNotify,Setter,Getter,meta=(AllowPrivateAccess))
	FString WidgetName;
};
