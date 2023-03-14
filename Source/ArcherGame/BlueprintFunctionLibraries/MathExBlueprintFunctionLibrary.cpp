// Fill out your copyright notice in the Description page of Project Settings.


#include "MathExBlueprintFunctionLibrary.h"

float UMathExBlueprintFunctionLibrary::GetAngleBetween(FVector aVector, FVector bVector)
{
	const float dot = aVector.Dot(bVector);
	return FMath::RadiansToDegrees(FMath::Acos(dot));
}

float UMathExBlueprintFunctionLibrary::GetSignedAngleBetween(FVector aVector, FVector bVector, FVector axisVector)
{
	const float angleBetween = GetAngleBetween(aVector, bVector);

	const FVector crossVector = aVector.Cross(bVector);

	return crossVector.Dot(axisVector) < 0 ? -angleBetween : angleBetween;
}
