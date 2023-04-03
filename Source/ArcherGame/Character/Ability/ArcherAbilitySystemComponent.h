// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "GameplayEffectExtension.h"
#include "ArcherAbilitySystemComponent.generated.h"

/**
 * 
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
	// When a ability action is performed these get called for abilities
	virtual void AbilityInputPressed(const FGameplayTag& InputTag);
	virtual void AbilityInputReleased(const FGameplayTag& InputTag);

protected:
	virtual void AbilitySpecInputPressed(FGameplayAbilitySpec& Spec) override;
	virtual void AbilitySpecInputReleased(FGameplayAbilitySpec& Spec) override;

	// Handles to abilities that had their input pressed this frame.
	TArray<FGameplayAbilitySpecHandle> InputPressedSpecHandles;

	// Handles to abilities that had their input released this frame.
	TArray<FGameplayAbilitySpecHandle> InputReleasedSpecHandles;

	// Handles to abilities that have their input held.
	TArray<FGameplayAbilitySpecHandle> InputHeldSpecHandles;

public:
	// PlayerController calls this and handles WaitInput ability tasks
	void ProcessAbilityInput(float DeltaTime, bool bGamePaused);
	//Clears input spec handles
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

	void TryActivateAbilitiesOnSpawn();
};
