// Fill out your copyright notice in the Description page of Project Settings.


#include "VectorExBlueprintFunctionLibrary.h"

FVector UVectorExBlueprintFunctionLibrary::GetDirection(FVector fromVector, FVector toVector, bool bNormalized)
{
	const FVector direction = toVector - fromVector;

	return bNormalized ? direction.GetSafeNormal() : direction;
}
