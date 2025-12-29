// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "SaveInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class USaveInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class AURA_API ISaveInterface
{
	GENERATED_BODY()
public:
	//设置Actor是否需要变换位置
	UFUNCTION(BlueprintCallable,BlueprintNativeEvent)
	bool ShouldLoadTransform();

	//在存档读取完数据后，调用更新Actor
	UFUNCTION(BlueprintCallable,BlueprintNativeEvent)
	void LoadActor();
};
