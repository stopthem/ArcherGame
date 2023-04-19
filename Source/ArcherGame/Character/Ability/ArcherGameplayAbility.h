// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "ArcherGameplayAbility.generated.h"

class UGameplayMessageSubsystem;

/*
 * FMessageTagInfo
 *
 * Struct that holds a gameplay tag about the message and should it be broadcasted.
 */
USTRUCT(BlueprintType)
struct FMessageTagInfo
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bShouldBroadcast = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(EditCondition = "bShouldBroadcast == true"))
	FGameplayTag MessageTag;
};

/*
 * FMessageTagInfoHolder
 *
 * Holder of special message infos.
 */
USTRUCT(BlueprintType)
struct FMessageTagInfoHolder
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Archer Gameplay Ability|Message")
	FMessageTagInfo CanActivateMessageInfo;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Archer Gameplay Ability|Message")
	FMessageTagInfo CooldownMessageInfo;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Archer Gameplay Ability|Message")
	FMessageTagInfo IsActiveMessageInfo;
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

	// Holder of the messages that this ability can broadcast
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Archer Gameplay Ability|Message")
	FMessageTagInfoHolder MessageTagInfoHolder;

	EArcherAbilityActivationPolicy GetActivationPolicy() const { return ActivationPolicy; }

public:
	UPROPERTY(EditAnywhere, Category="Archer Gameplay Ability|Sound")
	bool bShouldPlayCantActivateSoundEffects = false;

	UPROPERTY(EditAnywhere, Category="Archer Gameplay Ability|Sound")
	TSubclassOf<UGameplayAbility> OverridenLookCostForSoundEffect;
	UPROPERTY(EditAnywhere, Category="Archer Gameplay Ability|Sound")
	TSubclassOf<UGameplayAbility> OverridenLookCooldownForSoundEffect;
	UPROPERTY(EditAnywhere, Category="Archer Gameplay Ability|Sound")
	TSubclassOf<UGameplayAbility> OverridenLookCanActivateForSoundEffect;

public:
	UFUNCTION(BlueprintCallable)
	class AArcherPlayerCharacter* GetArcherPlayerCharacterFromActorInfo() const;

	UFUNCTION(BlueprintCallable)
	class AArcherCharacter* GetArcherCharacterFromActorInfo() const;

	UFUNCTION(BlueprintCallable)
	void AddRemoveLooseGameplayTags(bool bAdd, FGameplayTag gameplayTag, int count = 1);

public:
	void TryActivateAbilityOnSpawn(const FGameplayAbilityActorInfo* actorInfo, const FGameplayAbilitySpec& spec) const;

#pragma region UGameplayAbility Interface
	// Broadcast can activate ability if we want to
	virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags,
	                                FGameplayTagContainer* OptionalRelevantTags) const override;

	// Broadcast cooldown if we want to
	virtual bool CommitAbility(const FGameplayAbilitySpecHandle handle, const FGameplayAbilityActorInfo* actorInfo, const FGameplayAbilityActivationInfo activationInfo, FGameplayTagContainer* optionalRelevantTags) override;

	// Make and return our custom effect context with useful informations
	virtual FGameplayEffectContextHandle MakeEffectContext(const FGameplayAbilitySpecHandle handle, const FGameplayAbilityActorInfo* actorInfo) const override;

	// Specialized version to handle death exclusion and AbilityTags expansion via ASC
	virtual bool DoesAbilitySatisfyTagRequirements(const UAbilitySystemComponent& AbilitySystemComponent, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags,
	                                               FGameplayTagContainer* OptionalRelevantTags) const override;

	// Broadcast is active
	virtual void EndAbility(const FGameplayAbilitySpecHandle handle, const FGameplayAbilityActorInfo* actorInfo, const FGameplayAbilityActivationInfo activationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
#pragma endregion

protected:
	// Defines how this ability is meant to activate.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Archer Gameplay Ability")
	EArcherAbilityActivationPolicy ActivationPolicy = EArcherAbilityActivationPolicy::Manual;

#pragma region Broadcasting Messages

private:
	// Broadcast can activate ability with Gameplay Message plugin.
	void BroadcastCanActivate(const FGameplayAbilityActorInfo* actorInfo, const bool bCanActivate) const;

	// Broadcast is ability active with Gameplay Message plugin.
	void BroadcastIsActive(const bool bIsAbilityActive) const;

public:
	// Broadcast cooldown duration with Gameplay Message plugin.
	UFUNCTION(BlueprintCallable)
	void BroadcastCooldown() const;
#pragma endregion
};
