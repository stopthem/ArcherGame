// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "GameplayEffectExtension.h"
#include "ArcherAbilitySystemComponent.generated.h"

class UArcherAbilityRelationshipDataAsset;
class AArcherCharacter;
class UArcherAbilityTagRelationshipMapping;
/**
 * UArcherAbilitySystemComponent
 *
 * Base ability system component that every ability system owner uses.
 */
UCLASS()
class ARCHERGAME_API UArcherAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:
	UArcherAbilitySystemComponent();

	static AActor* GetInstigatorFromAttrChangeData(const FOnAttributeChangeData& ChangeData)
	{
		if (ChangeData.GEModData != nullptr)
		{
			const FGameplayEffectContextHandle& EffectContext = ChangeData.GEModData->EffectSpec.GetEffectContext();
			return EffectContext.GetOriginalInstigator();
		}

		return nullptr;
	}

	// Default abilities that will be given at OnPossessed
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(DisplayName ="Default Character Abilities"), Category= "Setup")
	TArray<TSubclassOf<class UArcherGameplayAbility>> DefaultAbilities;

	// Default effects that will be given at OnPossessed
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(DisplayName ="Default Character Effects"), Category= "Setup")
	TArray<TSubclassOf<class UArcherGameplayEffect>> DefaultEffects;

	virtual void InitAbilityActorInfo(AActor* InOwnerActor, AActor* InAvatarActor) override;

public:
	virtual void OnGiveAbility(FGameplayAbilitySpec& AbilitySpec) override;
	virtual void ApplyAbilityBlockAndCancelTags(const FGameplayTagContainer& AbilityTags, UGameplayAbility* RequestingAbility, bool bEnableBlockTags, const FGameplayTagContainer& BlockTags, bool bExecuteCancelTags,
	                                            const FGameplayTagContainer& CancelTags) override;
	// Looks at ability tags and gathers additional required and blocking tags
	void GetAdditionalActivationTagRequirements(const FGameplayTagContainer& AbilityTags, FGameplayTagContainer& OutActivationRequired, FGameplayTagContainer& OutActivationBlocked) const;

	UPROPERTY(BlueprintReadOnly, Category="Archer Ability System Component|Abilities With Widgets")
	TArray<TObjectPtr<UArcherGameplayAbility>> ArcherAbilitiesWithCostBroadcast;

	UPROPERTY(EditAnywhere, Category="Archer Ability System Component")
	TObjectPtr<UArcherAbilityRelationshipDataAsset> AbilityRelationshipDataAsset;

	UFUNCTION(BlueprintCallable)
	void CheckCanActivateBroadcasterArcherAbilities();

public:
	// When a ability action is performed these get called for abilities
	virtual void AbilityInputPressed(const FGameplayTag& InputTag);
	virtual void AbilityInputReleased(const FGameplayTag& InputTag);

protected:
	virtual void AbilitySpecInputPressed(FGameplayAbilitySpec& Spec) override;
	virtual void AbilitySpecInputReleased(FGameplayAbilitySpec& Spec) override;

	UPROPERTY(EditAnywhere, Category="ArcherAbilitySystemComponent|Tag Relationship")
	TObjectPtr<UArcherAbilityTagRelationshipMapping> TagRelationshipMapping;

	// Handles to abilities that had their input pressed this frame.
	TArray<FGameplayAbilitySpecHandle> InputPressedSpecHandles;

	// Handles to abilities that had their input released this frame.
	TArray<FGameplayAbilitySpecHandle> InputReleasedSpecHandles;

	// Handles to abilities that have their input held.
	TArray<FGameplayAbilitySpecHandle> InputHeldSpecHandles;

public:
	// PlayerController calls this and handles WaitInput ability tasks
	void ProcessAbilityInput(float DeltaTime, bool bGamePaused);

	// Clears input spec handles
	void ClearAbilityInput();

public:
	UFUNCTION(BlueprintCallable)
	bool FindAbilitySpecHandleFromTag(FGameplayTag abilityTag, FGameplayAbilitySpecHandle& out_gameplayAbilitySpecHandle);

	// Is found ability with given tag is active?
	bool IsAbilityActiveWithTag(FGameplayTag inputTag);

	// Get all activatable ability's AbilityTag and put them on a container
	UFUNCTION(BlueprintCallable)
	FGameplayTagContainer GetActivatableAbilitiesTagAsContainer();

private:
	// returns if a ability spec is found from given tag
	bool FindAbilitySpecFromInputTag(FGameplayTag inputTag, FGameplayAbilitySpec& out_gameplayAbilitySpec);

	bool FindAbilitySpecsFromInputTag(FGameplayTag inputTag, TArray<FGameplayAbilitySpec>& out_gameplayAbilitySpecs);

	bool FindAbilitySpecHandlesFromInputTag(FGameplayTag inputTag, TArray<FGameplayAbilitySpecHandle>& put_gameplayAbilitySpecHandles);

	void TryActivateAbilitiesOnSpawn();

	bool FindAbilitySpecFromTag(FGameplayTag abilityTag, FGameplayAbilitySpec& out_abilitySpec);

public:
	UPROPERTY(EditAnywhere, Category="ArcherAbilitySystemComponent|Sound")
	TObjectPtr<USoundBase> NoReasonCantActivateSound;

	UPROPERTY(EditAnywhere, Category="ArcherAbilitySystemComponent|Sound")
	TObjectPtr<USoundBase> CooldownCantActivateSound;

	UPROPERTY(EditAnywhere, Category="ArcherAbilitySystemComponent|Sound")
	TObjectPtr<USoundBase> CostCantActivateSound;

private:
	UPROPERTY()
	TObjectPtr<AArcherCharacter> ArcherCharacter;

	void HandleCantActivateSoundEffects(const FGameplayAbilitySpecHandle handle, const FGameplayAbilityActorInfo* actorInfo, OUT FGameplayTagContainer* optionalRelevantTags);

protected:
	virtual void BeginPlay() override;
};
