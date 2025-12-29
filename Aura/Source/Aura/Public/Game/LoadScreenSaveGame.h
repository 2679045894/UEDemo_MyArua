// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "GameFramework/SaveGame.h"
#include "LoadScreenSaveGame.generated.h"

UENUM(BlueprintType,Blueprintable)
enum ESaveSlotStatus
{
	Vacant,
	EnterName,
	Taken
};

USTRUCT(BlueprintType)
struct FSavedAbility
{
	GENERATED_BODY()
	//需要存储的技能
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UGameplayAbility> GameplayAbility;
	//当前技能等级
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 AbilityLevel=1;
	//当前技能标签
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag AbilityTag;
	//当前技能状态标签
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag AbilityStatusTag;
	//当前技能装配到的插槽
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag AbilityInputTag;
	//当前技能类型
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag AbilityType;
	//自定义运算符==，如果左右都是FSavedAbility类型的值，将通过函数内的值判断是否相等。
	inline bool operator==(const FSavedAbility& Right) const
	{
		// 建议比较多个字段以确保唯一性
		return AbilityTag.MatchesTagExact(Right.AbilityTag) &&
			   AbilityInputTag == Right.AbilityInputTag &&
			   AbilityType == Right.AbilityType;
	}
    
	// 同时添加 != 运算符
	inline bool operator!=(const FSavedAbility& Right) const
	{
		return !(*this == Right);
	}
};
//保存场景中的Actor
USTRUCT()
struct FSaveActor
{
	GENERATED_BODY()
	UPROPERTY()
	FName ActorName=FName();

	UPROPERTY()
	FTransform Transform=FTransform();

	//Actor身上序列号的数据，必须通过UPROPERTY定义过，旨在保存存档时使用
	UPROPERTY()
	TArray<uint8> Bytes;

	inline bool operator==(const FSaveActor& Right) const
	{
		return ActorName == Right.ActorName;
	}
	
	inline bool operator!=(const FSaveActor& Right) const
	{
		return !(*this == Right);
	}
	
};
//地图相关数据
USTRUCT()
struct FSaveMap
{
	GENERATED_BODY()
public:
	UPROPERTY()
	FString MapAssetName;

	UPROPERTY()
	TArray<FSaveActor> SavedActors;
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
	//技能容器
	UPROPERTY()
	TArray<FSavedAbility> SavedAbilities;

	//地图相关
	UPROPERTY()
	TArray<FSaveMap> SavedMaps;
	//通过地图名称获取地图数据
	FSaveMap GetSaveMapWithMapName(const FString& InMapName);

	//判断存档是否由对应的地图数据
	bool HasMap(const FString& InMapName);
};
