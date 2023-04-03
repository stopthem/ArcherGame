// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "ArcherAbilityIconsPrimaryAsset.generated.h"

/*
 * FAbilityIconInfo
 *
 * Holds information about a abilities tag and its icon
 */
USTRUCT(BlueprintType)
struct FAbilityIconInfo
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(AllowedClasses="Texture"))
	TObjectPtr<UObject> AbilityIcon;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag AbilityTag;
};

/*
 * FPrioritizedAbilityIconInfos
 *
 * Holds information about spawning priority for widget and ability icon infos that of this priority
 */
USTRUCT(BlueprintType)
struct FPrioritizedAbilityIconInfos
{
	GENERATED_BODY()

public:
	// The priority for spawning to widget
	UPROPERTY(EditDefaultsOnly)
	int32 SpawnPriority;

	UPROPERTY(EditDefaultsOnly)
	TArray<FAbilityIconInfo> AbilityIconInfos;
};

/**
 * UArcherAbilityIconsPrimaryAsset
 *
 * Primary data asset that can be accessed from asset manager and provides information about ability icons
 */
UCLASS()
class ARCHERGAME_API UArcherAbilityIconsPrimaryAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UArcherAbilityIconsPrimaryAsset();

	// Return found ability icon info if found
	UFUNCTION(BlueprintCallable, Category="Ability Icons Asset")
	FAbilityIconInfo GetAbilityIconInfoFromTag(FGameplayTag abilityTag);

	// This gives the ability icon infos in a ordered spawn priority which is basically think of a moba game skill icons and the farthest left one has 0 priority then 1 and so on
	UFUNCTION(BlueprintCallable, Category="Ability Icons Asset")
	TArray<FAbilityIconInfo> GetOrderedAbilityIconInfosFromGameplayTagContainer(FGameplayTagContainer gameplayTagContainer);

private:
	UPROPERTY(EditDefaultsOnly)
	TArray<FPrioritizedAbilityIconInfos> PrioritizedAbilityIconInfosArray;
};
