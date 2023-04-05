// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "ArcherAbilityCanActivateMessage.generated.h"
/**
 * FArcherAbilityCostMessage
 *
 * Used in ArcherGameplayAbility_WithWidget's check cost to send message to widget 
 */
USTRUCT(BlueprintType)
struct FArcherAbilityCanActivateMessage
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite)
	bool bCanActivate;
};
