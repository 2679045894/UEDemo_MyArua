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
};
