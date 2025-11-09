// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTService_FindNearestPlayer.h"

#include "AIController.h"
#include "BehaviorTree/BTFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"

void UBTService_FindNearestPlayer::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
	APawn* OwningPawn=AIOwner->GetPawn();
	bool bIsPlayer=OwningPawn->ActorHasTag("Player");
	const FName TargetTag=bIsPlayer?FName("Enemy"):FName("Player");

	//这里传入AActor*是为了配合下面的GetActor方法
	TArray<AActor*> ActorsWithTag;
	UGameplayStatics::GetAllActorsWithTag(OwningPawn,TargetTag,ActorsWithTag);

	AActor* ClosestActor=nullptr;
	float ClosestDistance=TNumericLimits<float>::Max();
	for (AActor* Actor:ActorsWithTag)
	{
		if (IsValid(Actor)&&IsValid(OwningPawn))
		if (OwningPawn->GetDistanceTo(Actor)<ClosestDistance)
		{
			ClosestDistance=OwningPawn->GetDistanceTo(Actor);
			ClosestActor=Actor;
		}
	}
	//更新黑板键值
	UBTFunctionLibrary::SetBlackboardValueAsObject(this,TargetToFollowSelector,ClosestActor);
	UBTFunctionLibrary::SetBlackboardValueAsFloat(this,DistanceToTargetSelector,ClosestDistance);
	
}
