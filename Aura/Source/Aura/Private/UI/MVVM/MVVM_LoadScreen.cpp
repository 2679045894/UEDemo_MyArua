// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/MVVM/MVVM_LoadScreen.h"

void UMVVM_LoadScreen::SetWidgetName(const FString& InSlotName)
{
	if (UE_MVVM_SET_PROPERTY_VALUE(WidgetName,InSlotName))
	{
		//通过宏调用其他函数的广播
	}
}

void UMVVM_LoadScreen::InitializeLoadSlot()
{
	LoadSlot_0=NewObject<UMVVM_LoadSlot>(this,LoadSlotViewModelClass);
	LoadSlot_0->SetSlotName("LoadSlot_0");
	LoadSlots.Add(0,LoadSlot_0);
	LoadSlot_1=NewObject<UMVVM_LoadSlot>(this,LoadSlotViewModelClass);
	LoadSlot_1->SetSlotName("LoadSlot_1");
	LoadSlots.Add(1,LoadSlot_1);
	LoadSlot_2=NewObject<UMVVM_LoadSlot>(this,LoadSlotViewModelClass);
	LoadSlot_2->SetSlotName("LoadSlot_2");
	LoadSlots.Add(2,LoadSlot_2);
}

UMVVM_LoadSlot* UMVVM_LoadScreen::GetLoadSlotViewModelByIndex(int32 Index) const
{
	return LoadSlots.FindChecked(Index);
}

void UMVVM_LoadScreen::NewSlotButtonPressed(int32 Slot, const FString& EnterName)
{
}

void UMVVM_LoadScreen::NewGameButtonPressed(int32 Slot)
{
	LoadSlots[Slot]->SetWidgetSwitcherIndex.Broadcast(1);
}

void UMVVM_LoadScreen::SelectSlotButtonPressed(int32 Slot)
{
}
