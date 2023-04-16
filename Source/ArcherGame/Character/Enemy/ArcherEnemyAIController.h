// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DetourCrowdAIController.h"

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
	AArcherEnemyAIController(const FObjectInitializer& objectInitializer);

	virtual void BeginPlay() override;

protected:
	UFUNCTION()
	virtual void OnOwnerDeathStarted(AActor* actor);

private:
	void HandleLogicFromConsoleVariable() const;
};
