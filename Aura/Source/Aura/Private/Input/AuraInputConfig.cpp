// Fill out your copyright notice in the Description page of Project Settings.


#include "Input/AuraInputConfig.h"

UInputAction* UAuraInputConfig::FindInputActionForTag(const FGameplayTag& InputTag, bool bLogNotFound) 
{
	for (FAuraInputAction &Action : UAuraInputConfig::AuraInputActions)
	{
		//如果当前标签不为空以及当前标签是传入的目标标签，返回该输入事件
		if (Action.InputAction&&Action.InputTag.MatchesTag(InputTag))
		{
			return Action.InputAction;
		}
	}

	if (bLogNotFound)
	{
		GEngine->AddOnScreenDebugMessage(-1,5.f,FColor::Red,TEXT("没有找到对应的输入标签"));
	}
	return nullptr;
}
