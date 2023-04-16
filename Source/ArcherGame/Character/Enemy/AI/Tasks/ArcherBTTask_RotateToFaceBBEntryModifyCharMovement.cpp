// Fill out your copyright notice in the Description page of Project Settings.


#include "ArcherBTTask_RotateToFaceBBEntryModifyCharMovement.h"

#include "AIController.h"
#include "GameFramework/CharacterMovementComponent.h"

UArcherBTTask_RotateToFaceBBEntryModifyCharMovement::UArcherBTTask_RotateToFaceBBEntryModifyCharMovement(const FObjectInitializer& objectInitializer)
	: Super(objectInitializer)
{
	// C++ tasks default shared but blueprint tasks are not.
	// This allows us to instance this task.
	bCreateNodeInstance = true;
}

EBTNodeResult::Type UArcherBTTask_RotateToFaceBBEntryModifyCharMovement::ExecuteTask(UBehaviorTreeComponent& ownerComp, uint8* nodeMemory)
{
	const EBTNodeResult::Type nodeResult = Super::ExecuteTask(ownerComp, nodeMemory);

	// This task will end when nodeResult is Succeeded, so we want control rotation to be reverse that.
	HandleCharMovementRotation(nodeResult != EBTNodeResult::Succeeded, ownerComp, nodeMemory);

	return nodeResult;
}

void UArcherBTTask_RotateToFaceBBEntryModifyCharMovement::HandleCharMovementRotation(bool bEnableControlRotation, UBehaviorTreeComponent& ownerComp, uint8* nodeMemory)
{
	if (!CharacterMovementComponent)
	{
		CharacterMovementComponent = ownerComp.GetAIOwner()->GetPawn()->FindComponentByClass<UCharacterMovementComponent>();

		if (!CharacterMovementComponent)
		{
			UE_LOG(LogTemp, Warning, TEXT("no character movement component was found in avatar actor %s"), *ownerComp.GetAIOwner()->GetPawn()->GetActorNameOrLabel());
			AbortTask(ownerComp, nodeMemory);
			return;
		}
	}

	CharacterMovementComponent->bUseControllerDesiredRotation = bEnableControlRotation;
	CharacterMovementComponent->bOrientRotationToMovement = !bEnableControlRotation;
}
