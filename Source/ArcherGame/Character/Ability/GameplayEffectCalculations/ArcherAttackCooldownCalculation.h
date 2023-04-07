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
	
	FGameplayEffectAttributeCaptureDefinition AttackSpeedDef;

	virtual float CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& spec) const override;
};
