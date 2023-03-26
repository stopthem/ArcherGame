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
struct FDirectionalAnimInfo
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UAnimMontage> AnimMontage = nullptr;

	UPROPERTY(EditDefaultsOnly)
	float Angle = 0;
};

/**
 * UDirectionalAnimationDataAsset
 *
 * Data asset class that holds directional animations
 */
UCLASS()
class ARCHERGAME_API UDirectionalAnimationDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	UDirectionalAnimationDataAsset();

	// List of directional anim infos will be used by Damaged Gameplay Ability.
	UPROPERTY(EditDefaultsOnly)
	TArray<FDirectionalAnimInfo> DirectionalAnimInfos;

	// Get the closest animation montage from given deltaAngle
	UFUNCTION(BlueprintCallable)
	UAnimMontage* GetAnimFromDeltaAngle(float deltaAngle) const;
};
