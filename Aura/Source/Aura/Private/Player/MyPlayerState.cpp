// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/MyPlayerState.h"

AMyPlayerState::AMyPlayerState()
{
	//网络更新频率的设置
	NetUpdateFrequency=100.0f;
	AbilitySystemComponent=CreateDefaultSubobject<UAbilitySystemComponent>("AbilitySystemComponent");
	//启用网络复制
	AbilitySystemComponent->SetIsReplicated(true);
	AttributeSet=CreateDefaultSubobject<UAttributeSet>("AttributeSet");
	//设置复制模式
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);
}

UAbilitySystemComponent* AMyPlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}
