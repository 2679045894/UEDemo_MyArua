// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/MyGameModeBase.h"

#include "EngineUtils.h"
#include "Game/AuraGameInstance.h"
#include "GameFramework/PlayerStart.h"
#include "Kismet/GameplayStatics.h"
#include "MyActor/CheckPoint.h"
#include "Serialization/ObjectAndNameAsStringProxyArchive.h"

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
	LoadScreenSaveGame->PlayerLevel=LoadSlot->GetPlayerLevel();

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

void AMyGameModeBase::SaveWorldState(const UWorld* World,const FString& DestinationMapAssetName=FString(""))
{
	FString SavedMapName=World->GetName();
	SavedMapName.RemoveFromStart(World->StreamingLevelsPrefix);

	UAuraGameInstance* Instance=Cast<UAuraGameInstance>(GetGameInstance());
	check(Instance);
	ULoadScreenSaveGame* SaveObject=GetSaveSlotData(Instance->LoadSlotName,Instance->LoadSlotIndex);
	check(SaveObject);
	if (DestinationMapAssetName!=FString(""))
	{
		SaveObject->MapAssetName=DestinationMapAssetName;
		SaveObject->MapName=GetMapNameFromMapAssetName(DestinationMapAssetName);
	}
	if (!SaveObject->HasMap(SavedMapName))
	{
		FSaveMap SavedMap=FSaveMap();
		SavedMap.MapAssetName=SavedMapName;
		SaveObject->SavedMaps.Add(SavedMap);
	}
	FSaveMap SaveMap=SaveObject->GetSaveMapWithMapName(SavedMapName);
	SaveMap.SavedActors.Empty();
	//使用迭代器，遍历场景中的Actor，将需要保存的Actor保存到结构体内
	for (FActorIterator It(World);It;++It)
	{
		AActor* Actor=*It;
		if (!IsValid(Actor)||!Actor->Implements<USaveInterface>())continue;
		FSaveActor SaveActor=FSaveActor();
		SaveActor.ActorName=Actor->GetFName();
		SaveActor.Transform=Actor->GetTransform();

		//创建一个FMemoryWriter，用于将数据写入SaveActor.Bytes
		FMemoryWriter MemoryWriter(SaveActor.Bytes);

		//创建一个反序列容器，将对象的成员以名称和值的形式保存到MemoryWriter
		FObjectAndNameAsStringProxyArchive Archive(MemoryWriter, true);
		Archive.ArIsSaveGame=true;

		//将Actor所需要保存的数据写入Archive，Archive将把数据存储到SavedActor.Bytes
		Actor->Serialize(Archive);

		SaveMap.SavedActors.AddUnique(SaveActor);
	}
	for (FSaveMap& MapToReplace : SaveObject->SavedMaps)
	{
		if (MapToReplace.MapAssetName==SavedMapName)
		{
			MapToReplace=SaveMap;
		}
	}
	UGameplayStatics::SaveGameToSlot(SaveObject,Instance->LoadSlotName,Instance->LoadSlotIndex);
	
}

void AMyGameModeBase::LoadWorldState(const UWorld* World) const
{
	FString LoadMapName=World->GetMapName();
	LoadMapName.RemoveFromStart(World->StreamingLevelsPrefix);

	UAuraGameInstance* Instance=Cast<UAuraGameInstance>(GetGameInstance());
	check(Instance);
	if (UGameplayStatics::DoesSaveGameExist(Instance->LoadSlotName,Instance->LoadSlotIndex))
	{
		ULoadScreenSaveGame* LoadObject=Cast<ULoadScreenSaveGame>(UGameplayStatics::LoadGameFromSlot(Instance->LoadSlotName,Instance->LoadSlotIndex));
		if (LoadObject==nullptr)return;
		if (LoadObject->HasMap(LoadMapName))
		{
			for (FActorIterator It(World);It;++It)
			{
				AActor* Actor=*It;
				if (!IsValid(Actor)||!Actor->Implements<USaveInterface>())continue;
				for (FSaveActor SaveActor:LoadObject->GetSaveMapWithMapName(LoadMapName).SavedActors)
				{
					SaveActor.ActorName=Actor->GetFName();
					if (ISaveInterface::Execute_ShouldLoadTransform(Actor))
					{
						Actor->SetActorTransform(SaveActor.Transform);
					}
					//反序列化，创建一个Reader实例用于从二进制数据中读取内容
					FMemoryReader MemoryReader(SaveActor.Bytes);
					//用于序列化和反序列化对象的属性 true表现允许
					FObjectAndNameAsStringProxyArchive Archive(MemoryReader, true);
					//指定反序列化用于加载存档数据
					Archive.ArIsSaveGame=true;
					//执行反序列化，将二进制数据设置到Actor上
					Actor->Serialize(Archive);
					ISaveInterface::Execute_LoadActor(Actor);
				}
			}
		}
	}
}

FString AMyGameModeBase::GetMapNameFromMapAssetName(const FString& MapAssetName)
{
	for (auto& Map:Maps)
	{
		if (Map.Value.ToSoftObjectPath().GetAssetName()==MapAssetName)
		{
			return Map.Key;
		}
	}
	return FString();
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
