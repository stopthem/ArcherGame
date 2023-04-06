// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemGlobals.h"
#include "ArcherAbilitySystemGlobals.generated.h"

/**
 * UArcherAbilitySystemGlobals
 *
 * 
 */
UCLASS(Config=Game)
class ARCHERGAME_API UArcherAbilitySystemGlobals : public UAbilitySystemGlobals
{
	GENERATED_BODY()

#pragma region UAbilitySystemGlobals Interface
	virtual FGameplayEffectContext* AllocGameplayEffectContext() const override;
#pragma endregion
};
