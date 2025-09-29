// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HUD/AuraHUD.h"

#include "AbilitySystem/AruaAttributeSet.h"


void AAuraHUD::BeginPlay()
{
	Super::BeginPlay();
	UUserWidget* Widget=CreateWidget<UUserWidget>(GetWorld(),OverplayWidgetClass);
	Widget->AddToViewport();
}
//单例模式：确保只有一个OverplayWidgetController实例
UOverplayWidgetController* AAuraHUD::GetOverPlayWidgetController(const FWidgetControllerParams& WCParams)
{
	if (OverplayWidgetController==nullptr)
	{
		//使用UE4的对象系统创建UOverplayWidgetController的新实例 
		OverplayWidgetController=NewObject<UOverplayWidgetController>(this,OverplayWidgetControllerClass);
		OverplayWidgetController->SetWidgetController(WCParams);
		return OverplayWidgetController;
	}
	return OverplayWidgetController;
}

void AAuraHUD::InitOverlay(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS)
{
	checkf(OverplayWidgetClass,TEXT("OverplayWidgetClass is null "));
	checkf(OverplayWidgetControllerClass,TEXT("OverplayWidgetControllerClass is null "));

	//创建OverlayWidget实例
	//等效于：OverplayWidget=CreateWidget<UAuraUserWidget>(GetWorld(),OverplayWidgetClass);
	UUserWidget* Widget=CreateWidget<UUserWidget>(GetWorld(),OverplayWidgetClass);
	OverplayWidget=Cast<UAuraUserWidget>(Widget);
	
	//配置Widget Controller
	const FWidgetControllerParams WidgetControllerParams(PC,PS,ASC,AS);
	UOverplayWidgetController* UOverplayWidgetController=GetOverPlayWidgetController(WidgetControllerParams);

	//将控制器和控件进行绑定
	OverplayWidget->SetWidgetController(UOverplayWidgetController);

	Widget->AddToViewport();
}
