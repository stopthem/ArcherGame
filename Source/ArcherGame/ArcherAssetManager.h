// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/AssetManager.h"
#include "ArcherAssetManager.generated.h"

/**
 * 
 */
UCLASS()
class ARCHERGAME_API UArcherAssetManager : public UAssetManager
{
	GENERATED_BODY()

public:
	// Returns the asset manager singleton object.
	static UArcherAssetManager& Get();

protected:
	virtual void StartInitialLoading() override;
};
