// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "VectorExBlueprintFunctionLibrary.generated.h"

/**
 * 
 */
UCLASS()
class ARCHERGAME_API UVectorExBlueprintFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	static FVector GetDirection(FVector fromVector, FVector toVector, bool bNormalized = true);
};
