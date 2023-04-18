// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "ArcherLevelEndMessage.generated.h"

/**
 * FArcherLevelEndMessage
 *
 * Preferably used with Gameplay Message Subsystem Broadcast Message's payload
 * Holds information about win or fail.
 */
USTRUCT(BlueprintType)
struct FArcherLevelEndMessage
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite)
	bool bWin = false;
};
