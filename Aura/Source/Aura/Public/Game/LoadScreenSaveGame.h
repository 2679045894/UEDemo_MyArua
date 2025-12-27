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

	UPROPERTY()
	FString MapName=FString("Default Name");

	//当前存档进入存档界面是，默认显示的用户界面
	UPROPERTY()
	TEnumAsByte<ESaveSlotStatus> SaveSlotStatus=Vacant;

	//存档点
	UPROPERTY()
	FName PlayerStartTag;

	//已高亮的存档点
	UPROPERTY()
	TArray<FName> ActivatedPlayerStatTags=TArray<FName>();

	UPROPERTY()
	bool bFirstTimeLoadIn=true;
	//玩家属性
	UPROPERTY()
	int32 PlayerLevel=1;
	UPROPERTY()
	int32 XP=0;
	UPROPERTY()
	int32 SpellPoints=0;
	UPROPERTY()
	int32 AttributePoints=3;
	UPROPERTY()
	float Strength=0;
	UPROPERTY()
	float Intelligence=0;
	UPROPERTY()
	float Resilience=0;
	UPROPERTY()
	float Vigor=0;
};
