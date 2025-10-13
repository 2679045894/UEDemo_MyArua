// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "UI/Widge/AuraUserWidget.h"
#include "UI/WidgeController/AttributeMenuWidgetController.h"
#include "UI/WidgeController/OverplayWidgetController.h"
#include "AuraHUD.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API AAuraHUD : public AHUD
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable)
	void InitOverlay(APlayerController* PC,APlayerState* PS,UAbilitySystemComponent* ASC,UAttributeSet* AS);
protected:
	UPROPERTY()
	TObjectPtr<UAuraUserWidget> OverplayWidget;
public:
	UFUNCTION(BlueprintCallable)
	//获取当前的OverplayWidgetController，如果没有就创建一个（NewObject）
	UOverplayWidgetController* GetOverPlayWidgetController(const FWidgetControllerParams& WCParams);

	//获取当前的AttirbuteMenu，如果没有就创建一个（NewObject）
	UAttributeMenuWidgetController* GetAttributeMenuWidgetController(const FWidgetControllerParams &WCParams);
	

private:
	//创建类是为了配置CreateWidget函数
	UPROPERTY(EditAnywhere)
	TSubclassOf<UAuraUserWidget> OverplayWidgetClass;

	UPROPERTY()
	TObjectPtr<UOverplayWidgetController> OverplayWidgetController;
	UPROPERTY(EditAnywhere)
	TSubclassOf<UOverplayWidgetController> OverplayWidgetControllerClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UAttributeMenuWidgetController> AttributeMenuWidgetControllerClass;

	UPROPERTY()
	TObjectPtr<UAttributeMenuWidgetController> AttributeMenuWidgetController;
};
