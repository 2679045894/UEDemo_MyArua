// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "Components/SplineComponent.h"
#include "Input/AuraInputConfig.h"
#include "Interaction/EnemyInterface.h"
#include "UI/Widge/DamageTextComponent.h"
#include "MyPlayerController.generated.h"

/**
 * 
 */
class UInputMappingContext;
class UInputAction;
UCLASS()
class AURA_API AMyPlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	AMyPlayerController();
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="MyInput")
	TObjectPtr<UInputMappingContext> AuraContext;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="MyInput")
	TObjectPtr<UInputAction> MoveAction;

	virtual void BeginPlay() override;

	virtual void SetupInputComponent() override;

	virtual void Move(const FInputActionValue& InputActionValue);

	virtual void PlayerTick(float DeltaTime) override;

	IEnemyInterface* ThisActor;

	virtual void CursorTrace();

	//外部传入输入配置信息(DataAsset)
	UPROPERTY(EditDefaultsOnly)
	UAuraInputConfig* AuraInputConfig;

	//用于输入映射绑定对应函数
	void AbilityInputTagPressed(FGameplayTag InputTag);
	void AbilityInputTagReleased(FGameplayTag InputTag);
	void AbilityInputTagHeld(FGameplayTag InputTag);

	UPROPERTY()
	TObjectPtr<UAuraAbilitySystemComponent> AuraASC;
	
	UAuraAbilitySystemComponent* GetASC();

	//点击移动相关

	//缓存鼠标点击的目标位置
	FVector CachedDestination=FVector::ZeroVector;
	//鼠标按住的持续时间
	float FollowTime=0.f;
	//短按阈值
	float ShortPressThreshold=500.f;
	//是否自动移动
	bool bAutoRunning=false;
	//是否正在标准目标
	bool bTargeting=false;

	//自定移动的接受半径
	UPROPERTY(EditDefaultsOnly)
	float AutoRunAcceptanceRadius=50.f;

	//用于路径规划的样条组件
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USplineComponent> Spline;

	void AutoRun();

	bool bShiftKeyDown=false;
	void ShiftPressed(){bShiftKeyDown=true;};
	void ShiftReleased(){bShiftKeyDown=false;};
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UInputAction> ShiftAction;

	UFUNCTION(Client,Reliable)
	void ShowDamageNumber(float DamageAmount,ACharacter* TargetCharacter);

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UDamageTextComponent> DamageTextComponentClass;
};
