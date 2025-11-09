// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlueprintBase.h"
#include "BTService_FindNearestPlayer.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UBTService_FindNearestPlayer : public UBTService_BlueprintBase
{
	GENERATED_BODY()
protected:
	//TickNode 的主要作用是让任务能够在多个帧中持续运行，并在每一帧执行特定的逻辑检查或渐进式操作。
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	UPROPERTY(EditAnywhere)
	FBlackboardKeySelector TargetToFollowSelector;

	UPROPERTY(EditAnywhere)
	FBlackboardKeySelector DistanceToTargetSelector;
};
