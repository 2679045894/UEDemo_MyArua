// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "UI/MVVM/MVVM_LoadScreen.h"
#include "UI/Widge/LoadScreenWidget.h"
#include "LoadScreenHUD.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API ALoadScreenHUD : public AHUD
{
	GENERATED_BODY()
public:
	virtual void BeginPlay() override;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget> LoadScreenWidgetClass;
	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<ULoadScreenWidget> LoadScreenWidget;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UMVVM_LoadScreen> LoadScreenViewModelClass;
	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<UMVVM_LoadScreen> LoadScreenViewModel;
};
