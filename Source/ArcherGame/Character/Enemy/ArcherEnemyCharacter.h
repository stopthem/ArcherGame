// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ArcherGame/Character/ArcherCharacter.h"
#include "ArcherGame/Character/Ability/Attribute/ArcherCombatSet.h"

#include "ArcherEnemyCharacter.generated.h"

/**
 * AArcherEnemyCharacter
 *
 * Base class that all enemies use.
 */
UCLASS()
class ARCHERGAME_API AArcherEnemyCharacter : public AArcherCharacter
{
	GENERATED_BODY()

public:
	AArcherEnemyCharacter();

	UFUNCTION(BlueprintCallable)
	float GetDamage() const
	{
		const UArcherCombatSet* combatSet = Cast<UArcherCombatSet>(GetAbilitySystemComponent()->GetAttributeSet(UArcherCombatSet::StaticClass()));
		return combatSet->GetBaseDamage();
	}
};
