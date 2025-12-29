// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HUD/AuraHUD.h"

#include "AbilitySystem/AuraAttributeSet.h"
#include "UI/WidgeController/SpellMenuWidgetController.h"


//单例模式：确保只有一个OverplayWidgetController实例
UOverplayWidgetController* AAuraHUD::GetOverPlayWidgetController(const FWidgetControllerParams& WCParams)
{
	if (OverplayWidgetController==nullptr)
	{
		//使用UE4的对象系统创建UOverplayWidgetController的新实例 
		OverplayWidgetController=NewObject<UOverplayWidgetController>(this,OverplayWidgetControllerClass);
		OverplayWidgetController->SetWidgetController(WCParams);
		OverplayWidgetController->BindCallbacksToDependencies();
		return OverplayWidgetController;
	}
	return OverplayWidgetController;
}

UAttributeMenuWidgetController* AAuraHUD::GetAttributeMenuWidgetController(const FWidgetControllerParams& WCParams)
{
	if (AttributeMenuWidgetController==nullptr)
	{
		check(AttributeMenuWidgetControllerClass);
		AttributeMenuWidgetController=NewObject<UAttributeMenuWidgetController>(this,AttributeMenuWidgetControllerClass);
		AttributeMenuWidgetController->SetWidgetController(WCParams);
		AttributeMenuWidgetController->BindCallbacksToDependencies();
	}
	return AttributeMenuWidgetController;
}

USpellMenuWidgetController* AAuraHUD::GetSpellMenuWidgetController(const FWidgetControllerParams& WCParams)
{
	if (SpellMenuWidgetController==nullptr)
	{
		SpellMenuWidgetController=NewObject<USpellMenuWidgetController>(this,SpellMenuWidgetControllerClass);
		SpellMenuWidgetController->SetWidgetController(WCParams);
		SpellMenuWidgetController->BindCallbacksToDependencies();
	}
	return SpellMenuWidgetController;
}


void AAuraHUD::InitOverlay(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS)
{
	checkf(OverplayWidgetClass, TEXT("OverplayWidgetClass is null "));
	checkf(OverplayWidgetControllerClass, TEXT("OverplayWidgetControllerClass is null "));

	UUserWidget* Widget = CreateWidget<UUserWidget>(GetWorld(), OverplayWidgetClass);
	OverplayWidget = Cast<UAuraUserWidget>(Widget);
	const FWidgetControllerParams WidgetControllerParams(PC, PS, ASC, AS);
	UOverplayWidgetController* WidgetController = GetOverPlayWidgetController(WidgetControllerParams);
	
	GetAttributeMenuWidgetController(WidgetControllerParams);
	GetSpellMenuWidgetController(WidgetControllerParams);

	OverplayWidget->SetWidgetController(WidgetController);
	OverplayWidgetController->BroadcastInitialValues();
	Widget->AddToViewport();
}

