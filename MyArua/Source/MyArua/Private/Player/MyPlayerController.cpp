// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/MyPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"


AMyPlayerController::AMyPlayerController()
{
	bReplicates = true;
}

void AMyPlayerController::BeginPlay()
{
	Super::BeginPlay();
	check(AuraContext);

	//通过玩家控制器获取当前玩家然后获取当前玩家的增强输入系统
	UEnhancedInputLocalPlayerSubsystem* Subsystem=
		ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	check(Subsystem);
	// 添加上下文到子系统
	Subsystem->AddMappingContext(AuraContext,0);

	//显示光标
	bShowMouseCursor=true;
	//// 设置默认光标样式为系统默认
	DefaultMouseCursor=EMouseCursor::Default;

	FInputModeGameAndUI InputModeData;
	// 鼠标不锁定到视口
	InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	////鼠标捕获时不隐藏光标
	InputModeData.SetHideCursorDuringCapture(false);
	SetInputMode(InputModeData);
}

void AMyPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	
	UEnhancedInputComponent* EnhancedInputComponent=CastChecked<UEnhancedInputComponent>(InputComponent);
	// 将MoveAction绑定到Move函数
	EnhancedInputComponent->BindAction(MoveAction,ETriggerEvent::Triggered,this,&AMyPlayerController::Move);
}

void AMyPlayerController::Move(const FInputActionValue& InputActionValue)
{
	// 获取2D输入向量（WASD/摇杆输入）
	const FVector2D InputAxisVector=InputActionValue.Get<FVector2D>();
	// 基于控制器旋转计算方向向量
	const FRotator Rotation=GetControlRotation();
	const FRotator YawRotation(0.f,Rotation.Yaw,0.f);

	// 计算前后移动方向（忽略俯仰和滚动）
	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	// 计算左右移动方向（使用完整旋转）
	const FVector RightDirection = FRotationMatrix(Rotation).GetUnitAxis(EAxis::Y);

	// 应用到控制的Pawn
	if (APawn* ControlledPawn=GetPawn<APawn>())
	{
		ControlledPawn->AddMovementInput(ForwardDirection,InputAxisVector.Y);
		ControlledPawn->AddMovementInput(RightDirection,InputAxisVector.X);
	}
}
