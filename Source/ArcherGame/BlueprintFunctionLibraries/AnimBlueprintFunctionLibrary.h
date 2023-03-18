// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "AnimBlueprintFunctionLibrary.generated.h"

/**
 * 
 */
UCLASS()
class ARCHERGAME_API UAnimBlueprintFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure)
	static float GetPlayRateFromTime(const UAnimMontage* animMontage, float wantedTime);
};
