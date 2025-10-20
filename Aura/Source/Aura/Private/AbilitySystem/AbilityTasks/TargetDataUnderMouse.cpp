// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/AbilityTasks/TargetDataUnderMouse.h"

#include "Player/MyPlayerController.h"

UTargetDataUnderMouse* UTargetDataUnderMouse::CreateTargetDataUnderMouse(UGameplayAbility* OwningAbility)
{
	//使用模板函数创建新的 AbilityTask 实例
	UTargetDataUnderMouse* MyObj=NewAbilityTask<UTargetDataUnderMouse>(OwningAbility);
	return MyObj;
}


void UTargetDataUnderMouse::Activate()
{
	APlayerController* PC=Ability->GetCurrentActorInfo()->PlayerController.Get();
	FHitResult Hit;
	PC->GetHitResultUnderCursor(ECC_Visibility,false,Hit);
	ValidData.Broadcast(Hit.Location);
}
