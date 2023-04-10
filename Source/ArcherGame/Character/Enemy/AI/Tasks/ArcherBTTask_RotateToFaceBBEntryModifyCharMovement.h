// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_RotateToFaceBBEntry.h"
#include "ArcherBTTask_RotateToFaceBBEntryModifyCharMovement.generated.h"

class UCharacterMovementComponent;
/**
 * 
 */
UCLASS()
class ARCHERGAME_API UArcherBTTask_RotateToFaceBBEntryModifyCharMovement : public UBTTask_RotateToFaceBBEntry
{
	GENERATED_BODY()

public:
	UArcherBTTask_RotateToFaceBBEntryModifyCharMovement(const FObjectInitializer& objectInitializer);

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual EBTNodeResult::Type AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

private:
	UPROPERTY()
	TObjectPtr<UCharacterMovementComponent> CharacterMovementComponent;

	void HandleCharMovementRotation(bool bEnableControlRotation) const;
};
