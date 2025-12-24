// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "LoadScreenSaveGame.generated.h"

UENUM(BlueprintType,Blueprintable)
enum ESaveSlotStatus
{
	Vacant,
	EnterName,
	Taken
};
UCLASS(BlueprintType)
class AURA_API ULoadScreenSaveGame : public ULocalPlayerSaveGame
{
	GENERATED_BODY()
public:
	//存档名称
	UPROPERTY()
	FString SlotName=FString();

	//存档索引
	UPROPERTY()
	int32 SlotIndex=0;

	//玩家名称
	UPROPERTY()
	FString PlayerName=FString("Default Name");

	//当前存档进入存档界面是，默认显示的用户界面
	UPROPERTY()
	TEnumAsByte<ESaveSlotStatus> SaveSlotStatus=Vacant;
};
