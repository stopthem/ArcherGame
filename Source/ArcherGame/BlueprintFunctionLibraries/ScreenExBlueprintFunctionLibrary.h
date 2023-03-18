// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "ScreenExBlueprintFunctionLibrary.generated.h"

/**
 * 
 */
UCLASS()
class ARCHERGAME_API UScreenExBlueprintFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, meta=(WorldContext="context"))
	static FVector2D GetScreenCenterToScreenPosDir(UObject* context, FVector2D screenPos, bool bNegate = true, bool bNormalize = true);
};
