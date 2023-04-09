// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "PoolerGameInstanceSubsystem.generated.h"

struct FGameplayTag;
class APooler;
/**
 * UPoolerGameInstanceSubsystem
 *
 * Class for pooler plugin utility functions
 */
UCLASS()
class POOLING_API UPoolerGameInstanceSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	UPoolerGameInstanceSubsystem();

	void AddToPoolerArray(APooler* pooler);

	// Get associated pooler by given gameplayTag
	UFUNCTION(BlueprintCallable, Category="Pool")
	APooler* GetPoolerFromGameplayTag(FGameplayTag gameplayTag);

private:
	TArray<TObjectPtr<APooler>> Poolers;
};
