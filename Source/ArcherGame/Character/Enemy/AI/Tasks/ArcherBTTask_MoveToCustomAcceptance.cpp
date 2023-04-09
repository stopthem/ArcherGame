// Fill out your copyright notice in the Description page of Project Settings.


#include "ArcherBTTask_MoveToCustomAcceptance.h"

#include "BehaviorTree/BlackboardComponent.h"

UArcherBTTask_MoveToCustomAcceptance::UArcherBTTask_MoveToCustomAcceptance(const FObjectInitializer& objectInitializer)
	: Super(objectInitializer)
{
}

EBTNodeResult::Type UArcherBTTask_MoveToCustomAcceptance::PerformMoveTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AcceptableRadius = OwnerComp.GetBlackboardComponent()->GetValueAsFloat(AcceptanceRadiusSelector.SelectedKeyName);
	return Super::PerformMoveTask(OwnerComp, NodeMemory);
}
