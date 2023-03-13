// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "ArcherAbilitySystemComponent.generated.h"

/**
 * 
 */
UCLASS()
class ARCHERGAME_API UArcherAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:
	UArcherAbilitySystemComponent();

	// Default abilities that will be given at OnPossessed
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(DisplayName ="Default Character Abilities"), Category= "Setup")
	TArray<TSubclassOf<class UArcherGameplayAbility>> DefaultAbilities;

	// Default effects that will be given at OnPossessed
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(DisplayName ="Default Character Effects"), Category= "Setup")
	TArray<TSubclassOf<class UArcherGameplayEffect>> DefaultEffects;

	void InitializeDefaultAbilitiesEffects();

	// When a ability action is performed these get called for abilities
	virtual void AbilityInputPressed(const FGameplayTag& InputTag);
	virtual void AbilityInputReleased(const FGameplayTag& InputTag);
};
