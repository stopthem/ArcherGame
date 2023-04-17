// Fill out your copyright notice in the Description page of Project Settings.


#include "ArcherCameraMode_TopDown.h"

#include "Kismet/KismetMathLibrary.h"

UArcherCameraMode_TopDown::UArcherCameraMode_TopDown()
{
}

FVector UArcherCameraMode_TopDown::GetPivotLocation() const
{
	// If given values treated like local, transform FollowOffset if not add our FollowOffset to target location.
	return bFollowOffsetRelative ? UKismetMathLibrary::TransformLocation(GetTargetActor()->GetTransform(), FollowOffset) : GetTargetActor()->GetActorLocation() + FollowOffset;
}

FRotator UArcherCameraMode_TopDown::GetPivotRotation()
{
	// Return rotation from camera component to target actor.
	return UKismetMathLibrary::FindLookAtRotation(GetPivotLocation(), GetTargetActor()->GetActorLocation());
}
