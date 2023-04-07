// Fill out your copyright notice in the Description page of Project Settings.


#include "ArcherAttackCooldownCalculation.h"

#include "ArcherGame/Character/Ability/ArcherGameplayEffectContext.h"
#include "ArcherGame/Character/Ability/Attribute/ArcherCombatSet.h"

UArcherAttackCooldownCalculation::UArcherAttackCooldownCalculation()
{
	AttackSpeedDef = FGameplayEffectAttributeCaptureDefinition(UArcherCombatSet::GetBaseAttackSpeedAttribute(), EGameplayEffectAttributeCaptureSource::Source, false);

	RelevantAttributesToCapture.Add(AttackSpeedDef);
}

float UArcherAttackCooldownCalculation::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& spec) const
{
	const FGameplayTagContainer* sourceTags = spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* targetTags = spec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters evaluateParameters;
	evaluateParameters.SourceTags = sourceTags;
	evaluateParameters.TargetTags = targetTags;

	float attackSpeed = 0.0f;
	GetCapturedAttributeMagnitude(AttackSpeedDef, spec, evaluateParameters, attackSpeed);
	attackSpeed = FMath::Max(attackSpeed, 0.0f);

	return 1 / attackSpeed;
}
