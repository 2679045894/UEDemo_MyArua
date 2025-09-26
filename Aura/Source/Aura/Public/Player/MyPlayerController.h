// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "InputActionValue.h"
#include "Interaction/EnemyInterface.h"
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
};
