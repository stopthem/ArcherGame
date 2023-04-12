// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "ArcherInteractionDurationMessage.generated.h"

class UGameplayEffect;
/**
 * FArcherInteractionDurationMessage
 *
 * Preferably used with Gameplay Message Subsystem Broadcast Message's payload
 * Holds information about the instigator and duration
 */
USTRUCT(BlueprintType)
struct FArcherInteractionDurationMessage
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<AActor> Instigator = nullptr;

	UPROPERTY(BlueprintReadWrite)
	float Duration = 0;
};
