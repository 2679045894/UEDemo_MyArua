// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AuraInputConfig.h"
#include "EnhancedInputComponent.h"
#include "AuraInputComponent.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UAuraInputComponent : public UEnhancedInputComponent
{
	GENERATED_BODY()
	
public:
	template<class UserClass, typename PressedFuncType, typename ReleasedFuncType, typename HeldFuncType>
void BindAbilityActions(const UAuraInputConfig* InputConfig, UserClass* Object, PressedFuncType PressedFunc,
						ReleasedFuncType ReleasedFunc, HeldFuncType HeldFunc)  // 修正参数顺序
	{
		check(InputConfig);
		for (const FAuraInputAction& Action : InputConfig->AuraInputActions)
		{
			if (Action.InputAction && Action.InputTag.IsValid())
			{
				//按键按下时的回调函数
				if (PressedFunc)
				{
					BindAction(Action.InputAction, ETriggerEvent::Started, Object, PressedFunc, Action.InputTag);  // 修正参数顺序
				}
				//按键释放时的回调函数
				if (ReleasedFunc)
				{
					BindAction(Action.InputAction, ETriggerEvent::Completed, Object, ReleasedFunc, Action.InputTag);  // 修正参数顺序
				}
				//按键持续按住时的回调函数
				if (HeldFunc)
				{
					BindAction(Action.InputAction, ETriggerEvent::Triggered, Object, HeldFunc, Action.InputTag);  // 修正参数顺序
				}
			}
		}
	}
};
