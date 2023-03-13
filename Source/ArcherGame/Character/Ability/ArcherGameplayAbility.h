// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "ArcherGameplayAbility.generated.h"

UENUM()
enum class EArcherAbilityInputID : uint8
{
	None,
	Confirm,
	Cancel,
	Dash,
	Aiming
};

/**
 * 
 */
UCLASS(Abstract)
class ARCHERGAME_API UArcherGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:
	UArcherGameplayAbility();

	// Input ID which activate this ability.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Ability Info")
	EArcherAbilityInputID AbilityInputID = EArcherAbilityInputID::None;

	// Level of the ability
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Ability Info")
	int32 AbilityLevel = 1;
};
