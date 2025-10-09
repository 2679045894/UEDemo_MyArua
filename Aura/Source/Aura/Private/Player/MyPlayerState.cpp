// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/MyPlayerState.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"  // 添加这行！
#include "AbilitySystem/AuraAttributeSet.h"
#include "Net/UnrealNetwork.h"

AMyPlayerState::AMyPlayerState()
{
	//网络更新频率的设置
	NetUpdateFrequency=100.0f;
	AbilitySystemComponent=CreateDefaultSubobject<UAuraAbilitySystemComponent>("AbilitySystemComponent");
	//启用网络复制
	AbilitySystemComponent->SetIsReplicated(true);
	AttributeSet=CreateDefaultSubobject<UAuraAttributeSet>("AttributeSet");
	//设置复制模式
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);
}

UAuraAbilitySystemComponent* AMyPlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void AMyPlayerState::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AMyPlayerState, Level);
}

void AMyPlayerState::OnRep_Level(int32 OldLevel) const
{
	
}



