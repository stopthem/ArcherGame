// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "MathExBlueprintFunctionLibrary.generated.h"

/**
 * UMathExBlueprintFunctionLibrary
 *
 * A blueprint function library class for Math functions
 */
UCLASS()
class ARCHERGAME_API UMathExBlueprintFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure)
	static float AngleBetween(const FVector& aVector, const FVector& bVector);

	UFUNCTION(BlueprintPure)
	static float SignedAngleBetween(const FVector& aVector, const FVector& bVector, const FVector& axisVector);

	UFUNCTION(BlueprintPure)
	static float NormalizeToRangeClamped(double value, double min, double max);

	UFUNCTION(BlueprintPure)
	static float ValueInCurveOrGiven(const UCurveFloat* curveFloat, float val);

	UFUNCTION(BlueprintPure)
	static float NormalizeToFloatRange(float value, const FFloatRange& floatRange);

	UFUNCTION(BlueprintPure)
	static float LerpInFloatRange(float value, const FFloatRange& floatRange);
};
