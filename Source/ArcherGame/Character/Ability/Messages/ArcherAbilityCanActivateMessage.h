// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "ArcherAbilityCanActivateMessage.generated.h"
/**
 * FArcherAbilityCostMessage
 *
 * Preferably used with Gameplay Message Subsystem Broadcast Message's payload
 * Holds information about ability can activate
 */
USTRUCT(BlueprintType)
struct FArcherAbilityCanActivateMessage
{
	GENERATED_BODY()

public:
	// Can we activate the ability?
	UPROPERTY(BlueprintReadOnly)
	bool bCanActivate = false;
};
