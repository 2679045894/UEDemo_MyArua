// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/AssetManager.h"
#include "AuraAssetManager.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UAuraAssetManager : public UAssetManager
{
	GENERATED_BODY()
	UAuraAssetManager();
private:
	

public:
	//获取自定义AssetManager(AuraAssetManager)的全局实例
	static UAuraAssetManager& Get();

	//资源初始化，自动调用
	virtual void StartInitialLoading() override;
	
};
