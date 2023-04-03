// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "ArcherAbilitySet.generated.h"

class UDataTable;
class UAttributeSet;
class UArcherAbilitySystemComponent;
class UArcherGameplayEffect;
class UArcherGameplayAbility;
/**
 * FArcherAbilitySet_GameplayAbility
 *
 *	Data used by the ability set to grant gameplay abilities.
 */
USTRUCT(BlueprintType)
struct FArcherAbilitySet_GameplayAbility
{
	GENERATED_BODY()

public:
	// Gameplay ability to grant.
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UArcherGameplayAbility> Ability = nullptr;

	// Level of ability to grant.
	UPROPERTY(EditDefaultsOnly)
	int32 AbilityLevel = 1;

	// Tag used to process input for the ability.
	UPROPERTY(EditDefaultsOnly, Meta = (Categories = "InputTag"))
	FGameplayTag InputTag;
};

/**
 * FArcherAbilitySet_GameplayEffect
 *
 *	Data used by the ability set to grant gameplay effects.
 */
USTRUCT(BlueprintType)
struct FArcherAbilitySet_GameplayEffect
{
	GENERATED_BODY()

public:
	// Gameplay effect to grant.
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UArcherGameplayEffect> GameplayEffect = nullptr;

	// Level of gameplay effect to grant.
	UPROPERTY(EditDefaultsOnly)
	float EffectLevel = 1.0f;
};

/**
 * FArcherAbilitySet_AttributeSet
 *
 *	Data used by the ability set to grant attribute sets.
 */
USTRUCT(BlueprintType)
struct FArcherAbilitySet_AttributeSet
{
	GENERATED_BODY()

public:
	// Gameplay effect to grant.
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UAttributeSet> AttributeSet;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UDataTable> DefaultStartingDataTable;
};

/**
 * UArcherAbilitySet
 *
 * Non-mutable data asset used to grant gameplay abilities and gameplay effects.
 */
UCLASS()
class ARCHERGAME_API UArcherAbilitySet : public UDataAsset
{
	GENERATED_BODY()

public:
	UArcherAbilitySet(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	// called at PossessedBy from ArcherCharacter for granting abilities,effects and attributes
	void GrantSetAbilityEffectAttributes(UArcherAbilitySystemComponent* ArcherASC);

	// searches granted gameplay abilities for given ability and returns its InputTag
	UFUNCTION(BlueprintCallable)
	FGameplayTag GetInputTagFromAbility(const TSubclassOf<UArcherGameplayAbility> gameplayAbility);

protected:
	// Gameplay abilities to grant when this ability set is granted.
	UPROPERTY(EditDefaultsOnly, Category = "Gameplay Abilities", meta=(TitleProperty=Ability))
	TArray<FArcherAbilitySet_GameplayAbility> GrantedGameplayAbilities;

	// Gameplay effects to grant when this ability set is granted.
	UPROPERTY(EditDefaultsOnly, Category = "Gameplay Effects", meta=(TitleProperty=GameplayEffect))
	TArray<FArcherAbilitySet_GameplayEffect> GrantedGameplayEffects;

	// Attribute sets to grant when this ability set is granted.
	UPROPERTY(EditDefaultsOnly, Category = "Attribute Sets", meta=(TitleProperty=AttributeSet))
	TArray<FArcherAbilitySet_AttributeSet> GrantedAttributes;
};
