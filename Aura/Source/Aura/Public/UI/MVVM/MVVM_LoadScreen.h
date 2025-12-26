// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MVVMViewModelBase.h"
#include "MVVM_LoadSlot.h"
#include "UObject/ObjectMacros.h"
#include "Game/MyGameModeBase.h"
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

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UMVVM_LoadSlot> LoadSlotViewModelClass;

	void InitializeLoadSlot();

	UFUNCTION(BlueprintPure)
	UMVVM_LoadSlot* GetLoadSlotViewModelByIndex(int32 Index)const;

	//创建新存档按下事件
	UFUNCTION(BlueprintCallable)
	void NewSlotButtonPressed(int32 Slot,const FString& EnterName);

	//开始游戏按下事件
	UFUNCTION(BlueprintCallable)
	void NewGameButtonPressed(int32 Slot);

	//选择存档按下事件
	UFUNCTION(BlueprintCallable)
	void SelectSlotButtonPressed(int32 Slot);

	UFUNCTION(BlueprintCallable)
	void EnterGameButtonPressed(const int32 Slot);

	void LoadData();

	UFUNCTION(BlueprintCallable)
	void DeleteButtonPressed(int32 Slot);
private:
	/**
	 * FieldNotify:这是 MVVM（Model-View-ViewModel）模式的关键特性
	 *			   当这个属性的值改变时，会自动发送通知，绑定到这个属性的 UI 元素会自动更新
	 * meta=(AllowPrivateAccess)
	 * 即使属性是私有的（在 private: 部分），Unreal 的反射系统和蓝图也能访问它
	 */
	UPROPERTY(BlueprintReadWrite,FieldNotify,Setter,Getter,meta=(AllowPrivateAccess))
	FString WidgetName;

	//索引和对应的MVVM实例的映射
	UPROPERTY()
	TMap<int32,UMVVM_LoadSlot*> LoadSlots;

	//对象对MVVM实例的引用，防止垃圾回收机制对其进行回收
	UPROPERTY()
	TObjectPtr<UMVVM_LoadSlot> LoadSlot_0;
	UPROPERTY()
	TObjectPtr<UMVVM_LoadSlot> LoadSlot_1;
	UPROPERTY()
	TObjectPtr<UMVVM_LoadSlot> LoadSlot_2;
	
};
