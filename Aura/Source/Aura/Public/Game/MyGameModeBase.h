// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LoadScreenSaveGame.h"
#include "GameFramework/GameModeBase.h"
#include "UI/MVVM/MVVM_LoadSlot.h"
#include "MyGameModeBase.generated.h"

class UCharacterClassInfo;
class UAbilityInfo;
UCLASS()
class AURA_API AMyGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
public:
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	UCharacterClassInfo* CharacterClassInfo;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UAbilityInfo> AbilityInfo;

	/**
	 * 创建新存档
	 * @param LoadSlot 需要保存的视图模型示例
	 * @param SlotIndex 存档索引
	 */
	void SaveSlotData(const UMVVM_LoadSlot* LoadSlot,int32 SlotIndex);

	/**
	 * 获取保存的存档
	 * @param SlotName 存档名称（每个存档名称固定）
	 * @param SlotIndex 存档索引
	 * @return 
	 */
	ULoadScreenSaveGame* GetSaveSlotData(const FString& SlotName,int32 SlotIndex);

	//存档使用的数据结构
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<USaveGame> LoadScreenSaveGameClass;

	UFUNCTION()
	static void DeleteSlotData(const FString& SlotName,int32 SlotIndex);

	//初始地图名称
	UPROPERTY(EditDefaultsOnly)
	FString DefaultMapName;

	//游戏初始地图
	UPROPERTY(EditDefaultsOnly)
	TSoftObjectPtr<UWorld> DefaultMap;

	//地图名称和地图的映射
	UPROPERTY(EditDefaultsOnly)
	TMap<FString,TSoftObjectPtr<UWorld>> Maps;

	virtual void BeginPlay() override;

	//打开地图
	void TravelToMap(const UMVVM_LoadSlot* Slot);

	//玩家切换关卡默认生成位置的PlayerStart标签
	UPROPERTY(EditDefaultsOnly)
	FName DefaultPlayerStartTag;

	virtual AActor* ChoosePlayerStart_Implementation(AController* Player) override;

	//获取当前游戏进行中所使用的存档数据
	ULoadScreenSaveGame* RetrieveInGameSaveData();

	//保存游戏进度
	void SaveInGameProgressData(ULoadScreenSaveGame* SaveObject) const;

	//高亮已经激活的检查点
private:
	void HighlightEnabledCheckPoints(TArray<AActor*> CheckPoints);
};
