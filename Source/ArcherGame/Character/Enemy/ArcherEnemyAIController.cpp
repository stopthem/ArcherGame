// Fill out your copyright notice in the Description page of Project Settings.


#include "ArcherEnemyAIController.h"

#include "BrainComponent.h"
#include "ArcherGame/Character/Ability/Attribute/ArcherHealthComponent.h"
#include "Navigation/CrowdFollowingComponent.h"

AArcherEnemyAIController::AArcherEnemyAIController(const FObjectInitializer& objectInitializer)
	: Super(objectInitializer.SetDefaultSubobjectClass<UCrowdFollowingComponent>(TEXT("PathFollowingComponent")))
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

#if WITH_EDITOR
	FAutoConsoleVariableSink CVarSink(FConsoleCommandDelegate::CreateUObject(this, &ThisClass::HandleLogicFromConsoleVariable));

	GetWorldTimerManager().SetTimerForNextTick([&]
	{
		HandleLogicFromConsoleVariable();
	});
#endif
}

void AArcherEnemyAIController::OnOwnerDeathStarted(AActor* actor)
{
	BrainComponent->StopLogic("Died");
}

void AArcherEnemyAIController::HandleLogicFromConsoleVariable() const
{
	bool bResumeLogic = false;
	IConsoleManager::Get().FindConsoleVariable(TEXT("ai.AllBehaviorTrees.Activated"))->GetValue(bResumeLogic);
	if (bResumeLogic)
	{
		BrainComponent->ResumeLogic("Console variable is 1");
	}
	else
	{
		BrainComponent->PauseLogic("Console variable is 0");
	}
}
