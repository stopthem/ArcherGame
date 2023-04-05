// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimBlueprintFunctionLibrary.h"

float UAnimBlueprintFunctionLibrary::GetPlayRateFromTime(const UAnimMontage* animMontage, float wantedTime)
{
	check(animMontage);

	return animMontage->GetPlayLength() / wantedTime;
}
