// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/MyGameModeBase.h"

#include "Game/AuraGameInstance.h"
#include "GameFramework/PlayerStart.h"
#include "Kismet/GameplayStatics.h"
#include "MyActor/CheckPoint.h"

void AMyGameModeBase::SaveSlotData(const UMVVM_LoadSlot* LoadSlot, int32 SlotIndex)
{
	//检查是否由对应名称的存档
	if (UGameplayStatics::DoesSaveGameExist(LoadSlot->GetSlotName(),SlotIndex))
	{
		//删除已保存的存档
		UGameplayStatics::DeleteGameInSlot(LoadSlot->GetSlotName(),SlotIndex);
	}

	// 添加调试信息
	UE_LOG(LogTemp, Warning, TEXT("LoadScreenSaveGameClass: %s"), 
		*GetNameSafe(LoadScreenSaveGameClass));
    
	// 检查是否为有效的类
	if (!LoadScreenSaveGameClass)
	{
		UE_LOG(LogTemp, Error, TEXT("LoadScreenSaveGameClass is NULL!"));
        
		// 尝试使用默认类
		LoadScreenSaveGameClass=FindObject<UClass>(ANY_PACKAGE, TEXT("LoadScreenSaveGame"));
		UE_LOG(LogTemp, Warning, TEXT("Tried to find class: %s"), 
			*GetNameSafe(LoadScreenSaveGameClass));
        
		if (!LoadScreenSaveGameClass)
		{
			LoadScreenSaveGameClass = ULoadScreenSaveGame::StaticClass();
			UE_LOG(LogTemp, Warning, TEXT("Using StaticClass: %s"), 
				*GetNameSafe(LoadScreenSaveGameClass));
		}
	}
    
	// 检查类是否可实例化
	if (LoadScreenSaveGameClass && LoadScreenSaveGameClass->HasAnyClassFlags(CLASS_Abstract))
	{
		UE_LOG(LogTemp, Error, TEXT("LoadScreenSaveGameClass is abstract!"));
	}
	//创建一个新的存档
	USaveGame* SaveGameObject=UGameplayStatics::CreateSaveGameObject(LoadScreenSaveGameClass);
	ULoadScreenSaveGame* LoadScreenSaveGame=Cast<ULoadScreenSaveGame>(SaveGameObject);

	//设置需要保存的数据
	LoadScreenSaveGame->PlayerName=LoadSlot->GetPlayerName();
	LoadScreenSaveGame->SlotName=LoadSlot->GetSlotName();
	LoadScreenSaveGame->SlotIndex=SlotIndex;
	LoadScreenSaveGame->SaveSlotStatus=Taken;
	LoadScreenSaveGame->MapName=LoadSlot->GetMapName();
	LoadScreenSaveGame->PlayerStartTag=LoadSlot->PlayerStartTag;

	//保存存档
	UGameplayStatics::SaveGameToSlot(LoadScreenSaveGame,LoadSlot->GetSlotName(),SlotIndex);
}

ULoadScreenSaveGame* AMyGameModeBase::GetSaveSlotData(const FString& SlotName, int32 SlotIndex)
{
	USaveGame* SaveGameObject;
	if (UGameplayStatics::DoesSaveGameExist(SlotName, SlotIndex))
	{
		SaveGameObject=UGameplayStatics::LoadGameFromSlot(SlotName, SlotIndex);
	}
	else
	{
		// 添加调试信息
		UE_LOG(LogTemp, Warning, TEXT("LoadScreenSaveGameClass: %s"), 
			*GetNameSafe(LoadScreenSaveGameClass));
    
		// 检查是否为有效的类
		if (!LoadScreenSaveGameClass)
		{
			UE_LOG(LogTemp, Error, TEXT("LoadScreenSaveGameClass is NULL!"));
        
			// 尝试使用默认类
			LoadScreenSaveGameClass=FindObject<UClass>(ANY_PACKAGE, TEXT("LoadScreenSaveGame"));
			UE_LOG(LogTemp, Warning, TEXT("Tried to find class: %s"), 
				*GetNameSafe(LoadScreenSaveGameClass));
        
			if (!LoadScreenSaveGameClass)
			{
				LoadScreenSaveGameClass = ULoadScreenSaveGame::StaticClass();
				UE_LOG(LogTemp, Warning, TEXT("Using StaticClass: %s"), 
					*GetNameSafe(LoadScreenSaveGameClass));
			}
		}
    
		// 检查类是否可实例化
		if (LoadScreenSaveGameClass && LoadScreenSaveGameClass->HasAnyClassFlags(CLASS_Abstract))
		{
			UE_LOG(LogTemp, Error, TEXT("LoadScreenSaveGameClass is abstract!"));
		}
		SaveGameObject=UGameplayStatics::CreateSaveGameObject(LoadScreenSaveGameClass);
	}

	ULoadScreenSaveGame* LoadScreenSaveGame=Cast<ULoadScreenSaveGame>(SaveGameObject);
	return LoadScreenSaveGame;
}

