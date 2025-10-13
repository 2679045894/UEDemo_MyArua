// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgeController/AttributeMenuWidgetController.h"

#include "AuraGameplayTags.h"
#include "AbilitySystem/AuraAttributeSet.h"

void UAttributeMenuWidgetController::BindCallbacksToDependencies()
{
	
}

void UAttributeMenuWidgetController::BroadcastInitialValues()
{
	//获取AuraAttirbuteSet(里面包含数值)
	UAuraAttributeSet* AuraAttributeSet=Cast<UAuraAttributeSet>(AttributeSet);
	check(AuraAttributeInfo);
	//调用AuraAttributeInfo中的通过标签获取对应结构体方法，将标签转换成对应的结构体
	FAuraAttributeInfo Info=AuraAttributeInfo->FindAttributeInfoForTag(FAuraGameplayTags::Get().Attributes_Primary_Strength);
	//将AuraAttirbuteSet中的对应属性赋值给结构体中的值
	Info.AttributeValue=AuraAttributeSet->GetStrength();
	//广播该结构体
	AttributeInfoDelegate.Broadcast(Info);
}
