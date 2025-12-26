// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/MVVM/MVVM_LoadSlot.h"

void UMVVM_LoadSlot::InitializeSlot() const
{
	//从枚举获取到对应的索引
	const int32 WidgetSwitcherIndex=LoadSlotStatus.GetValue();
	SetWidgetSwitcherIndex.Broadcast(WidgetSwitcherIndex);
}

void UMVVM_LoadSlot::SetSlotName(const FString& InSlotName)
{

	UE_MVVM_SET_PROPERTY_VALUE(SlotName, InSlotName);
}

void UMVVM_LoadSlot::SetPlayerName(const FString& InPlayerName)
{
	/** 这个宏主要有以下功能
		*属性值比较：检查新值 InPlayerName 与当前值 PlayerName 是否不同
		值赋值：如果值不同，将新值赋给成员变量 PlayerName
		触发属性变更通知：调用 BroadcastFieldValueChanged() 通知所有绑定的 UI 控件
		返回布尔值：指示属性值是否实际发生了改变
	**/
	UE_MVVM_SET_PROPERTY_VALUE(PlayerName, InPlayerName);
}

void UMVVM_LoadSlot::SetMapName(const FString& InMapName)
{
	UE_MVVM_SET_PROPERTY_VALUE(MapName, InMapName);
}
