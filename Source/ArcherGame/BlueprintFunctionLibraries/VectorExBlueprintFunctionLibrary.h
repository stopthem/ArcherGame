// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "VectorExBlueprintFunctionLibrary.generated.h"

/**
 * UVectorExBlueprintFunctionLibrary
 *
 * A blueprint function library class for Vector functions.
 */
UCLASS()
class ARCHERGAME_API UVectorExBlueprintFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure)
	static FVector GetDirection(const FVector& fromVector, const FVector& toVector, bool bNormalized = true);
};
