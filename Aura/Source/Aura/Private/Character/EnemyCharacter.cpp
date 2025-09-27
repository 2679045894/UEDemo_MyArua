// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/EnemyCharacter.h"


void AEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();
	//敌人初始化，Owner和Avatar都是自己
	AbilitySystemComponent->InitAbilityActorInfo(this,this);
}

AEnemyCharacter::AEnemyCharacter()
{
	GetMesh()->SetCollisionResponseToChannel(ECC_Visibility,ECR_Block);

	AbilitySystemComponent=CreateDefaultSubobject<UAbilitySystemComponent>("AbilitySystemComponent");
	//启用游戏能力系统组件（GAS）的网络复制功能。
	AbilitySystemComponent->SetIsReplicated(true);
	AttributeSet=CreateDefaultSubobject<UAttributeSet>("AttributeSet");
	//设置复制模式
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);
}

void AEnemyCharacter::HighlightActor()
{
	bHighlight = true;
	// 启用网格体的自定义深度渲染
	GetMesh()->SetRenderCustomDepth(true);
	//// 设置模板值为250
	GetMesh()->SetCustomDepthStencilValue(250);
	Weapon->SetRenderCustomDepth(true);
	Weapon->SetCustomDepthStencilValue(250);
}

void AEnemyCharacter::UnHighlightActor()
{
	bHighlight = false;
	GetMesh()->SetRenderCustomDepth(false);
	Weapon->SetRenderCustomDepth(false);
	//GetMesh()->SetCustomDepthStencilValue(250);
}
