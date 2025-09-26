// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/AruaCharacter.h"

#include "GameFramework/CharacterMovementComponent.h"

AAruaCharacter::AAruaCharacter()
{

}

void AAruaCharacter::BeginPlay()
{
	Super::BeginPlay();

	//不在构造函数中写，为了防止被蓝图覆盖
	if (GetCharacterMovement())
	{
		//角色会旋转到面向移动方向,不会螃蟹步
		//移动方向控制旋转，使用了这个就要禁止控制器的输入控制角色旋转，两个系统会"打架"，角色旋转行为不可预测
		GetCharacterMovement()->bOrientRotationToMovement = true;
        
		// 设置合适的旋转速率（重要！）
		GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // 540度/秒
        
		// 确保有最小速度阈值来触发旋转
		GetCharacterMovement()->bUseControllerDesiredRotation = false;

		//限制在特定平面移动（通常是X-Y平面）
		GetCharacterMovement()->bConstrainToPlane = true;
		//游戏开始时自动对齐到约束平面
		GetCharacterMovement()->bSnapToPlaneAtStart = true;
	}

	//防止控制器输入直接旋转角色
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;
}

