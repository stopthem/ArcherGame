// Fill out your copyright notice in the Description page of Project Settings.


#include "ArcherBTTask_RotateToFaceBBEntryModifyCharMovement.h"

#include "AIController.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

UArcherBTTask_RotateToFaceBBEntryModifyCharMovement::UArcherBTTask_RotateToFaceBBEntryModifyCharMovement(const FObjectInitializer& objectInitializer)
	: Super(objectInitializer)
{
}

EBTNodeResult::Type UArcherBTTask_RotateToFaceBBEntryModifyCharMovement::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (!CharacterMovementComponent)
	{
		if (UCharacterMovementComponent* characterMovementComponent = OwnerComp.GetAIOwner()->GetCharacter()->FindComponentByClass<UCharacterMovementComponent>())
		{
			CharacterMovementComponent = characterMovementComponent;
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("no character movement was found in avatar actor %s"), *OwnerComp.GetAIOwner()->GetCharacter()->GetActorNameOrLabel());
			return EBTNodeResult::Aborted;
		}
	}


	check(CharacterMovementComponent);

	HandleCharMovementRotation(true);
	return Super::ExecuteTask(OwnerComp, NodeMemory);
}

EBTNodeResult::Type UArcherBTTask_RotateToFaceBBEntryModifyCharMovement::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	check(CharacterMovementComponent);

	HandleCharMovementRotation(false);
	return Super::AbortTask(OwnerComp, NodeMemory);
}

void UArcherBTTask_RotateToFaceBBEntryModifyCharMovement::HandleCharMovementRotation(bool bEnableControlRotation) const
{
	CharacterMovementComponent->bUseControllerDesiredRotation = bEnableControlRotation;
	CharacterMovementComponent->bOrientRotationToMovement = !bEnableControlRotation;
}
