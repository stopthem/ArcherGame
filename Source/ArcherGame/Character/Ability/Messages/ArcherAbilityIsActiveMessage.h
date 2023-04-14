// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "ArcherAbilityIsActiveMessage.generated.h"

/*
 * FArcherAbilityIsActiveMessage
 *
 * Preferably used with Gameplay Message Subsystem Broadcast Message's payload
 * Holds information about ability is active
 */
USTRUCT(BlueprintType)
struct FArcherAbilityIsActiveMessage
{
	GENERATED_BODY()

public:
	// Is ability active right now?
	UPROPERTY(BlueprintReadOnly)
	bool bIsActive = false;
};
