// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/AuraAIController.h"

#include "BehaviorTree/BlackboardComponent.h"

AAuraAIController::AAuraAIController()
{
	BlackboardComponent=CreateDefaultSubobject<UBlackboardComponent>("BlackboardComp");
	BehaviorTreeComponent=CreateDefaultSubobject<UBehaviorTreeComponent>("BehaviorTreeComp");
}
