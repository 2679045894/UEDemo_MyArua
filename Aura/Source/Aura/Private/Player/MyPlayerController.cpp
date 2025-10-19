// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/MyPlayerController.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AuraGameplayTags.h"
#include "NavigationPath.h"
#include "NavigationSystem.h"
#include "Input/AuraInputComponent.h"
#include "Interaction/EnemyInterface.h"

AMyPlayerController::AMyPlayerController()
{
	bReplicates=true;

	//创建样条线
	Spline=CreateDefaultSubobject<USplineComponent>(TEXT("Spline"));
}

void AMyPlayerController::BeginPlay()
{
	Super::BeginPlay();
	// 确保AuraContext不为空，否则崩溃
	check(AuraContext);

	//获取Enhanced Input子系统
	UEnhancedInputLocalPlayerSubsystem *Subsystem=
		ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	/*check(Subsystem);
	// 添加输入映射上下文到子系统（优先级为0） 
	Subsystem->AddMappingContext(AuraContext,0);*/
	///??????????为什么换成的个就不会崩溃？？？？？？？？？？？？
	if (Subsystem)
	{
		Subsystem->AddMappingContext(AuraContext,0);
	}
	bShowMouseCursor=true;
	FInputModeGameAndUI InputModeData;
	InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	SetInputMode(InputModeData);
}



void AMyPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	// 将基础的InputComponent转换为AuraInputComponent
	UAuraInputComponent* AuraInputComponent=CastChecked<UAuraInputComponent>(InputComponent);

	// 绑定MoveAction到Move函数，当Triggered时调用
	AuraInputComponent->BindAction(MoveAction,ETriggerEvent::Triggered,this,&AMyPlayerController::Move);
	check(AuraInputConfig);
	//调用模版函数，该函数里面嵌套了3个BindAction
	AuraInputComponent->BindAbilityActions(AuraInputConfig,this,
		&ThisClass::AbilityInputTagPressed,&ThisClass::AbilityInputTagReleased,&ThisClass::AbilityInputTagHeld);
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
	AutoRun();
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

void AMyPlayerController::AbilityInputTagPressed(FGameplayTag InputTag)
{
	if (InputTag.MatchesTagExact(FAuraGameplayTags::Get().InputTag_LMB))
	{
		//判断当前点击的对象是否是敌人，设置目标状态
		bTargeting=ThisActor?true:false;
		//取消自动移动
		bAutoRunning=false;
	}
}

void AMyPlayerController::AbilityInputTagReleased(FGameplayTag InputTag)
{
	if (!InputTag.MatchesTagExact(FAuraGameplayTags::Get().InputTag_LMB))
	{
		if (GetASC())
		{
			GetASC()->AbilityInputTagReleased(InputTag);
		}
		return;
	}
	if (InputTag.MatchesTagExact(FAuraGameplayTags::Get().InputTag_LMB))
	{
		if (bTargeting)
		{
			if (GetASC())
			{
				GetASC()->AbilityInputTagReleased(InputTag);
			}
		}
		else
		{
			APawn* ControllerPawn=GetPawn<APawn>();
			if (FollowTime<=ShortPressThreshold&&ControllerPawn)
			{
				//同步查找从当前Pawn位置到CachedDestination（缓存目的地）的导航路径
				if (UNavigationPath* NavigationPath=UNavigationSystemV1::FindPathToLocationSynchronously(
					this,ControllerPawn->GetActorLocation(),CachedDestination))
				{
					Spline->ClearSplinePoints();
					for (const FVector& PointLocation:NavigationPath->PathPoints)
					{
						//绘制样条线
						Spline->AddSplinePoint(PointLocation,ESplineCoordinateSpace::Type::World);
						DrawDebugSphere(GetWorld(),PointLocation,8.f,8,FColor::Green,false,5.f);
					}
					CachedDestination=NavigationPath->PathPoints[NavigationPath->PathPoints.Num()-1];
					bAutoRunning=true;
				}
			}
		}
		FollowTime=0.f;
		bTargeting=false;
	}
}

void AMyPlayerController::AbilityInputTagHeld(FGameplayTag InputTag)
{
	// 处理非左键输入（技能按键）
	if (!InputTag.MatchesTagExact(FAuraGameplayTags::Get().InputTag_LMB))
	{
		if (GetASC())
		{
			GetASC()->AbilityInputTagHeld(InputTag);
		}
		return;
	}
	//左键按住逻辑
	if (InputTag.MatchesTagExact(FAuraGameplayTags::Get().InputTag_LMB))
	{
		//如果当前按压的对象是敌人，则发动技能
		if (bTargeting)
		{
			if (GetASC())
			{
				GetASC()->AbilityInputTagHeld(InputTag);
			}
		}

		// 若当前按压的对象是空对象：移动角色
		else
		{
			// 累计按住时间
			FollowTime+=GetWorld()->GetTimeSeconds();

			FHitResult HitResult;
			//将鼠标按下的对象存放至碰撞信息
			if (GetHitResultUnderCursor(ECC_Visibility,false,HitResult))
			{
				// 缓存目标位置
				CachedDestination=HitResult.ImpactPoint;
			}

			//获取Pawn进行移动
			if (APawn* ControlledPawn = GetPawn())
			{
				const FVector WorldDirection=(CachedDestination-ControlledPawn->GetActorLocation()).GetSafeNormal();
				ControlledPawn->AddMovementInput(WorldDirection);
			}
		}
	}
}

UAuraAbilitySystemComponent* AMyPlayerController::GetASC()
{
	if (AuraASC==nullptr)
	{
		//通过将APawn转化成AuraASC
		AuraASC=Cast<UAuraAbilitySystemComponent>
		(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetPawn<APawn>()));
	}
	return AuraASC;
}

void AMyPlayerController::AutoRun()
{
	if (!bAutoRunning) return;
	if (APawn* ControllerPawn=GetPawn())
	{
		// 在Spline上找到离Pawn当前位置最近的点坐标
		const FVector LocationOnSpline=Spline->
		FindLocationClosestToWorldLocation(ControllerPawn->GetActorLocation(),ESplineCoordinateSpace::World);
		// 获取在Spline上该点的切线方向（即移动方向）
		const FVector Direction=Spline->FindDirectionClosestToWorldLocation(LocationOnSpline,ESplineCoordinateSpace::World);
		// 向Pawn添加移动输入，使其沿Spline方向移动
		ControllerPawn->AddMovementInput(Direction);

		// 计算当前位置与目标位置之间的距离
		const float DistanceToDestination=(LocationOnSpline-CachedDestination).Length();
		if (DistanceToDestination<=AutoRunAcceptanceRadius)
		{
			bAutoRunning=false;
		}
	}
}


