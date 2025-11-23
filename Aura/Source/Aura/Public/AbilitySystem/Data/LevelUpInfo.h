// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "LevelUpInfo.generated.h"

/**
 * 
 */
USTRUCT(Blueprintable,BlueprintType)
struct FAuraLevelUpInfo
{
	GENERATED_BODY()

	//升到此等级所需的经验值
	UPROPERTY(EditDefaultsOnly)
	int32 LevelUpRequirement=0;
	//达到此等级奖励的属性点
	UPROPERTY(EditDefaultsOnly)
	int32 AttributePointAward=1;
	//达到此等级降级的可分配技能点数
	UPROPERTY(EditDefaultsOnly)
	int32 SpellPointAward=1;
	
};
UCLASS()
class AURA_API ULevelUpInfo : public UDataAsset
{
	GENERATED_BODY()
public:
	UPROPERTY(EditDefaultsOnly)
	TArray<FAuraLevelUpInfo> LevelUpInformation;
	//通过经验值值获取角色的等级
	UFUNCTION()
	int32 FindLevelForXP(int32 XP);
	
};
