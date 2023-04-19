// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "ArcherAbilityRelationshipDataAsset.generated.h"

/*
 * FAbilityRelationshipInfo
 *
 * Struct that holds information about an ability's relationships.
 */
USTRUCT()
struct FAbilityRelationshipInfo
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly)
	FGameplayTag AbilityTag;

	UPROPERTY(EditDefaultsOnly)
	FGameplayTag CanActivateAbilityTag;

	UPROPERTY(EditDefaultsOnly)
	FGameplayTag CheckCostAbilityTag;

	UPROPERTY(EditDefaultsOnly)
	FGameplayTag CheckCooldownAbilityTag;
};

/**
 * UArcherAbilityRelationshipDataAsset
 *
 * Class that holds information about various ability relationships.
 */
UCLASS()
class ARCHERGAME_API UArcherAbilityRelationshipDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	UArcherAbilityRelationshipDataAsset();

	bool GetAbilityRelationshipFromTagContainer(const FGameplayTagContainer& tagContainer, FAbilityRelationshipInfo& out_abilityRelationshipInfo);

protected:
	UPROPERTY(EditAnywhere, meta=(TitleProperty="AbilityTag"))
	TArray<FAbilityRelationshipInfo> AbilityRelationshipInfos;
};
