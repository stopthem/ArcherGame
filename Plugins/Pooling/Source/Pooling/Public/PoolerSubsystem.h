// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "PoolerSubsystem.generated.h"

struct FGameplayTag;
class APooler;
/**
 * UPoolerSubsystem
 *
 * Class for pooler plugin utility functions
 */
UCLASS()
class POOLING_API UPoolerSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

public:
	UPoolerSubsystem();

	virtual bool ShouldCreateSubsystem(UObject* Outer) const override
	{
		return true;
	}

	void AddToPoolerArray(APooler* pooler);

	// Get associated pooler by given gameplayTag
	UFUNCTION(BlueprintCallable, Category="Pool")
	APooler* GetPoolerFromGameplayTag(FGameplayTag gameplayTag);

private:
	TArray<TObjectPtr<APooler>> Poolers;
};
