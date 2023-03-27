// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "InputConfig.generated.h"

class UInputAction;

/**
 * FTaggedInputAction
 *
 * Struct used to map a input action to a gameplay input tag.
 */
USTRUCT()
struct FTaggedInputAction
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<const UInputAction> InputAction = nullptr;

	UPROPERTY(EditDefaultsOnly, Category="Input Tag")
	FGameplayTag InputTag;
};

/**
 * UInputConfig
 *
 * Non-mutable data asset that contains input configuration properties.
 */
UCLASS()
class ARCHERGAME_API UInputConfig : public UDataAsset
{
	GENERATED_BODY()

public:
	// Returns the first Input Action associated with a given tag.
	const UInputAction* FindInputActionForTag(const FGameplayTag& inputTag) const;

	// List of native input actions used by the owner. These input actions are mapped to a gameplay tag and must be manually bound.
	UPROPERTY(EditDefaultsOnly, meta=(TitleProperty = "InputAction"))
	TArray<FTaggedInputAction> NativeInputActions;

	// List of ability input actions used by the owner. These input actions are mapped to a gameplay tag and must be manually bound.
	UPROPERTY(EditDefaultsOnly, meta=(TitleProperty = "InputAction"))
	TArray<FTaggedInputAction> AbilityInputActions;
};
