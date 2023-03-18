// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "ArcherGameplayAbility.generated.h"

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
 * 
 */
UCLASS(Abstract)
class ARCHERGAME_API UArcherGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:
	UArcherGameplayAbility();

	UPROPERTY(EditDefaultsOnly, Category="Ability Info")
	FGameplayTag InputTag;

	// Level of the ability
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Ability Info")
	int32 AbilityLevel = 1;

	EArcherAbilityActivationPolicy GetActivationPolicy() const { return ActivationPolicy; }

	UFUNCTION(BlueprintCallable)
	class AArcherPlayerCharacter* GetArcherPlayerCharacterFromActorInfo() const;

protected:
	// Defines how this ability is meant to activate.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability Info")
	EArcherAbilityActivationPolicy ActivationPolicy;
};
