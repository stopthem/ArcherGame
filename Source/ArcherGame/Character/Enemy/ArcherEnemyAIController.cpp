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

	BrainComponent->PauseLogic("");

#if WITH_EDITOR
	IConsoleManager::Get().FindConsoleVariable(TEXT("ai.AllBehaviorTrees.Activated"))->SetOnChangedCallback(FConsoleVariableDelegate::CreateUObject(this, &ThisClass::HandleAIBehaviourTreeConsoleVariableChanged));
#endif
}

void AArcherEnemyAIController::OnOwnerDeathStarted(AActor* actor)
{
	BrainComponent->StopLogic("Died");
}

void AArcherEnemyAIController::HandleAIBehaviourTreeConsoleVariableChanged(IConsoleVariable* variable) const
{
	if (variable->GetBool())
	{
		BrainComponent->ResumeLogic("Console variable is 1");
	}
	else
	{
		BrainComponent->PauseLogic("Console variable is 0");
	}
}
