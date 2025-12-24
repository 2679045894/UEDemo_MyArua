// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/MVVM/MVVM_LoadSlot.h"

void UMVVM_LoadSlot::InitializeSlot() const
{
	SetWidgetSwitcherIndex.Broadcast(1);
}

void UMVVM_LoadSlot::SetSlotName(const FString& InSlotName)
{
	UE_MVVM_SET_PROPERTY_VALUE(SlotName, InSlotName);
}
