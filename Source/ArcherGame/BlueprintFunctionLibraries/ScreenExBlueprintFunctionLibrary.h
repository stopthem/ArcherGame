// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "ScreenExBlueprintFunctionLibrary.generated.h"

/**
 * UScreenExBlueprintFunctionLibrary
 *
 * A blueprint function library class for Screen functions
 */
UCLASS()
class ARCHERGAME_API UScreenExBlueprintFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	/**
	 * @brief Get the direction from center to given screenPos
	 * @param context The world context which will be auto filled by blueprint but not on c++
	 * @param screenPos Point to calculate direction from center of the screen
	 * @param bNegate Should we negate the direction because 0,0 is top left? true by default
	 * @param bNormalize Should we normalize the direction?
	 * @return The direction from center of the screen to given screenPos
	 */
	UFUNCTION(BlueprintPure, meta=(WorldContext="context"))
	static FVector2D GetScreenCenterToScreenPosDir(UObject* context, FVector2D screenPos, bool bNegate = true, bool bNormalize = true);
};
