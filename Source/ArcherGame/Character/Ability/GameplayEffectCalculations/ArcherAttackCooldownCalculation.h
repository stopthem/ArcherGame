// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayModMagnitudeCalculation.h"
#include "ArcherAttackCooldownCalculation.generated.h"

/**
 * UArcherAttackCooldownCalculation
 *
 * Calculates cooldown duration based on attack speed attribute
 */
UCLASS()
class ARCHERGAME_API UArcherAttackCooldownCalculation : public UGameplayModMagnitudeCalculation
{
	GENERATED_BODY()

public:
	UArcherAttackCooldownCalculation();

	// i don't know why we create a struct for ExecutionCalculation and not for ModMagnitudeCalculation but https://github.com/tranek/GASDocumentation/blob/master/README.md#concepts-ge-mmc must know something :D
	FGameplayEffectAttributeCaptureDefinition AttackSpeedDef;

	virtual float CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& spec) const override;
};
