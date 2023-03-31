// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "Ability/ArcherAbilitySystemComponent.h"
#include "GameFramework/Character.h"
#include "ArcherCharacter.generated.h"

class UArcherAbilitySet;
class UArcherCombatSet;
class UArcherHealthSet;
class UArcherHealthComponent;
struct FInputActionValue;
class UInputConfig;

/**
 * AArcherCharacter
 *
 * Base class of all the characters used in the game.
 */
UCLASS()
class ARCHERGAME_API AArcherCharacter : public ACharacter, public IAbilitySystemInterface, public IGameplayTagAssetInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AArcherCharacter();


	UPROPERTY(EditAnywhere, Category="ArcherCharacter|Ability")
	TObjectPtr<UArcherAbilitySet> AbilitySet;

	// Ability system interface getter.
	FORCEINLINE virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override { return AbilitySystemComponent; }

	UFUNCTION(BlueprintCallable)
	UArcherAbilitySystemComponent* GetArcherAbilitySystemComponent() const { return AbilitySystemComponent; }

private:
	bool bAbilitySystemInitted = false;

protected:
	// The gameplay ability system brain.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="ArcherCharacter|Ability")
	UArcherAbilitySystemComponent* AbilitySystemComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ArcherCharacter|Character", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UArcherHealthComponent> HealthComponent;

	// Init ability system component
	virtual void PossessedBy(AController* NewController) override;
	virtual void InitializeAbilitySystem();

public:
	// Start of IGameplayTagAssetInterface
	virtual void GetOwnedGameplayTags(FGameplayTagContainer& TagContainer) const override;
	virtual bool HasMatchingGameplayTag(FGameplayTag TagToCheck) const override;
	virtual bool HasAllMatchingGameplayTags(const FGameplayTagContainer& TagContainer) const override;
	virtual bool HasAnyMatchingGameplayTags(const FGameplayTagContainer& TagContainer) const override;
	// End of IGameplayTagAssetInterface
};
