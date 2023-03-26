// Fill out your copyright notice in the Description page of Project Settings.


#include "DirectionalAnimationDataAsset.h"

#include "Kismet/KismetMathLibrary.h"

UDirectionalAnimationDataAsset::UDirectionalAnimationDataAsset()
{
}

UAnimMontage* UDirectionalAnimationDataAsset::GetAnimFromDeltaAngle(float deltaAngle) const
{
	const float deltaAngleNormalized = UKismetMathLibrary::NormalizeToRange(deltaAngle, -180, 180);

	auto GetDistanceToDeltaAngleNormalized = [&](float angle)
	{
		return FMath::Abs(deltaAngleNormalized - UKismetMathLibrary::NormalizeToRange(angle, -180, 180));
	};

	// create new TArray for sorting
	TArray<FDirectionalAnimInfo> sortedDirectionalAnimInfos = TArray(DirectionalAnimInfos);
	// sort by distance to deltaAngleNormalized
	sortedDirectionalAnimInfos.Sort([&](const FDirectionalAnimInfo directionalAnimInfo1, const FDirectionalAnimInfo directionalAnimInfo2)
	{
		return GetDistanceToDeltaAngleNormalized(directionalAnimInfo1.Angle) < GetDistanceToDeltaAngleNormalized(directionalAnimInfo2.Angle);
	});

	// return first of the sorted array
	return sortedDirectionalAnimInfos[0].AnimMontage;
}
