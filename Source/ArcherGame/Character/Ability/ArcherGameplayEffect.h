// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffect.h"
#include "ArcherGameplayEffect.generated.h"

/**
 * UArcherGameplayEffect
 *
 * Base gameplay effect class that used in this game.
 */
UCLASS()
class ARCHERGAME_API UArcherGameplayEffect : public UGameplayEffect
{
	GENERATED_BODY()

public:
	UArcherGameplayEffect();

	// Level of the effect
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Setup")
	int32 EffectLevel = 1;
};
