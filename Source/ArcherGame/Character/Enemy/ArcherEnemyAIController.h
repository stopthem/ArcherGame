// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"

#include "ArcherEnemyAIController.generated.h"

/**
 * AArcherEnemyAIController
 *
 * Base ai class that all enemy ai characters use.
 */
UCLASS()
class ARCHERGAME_API AArcherEnemyAIController : public AAIController
{
	GENERATED_BODY()

public:
	AArcherEnemyAIController();

	virtual void BeginPlay() override;

protected:
	UFUNCTION()
	virtual void OnOwnerDeathStarted(AActor* actor);

private:
#if WITH_EDITOR
	void HandleAIBehaviourTreeConsoleVariableChanged(IConsoleVariable* variable) const;
#endif
};
