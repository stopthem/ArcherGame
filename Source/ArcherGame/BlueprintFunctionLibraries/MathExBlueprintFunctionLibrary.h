// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "MathExBlueprintFunctionLibrary.generated.h"

/**
 * 
 */
UCLASS()
class ARCHERGAME_API UMathExBlueprintFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure)
	static float GetAngleBetween(FVector aVector, FVector bVector);

	UFUNCTION(BlueprintPure)
	static float GetSignedAngleBetween(FVector aVector, FVector bVector, FVector axisVector);
};
