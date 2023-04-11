// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "ArcherInputKeyNamesDataAsset.generated.h"

class UInputAction;

/*
 * FCustomKeyName
 *
 * Struct that holds information about key and its custom name
 */
USTRUCT()
struct FCustomKeyName
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	FKey Key;

	UPROPERTY(EditAnywhere)
	FText KeyName;
};

/**
 * UArcherInputKeyNamesDataAsset
 *
 * Provides customized key names
 */
UCLASS(BlueprintType)
class ARCHERGAME_API UArcherInputKeyNamesDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	UArcherInputKeyNamesDataAsset();

	UFUNCTION(BlueprintCallable)
	FText GetKeyNameFromKey(const FKey& Key);

private:
	UPROPERTY(EditDefaultsOnly, meta=(TitleProperty="Key"))
	TArray<FCustomKeyName> CustomKeyNames;
};
