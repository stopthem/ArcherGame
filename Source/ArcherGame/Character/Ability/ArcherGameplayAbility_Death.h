// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ArcherGame/Character/Ability/ArcherGameplayAbility.h"
#include "ArcherGameplayAbility_Death.generated.h"

class UDirectionalAnimationDataAsset;
/**
 * UArcherGameplayAbility_Death
 *
 * Class that created specially created for Death Ability.
 */
UCLASS()
class ARCHERGAME_API UArcherGameplayAbility_Death : public UArcherGameplayAbility
{
	GENERATED_BODY()

public:
	UArcherGameplayAbility_Death();

protected:
	virtual void OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;
};
