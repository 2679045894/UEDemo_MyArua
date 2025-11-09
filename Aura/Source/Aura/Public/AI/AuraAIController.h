// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "AuraAIController.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API AAuraAIController : public AAIController
{
	GENERATED_BODY()
	AAuraAIController();
public:
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category = "AICom")
	TObjectPtr<UBlackboardComponent> BlackboardComponent;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AICom")
	TObjectPtr<UBehaviorTreeComponent> BehaviorTreeComponent;
};
