// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "LootTiers.generated.h"

USTRUCT(BlueprintType)
struct FLootItem
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<AActor> LootClass;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float ChanceToSpawn=0.f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 MaxNumberToSpawn=0;
	//修改物品生成等级，false则使用敌人等级
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bLootLevelOverride=true;
};
UCLASS()
class AURA_API ULootTiers : public UDataAsset
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<FLootItem> LootItems;
	UFUNCTION(BlueprintCallable)
	TArray<FLootItem> GetLoopItem();
	
};
