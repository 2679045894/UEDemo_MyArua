// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/MVVM/MVVM_LoadScreen.h"

#include "Game/AuraGameInstance.h"
#include "Game/MyGameModeBase.h"
#include "Kismet/GameplayStatics.h"

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
	AMyGameModeBase* AuraGameMode=Cast<AMyGameModeBase>(UGameplayStatics::GetGameMode(this));
	LoadSlots[Slot]->SetPlayerName(EnterName);
	//设置地图名称
	LoadSlots[Slot]->SetMapName(AuraGameMode->DefaultMapName);
	LoadSlots[Slot]->LoadSlotStatus=Taken;
	//存储关卡生成点
	LoadSlots[Slot]->PlayerStartTag=AuraGameMode->DefaultPlayerStartTag;
	AuraGameMode->SaveSlotData(LoadSlots[Slot],Slot);
	LoadSlots[Slot]->InitializeSlot();
}

void UMVVM_LoadScreen::NewGameButtonPressed(int32 Slot)
{
	LoadSlots[Slot]->SetWidgetSwitcherIndex.Broadcast(1);
}

void UMVVM_LoadScreen::SelectSlotButtonPressed(int32 Slot)
{
	
}

void UMVVM_LoadScreen::EnterGameButtonPressed(const int32 Slot)
{
	AMyGameModeBase* AuraGameMode=Cast<AMyGameModeBase>(UGameplayStatics::GetGameMode(this));
	
	UAuraGameInstance* AuraGameInstance=Cast<UAuraGameInstance>(AuraGameMode->GetGameInstance());
	AuraGameInstance->PlayerStartTag=LoadSlots[Slot]->PlayerStartTag;
	AuraGameInstance->LoadSlotIndex=Slot;
	AuraGameInstance->LoadSlotName=LoadSlots[Slot]->GetSlotName();
	AuraGameMode->TravelToMap(LoadSlots[Slot]);
}

void UMVVM_LoadScreen::LoadData()
{
	//获取到加载存档界面的GameMode
	AMyGameModeBase* AuraGameMode=Cast<AMyGameModeBase>(UGameplayStatics::GetGameMode(this));

	//遍历映射，获取对应存档
	for (const TTuple<int32,UMVVM_LoadSlot*> Slot:LoadSlots)
	{
		ULoadScreenSaveGame* SaveGame=AuraGameMode->GetSaveSlotData(Slot.Value->GetSlotName(),Slot.Key);

		//获取存档数据
		const FString PlayerName=SaveGame->PlayerName;
		const TEnumAsByte<ESaveSlotStatus> SaveSlotStatus=SaveGame->SaveSlotStatus;
		const FName PlayerStartTag=SaveGame->PlayerStartTag;

		//设置存档视图模型数据
		Slot.Value->SetPlayerName(PlayerName);
		Slot.Value->LoadSlotStatus=SaveSlotStatus;
		Slot.Value->SetMapName(SaveGame->MapName);
		Slot.Value->PlayerStartTag=PlayerStartTag;
		//调用视图模型初始化
		Slot.Value->InitializeSlot();
	}
}

void UMVVM_LoadScreen::DeleteButtonPressed(int32 Slot)
{
	AMyGameModeBase* AuraGameMode=Cast<AMyGameModeBase>(UGameplayStatics::GetGameMode(this));
	AuraGameMode->DeleteSlotData(LoadSlots[Slot]->GetSlotName(),Slot);
	LoadSlots[Slot]->LoadSlotStatus=Vacant;
	LoadSlots[Slot]->InitializeSlot();
}
