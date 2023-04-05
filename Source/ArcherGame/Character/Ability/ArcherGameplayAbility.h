// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "ArcherGameplayAbility.generated.h"

class UGameplayMessageSubsystem;
USTRUCT(BlueprintType)
struct FMessageTagInfo
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bShouldBroadcast;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(EditCondition = "bShouldBroadcast == true"))
	FGameplayTag MessageTag;
};

USTRUCT(BlueprintType)
struct FMessageTagInfoHolder
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Archer Gameplay Ability|Message")
	FMessageTagInfo CanActivateMessageInfo;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Archer Gameplay Ability|Message")
	FMessageTagInfo CooldownMessageInfo;
};

/**
 * EArcherAbilityActivationPolicy
 *
 * Defines how a gameplay ability should be activated
 */
UENUM(BlueprintType)
enum class EArcherAbilityActivationPolicy : uint8
{
	// Try to activate the ability when the input is triggered.
	OnInputTriggered,

	// Continually try to activate the ability while the input is active.
	WhileInputActive,

	// Ability will be manually activated.
	Manual,

	// Ability will be activated on spawn
	OnSpawn
};


/**
 * UArcherGameplayAbility
 *
 * Base gameplay ability class used by this game.
 */
UCLASS(Abstract)
class ARCHERGAME_API UArcherGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:
	UArcherGameplayAbility();

	// Level of the ability
	// UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Ability Info")
	// int32 AbilityLevel = 1;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Archer Gameplay Ability|Message")
	FMessageTagInfoHolder MessageTagInfoHolder;

	EArcherAbilityActivationPolicy GetActivationPolicy() const { return ActivationPolicy; }

public:
	UFUNCTION(BlueprintCallable)
	class AArcherPlayerCharacter* GetArcherPlayerCharacterFromActorInfo() const;

	UFUNCTION(BlueprintCallable)
	class AArcherCharacter* GetArcherCharacterFromActorInfo() const;

	bool HasActorInfo() const
	{
		return CurrentActorInfo != nullptr;
	}

public:
	void TryActivateAbilityOnSpawn(const FGameplayAbilityActorInfo* actorInfo, const FGameplayAbilitySpec& spec) const;

#pragma region UGameplayAbility Interface
	// broadcast can activate ability if we want to
	virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags,
	                                FGameplayTagContainer* OptionalRelevantTags) const override;
	// broadcast cooldown if we want to
	virtual bool CommitAbility(const FGameplayAbilitySpecHandle handle, const FGameplayAbilityActorInfo* actorInfo, const FGameplayAbilityActivationInfo activationInfo, FGameplayTagContainer* optionalRelevantTags) override;
#pragma endregion

protected:
	// Defines how this ability is meant to activate.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Archer Gameplay Ability")
	EArcherAbilityActivationPolicy ActivationPolicy;

private:
	void BroadcastCanActivate(const FGameplayAbilityActorInfo* actorInfo, const bool bCanActivate) const;

	void BroadcastCooldown() const;
};
