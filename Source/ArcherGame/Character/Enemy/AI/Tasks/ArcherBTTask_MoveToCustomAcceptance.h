// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_MoveTo.h"
#include "ArcherBTTask_MoveToCustomAcceptance.generated.h"

/**
 * UArcherBTTask_MoveTo
 *
 * Adds capability of dynamic AcceptanceRadius
 *
 * Seen in behaviour tree task dropdown "MoveToCustomAcceptance".
 */
UCLASS(Config=Game)
class ARCHERGAME_API UArcherBTTask_MoveToCustomAcceptance : public UBTTask_MoveTo
{
	GENERATED_BODY()

public:
	UArcherBTTask_MoveToCustomAcceptance(const FObjectInitializer& objectInitializer);

	// will override AcceptanceRadius
	UPROPERTY(EditAnywhere, Category="Node")
	FBlackboardKeySelector AcceptanceRadiusSelector;

	virtual EBTNodeResult::Type PerformMoveTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
