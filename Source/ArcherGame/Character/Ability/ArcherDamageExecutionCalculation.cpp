// Fill out your copyright notice in the Description page of Project Settings.


#include "ArcherDamageExecutionCalculation.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "ArcherGame/ArcherGameplayTags.h"
#include "Attribute/ArcherCombatSet.h"
#include "Attribute/ArcherHealthSet.h"

struct FDamageStatics
{
	FGameplayEffectAttributeCaptureDefinition BaseDamageRef;

	FDamageStatics()
	{
		BaseDamageRef = FGameplayEffectAttributeCaptureDefinition(UArcherCombatSet::GetBaseDamageAttribute(), EGameplayEffectAttributeCaptureSource::Source, true);
	}
};

static FDamageStatics& DamageStatics()
{
	static FDamageStatics Statics;
	return Statics;
}

UArcherDamageExecutionCalculation::UArcherDamageExecutionCalculation()
{
	RelevantAttributesToCapture.Add(DamageStatics().BaseDamageRef);
}

void UArcherDamageExecutionCalculation::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	const FGameplayEffectSpec& spec = ExecutionParams.GetOwningSpec();

	const FGameplayTagContainer* sourceTags = spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* targetTags = spec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters evaluateParameters;
	evaluateParameters.SourceTags = sourceTags;
	evaluateParameters.TargetTags = targetTags;

	const float baseDamage = FMath::Max(spec.GetSetByCallerMagnitude(TAG_Data_Damage, false, -1.0f), -1.0f);
	// ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().BaseDamageRef, evaluateParameters, baseDamage);

	const float damageDone = FMath::Max(baseDamage, 0.0f);

	if (damageDone <= 0.0f)
	{
		return;
	}
	
	OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(UArcherHealthSet::GetDamageAttribute(), EGameplayModOp::Additive, damageDone));
}
