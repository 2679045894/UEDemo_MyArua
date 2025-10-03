// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widge/AuraUserWidget.h"

#include "UI/WidgeController/OverplayWidgetController.h"

class UOverplayWidgetController;
void UAuraUserWidget::SetWidgetController(UObject* InWidgetController)
{
	WidgetController=InWidgetController;
	WidgetControllerSet();
}





