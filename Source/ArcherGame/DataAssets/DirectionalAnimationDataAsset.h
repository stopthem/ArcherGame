// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "DirectionalAnimationDataAsset.generated.h"

/**
 * FDirectionalAnimInfo
 *
 * UStruct that holds directional animation info which is montage and its angle
 */
USTRUCT()
struct FDirectionalAnimSet
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly)
	TArray<TObjectPtr<UAnimMontage>> AnimMontages;

	UPROPERTY(EditDefaultsOnly)
	float Angle = 0;
};

/**
 * UDirectionalAnimationDataAsset
 *
 * Data asset class that holds directional animations
 */
UCLASS(BlueprintType)
class ARCHERGAME_API UDirectionalAnimationDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	UDirectionalAnimationDataAsset();

	// List of directional anim infos will be used by Damaged Gameplay Ability.
	UPROPERTY(EditDefaultsOnly)
	TArray<FDirectionalAnimSet> DirectionalAnimSets;

	// Get first montage from the closest animation set's angle
	UFUNCTION(BlueprintCallable)
	UAnimMontage* GetMontageFromDeltaAngle(const float deltaAngle);

	// Get random montage from the closest animation set's angle
	UFUNCTION(BlueprintCallable)
	UAnimMontage* GetRandomMontageFromDeltaAngle(const float deltaAngle);

private:
	FDirectionalAnimSet GetClosestAngleAnimSet(const float deltaAngle) const;
};