void AMyGameModeBase::DeleteSlotData(const FString& SlotName, int32 SlotIndex)
{
	if (UGameplayStatics::DoesSaveGameExist(SlotName, SlotIndex))
	{
		UGameplayStatics::DeleteGameInSlot(SlotName, SlotIndex);
	}
}

void AMyGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	Maps.Add(DefaultMapName,DefaultMap);
}

void AMyGameModeBase::TravelToMap(const UMVVM_LoadSlot* Slot)
{
	const FString SlotName=Slot->GetSlotName();
	const int32 SlotIndex=Slot->SlotIndex;

	//打开地图
	UGameplayStatics::OpenLevelBySoftObjectPtr(Slot,Maps.FindChecked(Slot->GetMapName()));
}

AActor* AMyGameModeBase::ChoosePlayerStart_Implementation(AController* Player)
{
	const UAuraGameInstance* AuraGameInstance=Cast<UAuraGameInstance>(GetGameInstance());
	TArray<AActor*> Actors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(),APlayerStart::StaticClass(),Actors);
	HighlightEnabledCheckPoints(Actors);
	if (Actors.Num()>0)
	{
		AActor* SelectedActor=Actors[0];
		for (AActor* Actor:Actors)
		{
			if (APlayerStart* PlayerStart=Cast<APlayerStart>(Actor))
			{
				if (PlayerStart->PlayerStartTag==AuraGameInstance->PlayerStartTag)
				{
					SelectedActor=Actor;
					break;
				}
			}
		}
		return SelectedActor;
	}
	return nullptr;
}

ULoadScreenSaveGame* AMyGameModeBase::RetrieveInGameSaveData()
{
	UAuraGameInstance* AuraGameInstance=Cast<UAuraGameInstance>(GetGameInstance());
	const FString SlotName=AuraGameInstance->LoadSlotName;
	const int32 SlotIndex=AuraGameInstance->LoadSlotIndex;
	return GetSaveSlotData(SlotName,SlotIndex);
}

void AMyGameModeBase::SaveInGameProgressData(ULoadScreenSaveGame* SaveObject) const
{
	UAuraGameInstance* AuraGameInstance=Cast<UAuraGameInstance>(GetGameInstance());
	SaveObject->PlayerStartTag=AuraGameInstance->PlayerStartTag;
	const FString SlotName=AuraGameInstance->LoadSlotName;
	const int32 SlotIndex=AuraGameInstance->LoadSlotIndex;
	UGameplayStatics::SaveGameToSlot(SaveObject,SlotName, SlotIndex);
}

void AMyGameModeBase::HighlightEnabledCheckPoints(TArray<AActor*> CheckPoints)
{
	ULoadScreenSaveGame* SaveObject=RetrieveInGameSaveData();
	for (AActor* Actor:CheckPoints)
	{
		if (ACheckPoint* CheckPoint=Cast<ACheckPoint>(Actor))
		{
			if (SaveObject->ActivatedPlayerStatTags.Contains(CheckPoint->PlayerStartTag))
			{
				CheckPoint->HandleGlowEffect();
			}
		}
	}
}
