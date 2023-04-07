// Fill out your copyright notice in the Description page of Project Settings.


#include "ArcherDamageExecutionCalculation.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "ArcherGame/ArcherGameplayTags.h"
#include "ArcherGame/Character/Ability/ArcherGameplayEffectContext.h"
#include "ArcherGame/Character/Ability/Attribute/ArcherCombatSet.h"
#include "ArcherGame/Character/Ability/Attribute/ArcherHealthSet.h"

struct FDamageStatics
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(BaseDamage)

	FDamageStatics()
	{
		DEFINE_ATTRIBUTE_CAPTUREDEF(UArcherCombatSet, BaseDamage, Source, true);
	}
};

static const FDamageStatics& DamageStatics()
{
	static FDamageStatics Statics;
	return Statics;
}

UArcherDamageExecutionCalculation::UArcherDamageExecutionCalculation()
{
	RelevantAttributesToCapture.Add(DamageStatics().BaseDamageDef);
}

void UArcherDamageExecutionCalculation::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	const FGameplayEffectSpec& spec = ExecutionParams.GetOwningSpec();

	const FArcherGameplayEffectContext* typedContext = FArcherGameplayEffectContext::ExtractEffectContext(spec.GetContext());
	check(typedContext);

	const FGameplayTagContainer* sourceTags = spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* targetTags = spec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters evaluateParameters;
	evaluateParameters.SourceTags = sourceTags;
	evaluateParameters.TargetTags = targetTags;

	// damage causer set damage by AssignTagSetByCallerMagnitude and we are getting it 
	const float baseDamage = FMath::Max(spec.GetSetByCallerMagnitude(TAG_Data_Damage, false, -1.0f), -1.0f);

	float damageDone = FMath::Max(baseDamage, 0.0f);

	float attributeDamage = 0.0f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().BaseDamageDef, evaluateParameters, attributeDamage);
	if (attributeDamage > 0.0f)
	{
		damageDone += attributeDamage;
	}

	if (damageDone <= 0.0f)
	{
		return;
	}


	OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(UArcherHealthSet::GetDamageAttribute(), EGameplayModOp::Additive, damageDone));
}
