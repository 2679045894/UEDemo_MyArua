// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/AbilityTasks/TargetDataUnderMouse.h"

#include "Aura/Aura.h"
#include "Player/MyPlayerController.h"

UTargetDataUnderMouse* UTargetDataUnderMouse::CreateTargetDataUnderMouse(UGameplayAbility* OwningAbility)
{
	//使用模板函数创建新的 AbilityTask 实例
	UTargetDataUnderMouse* MyObj=NewAbilityTask<UTargetDataUnderMouse>(OwningAbility);
	return MyObj;
}

//激活流程
void UTargetDataUnderMouse::Activate()
{
	//检查当前是否由本地控制
	bool bIsLocallyControlled=Ability->GetCurrentActorInfo()->IsLocallyControlled();
	if (bIsLocallyControlled)
	{
		//客户端
		SendMouseCursorData();
	}
	else
	{
		//服务器
		//获取技能标识符
		const FGameplayAbilitySpecHandle SpecHandle=GetAbilitySpecHandle();
		//获取预测键
		const FPredictionKey ActivationPredictionKey=GetActivationPredictionKey();
		//检查缓存数据(判断数据是否到达)
		const bool bCalledDelegate=AbilitySystemComponent.Get()->CallReplicatedTargetDataDelegatesIfSet(SpecHandle,ActivationPredictionKey);
		//注册数据到达委托
		AbilitySystemComponent.Get()->AbilityTargetDataSetDelegate(SpecHandle,ActivationPredictionKey).
		AddUObject(this,&UTargetDataUnderMouse::OnTargetDataReplicatedCallback);
		
		//设置等待状态(等待管理)
		if (!bCalledDelegate)
		{
			SetWaitingOnRemotePlayerData();
		}
	}

}

void UTargetDataUnderMouse::SendMouseCursorData() const
{
	//预测窗口：创建作用域预测窗口，支持客户端预测
	FScopedPredictionWindow ScopedPredictionWindow(AbilitySystemComponent.Get());
	APlayerController* PC=Ability->GetCurrentActorInfo()->PlayerController.Get();
	FHitResult Hit;
	PC->GetHitResultUnderCursor(ECC_TARGET_CHANNEL, false, Hit);

	//创建目标数据容器
	FGameplayAbilityTargetDataHandle DataHandle;
	//创建单目标命中数据实例
	FGameplayAbilityTargetData_SingleTargetHit* Data=new FGameplayAbilityTargetData_SingleTargetHit();
	//填充命中结果
	Data->HitResult=Hit;
	//将数据添加到容器中
	DataHandle.Add(Data);

	//发送数据
	AbilitySystemComponent->ServerSetReplicatedTargetData(
		GetAbilitySpecHandle(),
		GetActivationPredictionKey(),
		DataHandle,
		FGameplayTag(),
		AbilitySystemComponent->ScopedPredictionKey);

	//本地广播
	if (ShouldBroadcastAbilityTaskDelegates())
	{
		ValidData.Broadcast(DataHandle);
	}
	
}

void UTargetDataUnderMouse::OnTargetDataReplicatedCallback(const FGameplayAbilityTargetDataHandle& DataHandle,
	FGameplayTag ActivationTag) const
{
	//消费客户端赋值的目标数据
	AbilitySystemComponent->ConsumeClientReplicatedTargetData(GetAbilitySpecHandle(),GetActivationPredictionKey());
	//检查是否应该广播Ability Task委托
	if (ShouldBroadcastAbilityTaskDelegates())
	{
		ValidData.Broadcast(DataHandle);
	}
}
