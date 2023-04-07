// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectExecutionCalculation.h"
#include "ArcherDamageExecutionCalculation.generated.h"

/**
 * UArcherDamageExecutionCalculation
 *
 * Execution used by gameplay effects to apply damage to the health attributes.
 */
UCLASS()
class ARCHERGAME_API UArcherDamageExecutionCalculation : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()

public:
	UArcherDamageExecutionCalculation();

protected:
	virtual void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;
};
