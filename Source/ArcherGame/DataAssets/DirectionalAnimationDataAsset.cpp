// Fill out your copyright notice in the Description page of Project Settings.


#include "DirectionalAnimationDataAsset.h"

#include "Kismet/KismetMathLibrary.h"

UDirectionalAnimationDataAsset::UDirectionalAnimationDataAsset()
{
}

FDirectionalAnimSet UDirectionalAnimationDataAsset::GetClosestAngleAnimSet(const float deltaAngle) const
{
	const float deltaAngleNormalized = UKismetMathLibrary::NormalizeToRange(deltaAngle, -180, 180);

	auto GetNormalizedDistanceToDeltaAngleNormalized = [&](float angle)
	{
		return FMath::Abs(deltaAngleNormalized - UKismetMathLibrary::NormalizeToRange(angle, -180, 180));
	};

	// create new TArray for sorting
	TArray<FDirectionalAnimSet> sortedDirectionalAnimInfos = TArray(DirectionalAnimSets);
	// sort by distance to deltaAngleNormalized
	sortedDirectionalAnimInfos.Sort([&](const FDirectionalAnimSet directionalAnimInfo1, const FDirectionalAnimSet directionalAnimInfo2)
	{
		return GetNormalizedDistanceToDeltaAngleNormalized(directionalAnimInfo1.Angle) < GetNormalizedDistanceToDeltaAngleNormalized(directionalAnimInfo2.Angle);
	});

	return sortedDirectionalAnimInfos[0];
}

UAnimMontage* UDirectionalAnimationDataAsset::GetMontageFromDeltaAngle(float deltaAngle)
{
	return GetClosestAngleAnimSet(deltaAngle).AnimMontages[0];
}

UAnimMontage* UDirectionalAnimationDataAsset::GetRandomMontageFromDeltaAngle(float deltaAngle)
{
	FDirectionalAnimSet directionalAnimSet = GetClosestAngleAnimSet(deltaAngle);
	return directionalAnimSet.AnimMontages[FMath::RandRange(0, directionalAnimSet.AnimMontages.Num() - 1)];
}
