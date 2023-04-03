// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "InputBlueprintFunctionLibrary.generated.h"

class UInputMappingContext;
class UInputAction;
struct FEnhancedActionKeyMapping;
/**
 * UInputBlueprintFunctionLibrary
 *
 * Input based functions
 */
UCLASS()
class ARCHERGAME_API UInputBlueprintFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UInputBlueprintFunctionLibrary();

	// Get corresponding EnhancedActionKeyMapping from action inside given mapping
	UFUNCTION(BlueprintCallable)
	static FEnhancedActionKeyMapping GetEnhancedKeyMappingFromAction(const UInputMappingContext* mappingContext, const UInputAction* inputAction);
};
