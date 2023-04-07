// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ArcherAttributeSet.h"
#include "ArcherCombatSet.generated.h"

/**
 * UArcherCombatSet
 *
 * Class that defines attributes that are necessary for applying damage or healing.
 */
UCLASS(BlueprintType)
class ARCHERGAME_API UArcherCombatSet : public UArcherAttributeSet
{
	GENERATED_BODY()

public:
	UArcherCombatSet();

	ATTRIBUTE_ACCESSORS(UArcherCombatSet, BaseDamage)
	ATTRIBUTE_ACCESSORS(UArcherCombatSet, BaseHeal)
	ATTRIBUTE_ACCESSORS(UArcherCombatSet, BaseAttackSpeed);

	// The base amount of damage to apply in the damage execution.
	UPROPERTY(BlueprintReadOnly, Category="Archer|Combat", meta=(AllowPrivateAccess = true))
	FGameplayAttributeData BaseDamage;

private:
	// The base amount of healing to apply in the heal execution.
	UPROPERTY(BlueprintReadOnly, Category="Archer|Combat", meta=(AllowPrivateAccess = true))
	FGameplayAttributeData BaseHeal;

	UPROPERTY(BlueprintReadOnly, Category="Archer|Combat", meta=(AllowPrivateAccess = true))
	FGameplayAttributeData BaseAttackSpeed;
};
