// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ArcherPlayerController.generated.h"

/**
 * AArcherPlayerController
 *
 * The class that player uses for post processing input
 */
UCLASS()
class ARCHERGAME_API AArcherPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AArcherPlayerController();

protected:
	virtual void PostProcessInput(const float DeltaTime, const bool bGamePaused) override;
};
