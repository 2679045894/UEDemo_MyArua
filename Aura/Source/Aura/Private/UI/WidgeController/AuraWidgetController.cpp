// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgeController/AuraWidgetController.h"


void UAuraWidgetController::SetWidgetController(const FWidgetControllerParams& WCParams)
{
	PlayerState=WCParams.PlayerState;
	PlayerController=WCParams.PlayerController;
	AbilitySystemComponent=WCParams.AbilitySystemComponent;
	AttributeSet=WCParams.AttributeSet;
}
