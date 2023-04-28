// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ArcherGame/Camera/LyraCameraMode.h"
#include "ArcherCameraMode_TopDown.generated.h"

/**
 * UArcherGameMode_TopDown
 *
 * Main camera mode that this game uses.
 */
UCLASS(Abstract, Blueprintable)
class ARCHERGAME_API UArcherCameraMode_TopDown : public ULyraCameraMode
{
	GENERATED_BODY()

public:
	UArcherCameraMode_TopDown();

	// Should we treat given values like local?
	UPROPERTY(EditAnywhere, Category="TopDownCam|Follow")
	bool bFollowOffsetRelative = false;

	// Offset from the target actor.
	UPROPERTY(EditAnywhere, Category="TopDownCam|Follow")
	FVector FollowOffset;

protected:
#pragma region ULyraCameraMode Interface
	virtual FVector GetPivotLocation() override;
	virtual FRotator GetPivotRotation() override;
#pragma endregion
};
