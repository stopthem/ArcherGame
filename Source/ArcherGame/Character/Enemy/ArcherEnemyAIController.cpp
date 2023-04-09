// Fill out your copyright notice in the Description page of Project Settings.


#include "ArcherEnemyAIController.h"

#include "BrainComponent.h"
#include "ArcherGame/Character/Ability/Attribute/ArcherHealthComponent.h"

AArcherEnemyAIController::AArcherEnemyAIController()
{
	AAIController::SetGenericTeamId(1);
}

void AArcherEnemyAIController::BeginPlay()
{
	Super::BeginPlay();

	// if the owner pawn has a health component, disable brain component logic when it died
	if (UArcherHealthComponent* foundHealthComponent = UArcherHealthComponent::FindHealthComponent(GetPawn()))
	{
		foundHealthComponent->OnDeathStarted.AddDynamic(this, &ThisClass::OnOwnerDeathStarted);
	}
}

void AArcherEnemyAIController::OnOwnerDeathStarted(AActor* actor)
{
	BrainComponent->StopLogic("Died");
}
