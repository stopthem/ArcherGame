// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DebugHUD.h"
#include "ArcherDebugHUD.generated.h"

/**
 * 
 */
UCLASS()
class ARCHERGAME_API AArcherDebugHUD : public ADebugHUD
{
	GENERATED_BODY()

protected:
	virtual void DrawHUD() override;
};
