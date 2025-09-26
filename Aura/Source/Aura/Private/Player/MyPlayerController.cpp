// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/MyPlayerController.h"

#include "Interaction/EnemyInterface.h"

AMyPlayerController::AMyPlayerController()
{
	
}

void AMyPlayerController::BeginPlay()
{
	Super::BeginPlay();
	// 确保AuraContext不为空，否则崩溃
	check(AuraContext);

	//获取Enhanced Input子系统
	UEnhancedInputLocalPlayerSubsystem *Subsystem=
		ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	check(Subsystem);
	// 添加输入映射上下文到子系统（优先级为0） 
	Subsystem->AddMappingContext(AuraContext,0);

	bShowMouseCursor=true;
	FInputModeGameAndUI InputModeData;
	InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	SetInputMode(InputModeData);
}



void AMyPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	// 将基础的InputComponent转换为EnhancedInputComponent
	UEnhancedInputComponent* EnhancedInputComponent=CastChecked<UEnhancedInputComponent>(InputComponent);

	// 绑定MoveAction到Move函数，当Triggered时调用
	EnhancedInputComponent->BindAction(MoveAction,ETriggerEvent::Triggered,this,&AMyPlayerController::Move);
	
}

void AMyPlayerController::Move(const FInputActionValue& InputActionValue)
{
	// 从输入值获取2D向量（通常是WASD或摇杆输入）
	const FVector2D InputAxisVector=InputActionValue.Get<FVector2D>();
	// 获取控制器的当前旋转
	const FRotator Rotation=GetControlRotation();
	// 只保留Yaw（水平旋转），忽略Pitch和Roll
	const FRotator YawRotation(0.f,Rotation.Yaw,0.f);

	// 根据Yaw旋转计算前向和右向向量
	const FVector ForwardDirection=FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection=FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
	// 获取控制的Pawn并应用移动输入
	if (APawn* ControlledPawn=GetPawn<APawn>())
	{
		//按下W键时，InputAxisVector.Y=1,表示正向
		ControlledPawn->AddMovementInput(ForwardDirection,InputAxisVector.Y);
		ControlledPawn->AddMovementInput(RightDirection,InputAxisVector.X);
	}
}

void AMyPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);
	CursorTrace();
}

void AMyPlayerController::CursorTrace()
{
	FHitResult HitResult;
	IEnemyInterface* CurrentActor=nullptr;
	GetHitResultUnderCursor(ECC_Visibility,false,HitResult);
	if (HitResult.bBlockingHit && HitResult.GetActor() && HitResult.GetActor()->Implements<UEnemyInterface>())
	{
		CurrentActor=Cast<IEnemyInterface>(HitResult.GetActor());
	}
	if (CurrentActor!=ThisActor)
	{
		if (ThisActor!=nullptr)
		{
			ThisActor->UnHighlightActor();
		}
		if (CurrentActor!=nullptr)
		{
			CurrentActor->HighlightActor();
		}
		ThisActor=CurrentActor;
	}
		
}


