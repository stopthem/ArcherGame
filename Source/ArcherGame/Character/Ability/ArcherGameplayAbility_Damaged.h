// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ArcherGameplayAbility.h"
#include "ArcherGameplayAbility_Damaged.generated.h"

class UDirectionalAnimationDataAsset;
/**
 * UArcherGameplayAbility_Damaged
 *
 * Class that created specially created for Damaged Ability.
 */
UCLASS(Abstract)
class ARCHERGAME_API UArcherGameplayAbility_Damaged : public UArcherGameplayAbility
{
	GENERATED_BODY()

public:
	UArcherGameplayAbility_Damaged();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Damaged Ability")
	TObjectPtr<UDirectionalAnimationDataAsset> DirectionalAnimationDataAsset;

	// Filled with TriggerEventData from ActivateAbility for blueprint usage
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Damaged Ability")
	FGameplayEventData GameplayEventData;

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;
};
