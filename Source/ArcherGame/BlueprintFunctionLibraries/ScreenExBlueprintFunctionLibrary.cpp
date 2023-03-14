// Fill out your copyright notice in the Description page of Project Settings.


#include "ScreenExBlueprintFunctionLibrary.h"

#include "VectorExBlueprintFunctionLibrary.h"

FVector2D UScreenExBlueprintFunctionLibrary::GetScreenCenterToScreenPosDir(UObject* context, FVector2D screenPos, bool bNegate, bool bNormalize)
{
	FVector2D viewportSize;

	// get viewport size
	context->GetWorld()->GetGameViewport()->GetViewportSize(viewportSize);

	// get the center of the screen which is size / 2
	const FVector2D screenCenter = viewportSize / 2;

	// get direction from center to given screenPos
	FVector centerToScreenPosDir = UVectorExBlueprintFunctionLibrary::GetDirection(FVector(screenCenter.X, screenCenter.Y, 0), FVector(screenPos.X, screenPos.Y, 0), bNormalize);

	// Negate because viewport starts at top left
	if (bNegate)
	{
		centerToScreenPosDir *= -1;
	}

	return FVector2D(centerToScreenPosDir.X, centerToScreenPosDir.Y);
}
