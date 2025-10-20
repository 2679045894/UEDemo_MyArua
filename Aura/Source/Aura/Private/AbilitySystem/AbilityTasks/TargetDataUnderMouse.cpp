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
	//检查当前是否由本地控制
	bool bIsLocallyControlled=Ability->GetCurrentActorInfo()->IsLocallyControlled();
	if (bIsLocallyControlled)
	{
		SendMouseCursorData();
	}
	else
	{
		
	}

}

void UTargetDataUnderMouse::SendMouseCursorData() const
{
	FScopedPredictionWindow ScopedPredictionWindow(AbilitySystemComponent.Get());
	APlayerController* PC=Ability->GetCurrentActorInfo()->PlayerController.Get();
	FHitResult Hit;
	PC->GetHitResultUnderCursor(ECC_Visibility, false, Hit);

	//创建目标数据容器
	FGameplayAbilityTargetDataHandle DataHandle;
	//创建单目标命中数据实例
	FGameplayAbilityTargetData_SingleTargetHit* Data=new FGameplayAbilityTargetData_SingleTargetHit();
	//填充命中结果
	Data->HitResult=Hit;
	//将数据添加到容器中
	DataHandle.Add(Data);

	AbilitySystemComponent->ServerSetReplicatedTargetData(
		GetAbilitySpecHandle(),
		GetActivationPredictionKey(),
		DataHandle,
		FGameplayTag(),
		AbilitySystemComponent->ScopedPredictionKey);

	if (ShouldBroadcastAbilityTaskDelegates())
	{
		ValidData.Broadcast(DataHandle);
	}
	
}
