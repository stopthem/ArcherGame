// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "AnimBlueprintFunctionLibrary.generated.h"

/**
 * UAnimBlueprintFunctionLibrary
 *
 * A blueprint function library class for Animation functions
 */
UCLASS()
class ARCHERGAME_API UAnimBlueprintFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	// Calculates the rate that animation montage needs to play at in order to complete in given wantedTime.
	UFUNCTION(BlueprintPure)
	static float GetPlayRateFromTime(const UAnimMontage* animMontage, float wantedTime);
};
