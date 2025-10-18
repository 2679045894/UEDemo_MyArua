// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputAction.h"
#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"
#include "AuraInputConfig.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct FAuraInputAction
{
	GENERATED_BODY();
	UPROPERTY(EditDefaultsOnly)
	UInputAction* InputAction;
	UPROPERTY(EditDefaultsOnly)
	FGameplayTag InputTag;
};
UCLASS()
class AURA_API UAuraInputConfig : public UDataAsset
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	TArray<FAuraInputAction> AuraInputActions;

	//通过标签返回对应的输入映射
	UInputAction* FindInputActionForTag(const FGameplayTag& InputTag,bool bLogNotFound=false);
};
