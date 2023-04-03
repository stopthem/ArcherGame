// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayCueManager.h"
#include "ArcherGameplayCueManager.generated.h"

/**
 * 
 */
UCLASS()
class ARCHERGAME_API UArcherGameplayCueManager : public UGameplayCueManager
{
	GENERATED_BODY()

public:
	UArcherGameplayCueManager(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	static UArcherGameplayCueManager* Get();

#pragma region UGameplayCueManager Interface
	// We don't want unreal to load every cue and its dependent properties, that can be costly.
	// we want it to load on demand which on the first time sometimes waiting a tick is good.
	virtual bool ShouldAsyncLoadRuntimeObjectLibraries() const override;
#pragma endregion
};
