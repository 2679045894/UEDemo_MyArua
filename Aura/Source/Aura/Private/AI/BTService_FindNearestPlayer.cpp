// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTService_FindNearestPlayer.h"

#include "AIController.h"

void UBTService_FindNearestPlayer::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, *AIOwner->GetName());
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, *ActorOwner->GetName());
}
