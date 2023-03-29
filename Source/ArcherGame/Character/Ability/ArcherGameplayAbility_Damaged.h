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

protected:
	virtual void OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;
};
