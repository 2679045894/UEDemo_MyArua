// Fill out your copyright notice in the Description page of Project Settings.


#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "InputActionValue.h"
#include "InputAction.h"
#include "MyPlayerController.generated.h"

/**
 * 
 */

class UInputMappingContext;
UCLASS()
class MYARUA_API AMyPlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	AMyPlayerController();
	
protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;
	
private:
	UPROPERTY(EditAnywhere,Category="Input")
	TObjectPtr<UInputMappingContext> AuraContext;
	//IA
	UPROPERTY(EditAnywhere,Category="Input")
	TObjectPtr<UInputAction> MoveAction;

	void Move(const FInputActionValue& InputActionValue);
};
