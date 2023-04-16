// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_RotateToFaceBBEntry.h"
#include "ArcherBTTask_RotateToFaceBBEntryModifyCharMovement.generated.h"

class UCharacterMovementComponent;
/**
 * UArcherBTTask_RotateToFaceBBEntryModifyCharMovement
 *
 * Basically rotate to face bb entry but when this task is in progress, alters char movement of the owner to use control rotation.
 */
UCLASS()
class ARCHERGAME_API UArcherBTTask_RotateToFaceBBEntryModifyCharMovement : public UBTTask_RotateToFaceBBEntry
{
	GENERATED_BODY()

public:
	UArcherBTTask_RotateToFaceBBEntryModifyCharMovement(const FObjectInitializer& objectInitializer);

	// The safest place to check if task is in progress or success-fail to alter char movement.
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& ownerComp, uint8* nodeMemory) override;

private:
	// Owner's char movement.
	UPROPERTY()
	TObjectPtr<UCharacterMovementComponent> CharacterMovementComponent;

	// Handle char movement using control rotation and orient rotation to movement.
	void HandleCharMovementRotation(bool bEnableControlRotation, UBehaviorTreeComponent& ownerComp, uint8* nodeMemory);
};
