// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "ArcherGameplayAbility.generated.h"

/**
 * EArcherAbilityActivationPolicy
 *
 * Defines how a gameplay ability should be activated
 */
UENUM(BlueprintType)
enum class EArcherAbilityActivationPolicy : uint8
{
	// Try to activate the ability when the input is triggered.
	OnInputTriggered,

	// Continually try to activate the ability while the input is active.
	WhileInputActive,

	// Ability will be manually activated.
	Manual
};


/**
 * UArcherGameplayAbility
 *
 * Base gameplay ability class used by this game.
 */
UCLASS(Abstract)
class ARCHERGAME_API UArcherGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:
	UArcherGameplayAbility();

	UPROPERTY(EditDefaultsOnly, Category="Ability Info", meta=(EditCondition="ActivationPolicy != EArcherAbilityActivationPolicy::Manual"))
	FGameplayTag InputTag;

	// Level of the ability
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Ability Info")
	int32 AbilityLevel = 1;

	EArcherAbilityActivationPolicy GetActivationPolicy() const { return ActivationPolicy; }

	UFUNCTION(BlueprintCallable)
	class AArcherPlayerCharacter* GetArcherPlayerCharacterFromActorInfo() const;

	UFUNCTION(BlueprintCallable)
	class AArcherCharacter* GetArcherCharacterFromActorInfo() const;

protected:
	// Defines how this ability is meant to activate.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability Info")
	EArcherAbilityActivationPolicy ActivationPolicy;
};
