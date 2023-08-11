// Fill out your copyright notice in the Description page of Project Settings.


#include "ArcherAbilitySystemComponent.h"

#include "ArcherAbilityTagRelationshipMapping.h"
#include "ArcherGameplayAbility.h"
#include "ArcherGameplayEffect.h"
#include "Abilities/Async/AbilityAsync_WaitGameplayEvent.h"
#include "ArcherGame/BlueprintFunctionLibraries/ArrayBlueprintFunctionLibrary.h"
#include "ArcherGame/Character/ArcherCharacter.h"
#include "ArcherGame/DataAssets/ArcherAbilityRelationshipDataAsset.h"
#include "Attribute/ArcherHealthSet.h"

UArcherAbilitySystemComponent::UArcherAbilitySystemComponent()
{
}

void UArcherAbilitySystemComponent::CheckCanActivateBroadcasterArcherAbilities()
{
	for (const UArcherGameplayAbility* archerAbilitiesWithCostBroadcast : ArcherAbilitiesWithCostBroadcast)
	{
		if (AbilityActorInfo == nullptr)
		{
			continue;
		}

		FGameplayAbilitySpecHandle specHandle;
		FindAbilitySpecHandleFromTag(archerAbilitiesWithCostBroadcast->AbilityTags.First(), specHandle);
		archerAbilitiesWithCostBroadcast->CanActivateAbility(specHandle, AbilityActorInfo.Get(), &GameplayTagCountContainer.GetExplicitGameplayTags(), nullptr, nullptr);
	}
}

void UArcherAbilitySystemComponent::AbilityInputPressed(const FGameplayTag& InputTag)
{
	if (TArray<FGameplayAbilitySpecHandle> foundGameplayAbilitySpecHandles; FindAbilitySpecHandlesFromInputTag(InputTag, foundGameplayAbilitySpecHandles))
	{
		UArrayBlueprintFunctionLibrary::AppendUnique(foundGameplayAbilitySpecHandles, InputPressedSpecHandles);
		UArrayBlueprintFunctionLibrary::AppendUnique(foundGameplayAbilitySpecHandles, InputHeldSpecHandles);
	}
}

void UArcherAbilitySystemComponent::AbilityInputReleased(const FGameplayTag& InputTag)
{
	if (TArray<FGameplayAbilitySpecHandle> foundGameplayAbilitySpecHandles; FindAbilitySpecHandlesFromInputTag(InputTag, foundGameplayAbilitySpecHandles))
	{
		UArrayBlueprintFunctionLibrary::AppendUnique(foundGameplayAbilitySpecHandles, InputReleasedSpecHandles);

		InputHeldSpecHandles.RemoveAll([&](const FGameplayAbilitySpecHandle& gameplayAbilitySpecHandle)
		{
			return foundGameplayAbilitySpecHandles.Contains(gameplayAbilitySpecHandle);
		});
	}
}

void UArcherAbilitySystemComponent::OnGiveAbility(FGameplayAbilitySpec& AbilitySpec)
{
	Super::OnGiveAbility(AbilitySpec);

	UArcherGameplayAbility* archerGameplayAbility = Cast<UArcherGameplayAbility>(AbilitySpec.Ability);
	if (archerGameplayAbility && archerGameplayAbility->MessageTagInfoHolder.CanActivateMessageInfo.bShouldBroadcast)
	{
		ArcherAbilitiesWithCostBroadcast.Add(archerGameplayAbility);
	}
}

void UArcherAbilitySystemComponent::ApplyAbilityBlockAndCancelTags(const FGameplayTagContainer& AbilityTags, UGameplayAbility* RequestingAbility, bool bEnableBlockTags, const FGameplayTagContainer& BlockTags, bool bExecuteCancelTags,
                                                                   const FGameplayTagContainer& CancelTags)
{
	FGameplayTagContainer modifiedBlockTags = BlockTags;
	FGameplayTagContainer modifiedCancelTags = CancelTags;

	if (TagRelationshipMapping)
	{
		TagRelationshipMapping->GetAbilityTagsToBlockAndCancel(AbilityTags, &modifiedBlockTags, &modifiedCancelTags);
	}

	Super::ApplyAbilityBlockAndCancelTags(AbilityTags, RequestingAbility, bEnableBlockTags, modifiedBlockTags, bExecuteCancelTags, modifiedCancelTags);
}

void UArcherAbilitySystemComponent::GetAdditionalActivationTagRequirements(const FGameplayTagContainer& AbilityTags, FGameplayTagContainer& OutActivationRequired, FGameplayTagContainer& OutActivationBlocked) const
{
	if (TagRelationshipMapping)
	{
		TagRelationshipMapping->GetRequiredAndBlockedActivationTags(AbilityTags, &OutActivationRequired, &OutActivationBlocked);
	}
}

void UArcherAbilitySystemComponent::InitAbilityActorInfo(AActor* InOwnerActor, AActor* InAvatarActor)
{
	Super::InitAbilityActorInfo(InOwnerActor, InAvatarActor);

	TryActivateAbilitiesOnSpawn();
}

void UArcherAbilitySystemComponent::TryActivateAbilitiesOnSpawn()
{
	ABILITYLIST_SCOPE_LOCK()
	for (const FGameplayAbilitySpec& gameplayAbilitySpec : ActivatableAbilities.Items)
	{
		const UArcherGameplayAbility* gameplayAbilityCDO = CastChecked<UArcherGameplayAbility>(gameplayAbilitySpec.Ability);
		gameplayAbilityCDO->TryActivateAbilityOnSpawn(AbilityActorInfo.Get(), gameplayAbilitySpec);
	}
}

void UArcherAbilitySystemComponent::HandleCantActivateSoundEffects(const FGameplayAbilitySpecHandle handle, const FGameplayAbilityActorInfo* actorInfo, FGameplayTagContainer* optionalRelevantTags)
{
	check(AbilityRelationshipDataAsset);

	UGameplayAbility* ability = FindAbilitySpecFromHandle(handle)->Ability;
	const UArcherGameplayAbility* archerGameplayAbility = Cast<UArcherGameplayAbility>(ability);

	if (!archerGameplayAbility->bShouldPlayCantActivateSoundEffects)
	{
		return;
	}

	FAbilityRelationshipInfo foundAbilityRelationshipInfo;

	const bool bFoundAbilityRelationship = AbilityRelationshipDataAsset->GetAbilityRelationshipFromTagContainer(ability->AbilityTags, foundAbilityRelationshipInfo);

	auto GetToCheckAbility = [&](const FGameplayTag abilityTag)
	{
		// Given tag is found within foundAbilityRelationshipInfo, so if that is null tag will be invalid.
		if (abilityTag.IsValid())
		{
			FGameplayAbilitySpec foundSpec;
			FindAbilitySpecFromTag(abilityTag, foundSpec);

			// Lambda return requires that all returns are same type.
			// Can't do UGameplayAbility* and TObjectPtr<UGameplayAbility>
			return foundSpec.Ability.Get();
		}

		return ability;
	};

	// We get cooldown and cost abilities to check separately because
	// there are some finishing abilities that they have a another ability to ready them up.
	// in this case we use finishing abilitiy's cooldown for readying ability because
	// readying ability is activated and its cooldown will be wrong to play a cue.
	const UGameplayAbility* abilityToCheckCooldown = ability;
	const UGameplayAbility* abilityToCheckCost = ability;
	if (bFoundAbilityRelationship)
	{
		abilityToCheckCooldown = GetToCheckAbility(foundAbilityRelationshipInfo.CheckCooldownAbilityTag);
		abilityToCheckCost = GetToCheckAbility(foundAbilityRelationshipInfo.CheckCostAbilityTag);
	}

	USoundBase* soundToPlay;

	if (!abilityToCheckCost->CheckCost(handle, actorInfo, optionalRelevantTags))
	{
		soundToPlay = CostCantActivateSound;
	}
	else if (!abilityToCheckCooldown->CheckCooldown(handle, actorInfo, optionalRelevantTags))
	{
		soundToPlay = CooldownCantActivateSound;
	}
	else
	{
		soundToPlay = NoReasonCantActivateSound;
	}

	ArcherCharacter->PlaySoundEffect(soundToPlay);
}

void UArcherAbilitySystemComponent::BeginPlay()
{
	Super::BeginPlay();

	ArcherCharacter = Cast<AArcherCharacter>(GetOwner());
}

void UArcherAbilitySystemComponent::ProcessAbilityInput(float DeltaTime, bool bGamePaused)
{
	static TArray<FGameplayAbilitySpecHandle> abilitiesToActivate;
	abilitiesToActivate.Reset();

	// Process all abilities that activate when the input is held.
	for (const FGameplayAbilitySpecHandle& specHandle : InputHeldSpecHandles)
	{
		if (const FGameplayAbilitySpec* abilitySpec = FindAbilitySpecFromHandle(specHandle))
		{
			if (abilitySpec->Ability && !abilitySpec->IsActive())
			{
				const UArcherGameplayAbility* archerGameplayAbility = CastChecked<UArcherGameplayAbility>(abilitySpec->Ability);

				if (archerGameplayAbility->GetActivationPolicy() == EArcherAbilityActivationPolicy::WhileInputActive)
				{
					abilitiesToActivate.AddUnique(abilitySpec->Handle);
				}
			}
		}
	}

	// Process all abilities that had their input pressed this frame.
	for (const FGameplayAbilitySpecHandle& specHandle : InputPressedSpecHandles)
	{
		if (FGameplayAbilitySpec* abilitySpec = FindAbilitySpecFromHandle(specHandle))
		{
			if (abilitySpec->Ability)
			{
				abilitySpec->InputPressed = true;

				if (abilitySpec->IsActive())
				{
					// Ability is active so pass along the input event.
					AbilitySpecInputPressed(*abilitySpec);
				}
				else
				{
					const UArcherGameplayAbility* archerGameplayAbility = CastChecked<UArcherGameplayAbility>(abilitySpec->Ability);

					if (archerGameplayAbility->GetActivationPolicy() == EArcherAbilityActivationPolicy::OnInputTriggered)
					{
						abilitiesToActivate.AddUnique(abilitySpec->Handle);
					}
				}
			}
		}
	}

	// Try to activate all the abilities that are from presses and holds.
	// We do it all at once so that held inputs don't activate the ability
	// and then also send a input event to the ability because of the press.
	for (const FGameplayAbilitySpecHandle& AbilitySpecHandle : abilitiesToActivate)
	{
		// If we couldn't activate the ability, play sound effects based on that.
		if (!TryActivateAbility(AbilitySpecHandle))
		{
			HandleCantActivateSoundEffects(AbilitySpecHandle, AbilityActorInfo.Get(), nullptr);
		}
	}

	// Process all abilities that had their input released this frame.
	for (const FGameplayAbilitySpecHandle& SpecHandle : InputReleasedSpecHandles)
	{
		if (FGameplayAbilitySpec* AbilitySpec = FindAbilitySpecFromHandle(SpecHandle))
		{
			if (AbilitySpec->Ability)
			{
				AbilitySpec->InputPressed = false;

				if (AbilitySpec->IsActive())
				{
					// Ability is active so pass along the input event.
					AbilitySpecInputReleased(*AbilitySpec);
				}
			}
		}
	}

	// Clear the cached ability handles.
	InputPressedSpecHandles.Reset();
	InputReleasedSpecHandles.Reset();
}

void UArcherAbilitySystemComponent::ClearAbilityInput()
{
	InputPressedSpecHandles.Reset();
	InputReleasedSpecHandles.Reset();
	InputHeldSpecHandles.Reset();
}

bool UArcherAbilitySystemComponent::FindAbilitySpecHandleFromTag(FGameplayTag abilityTag, FGameplayAbilitySpecHandle& out_gameplayAbilitySpecHandle)
{
	ABILITYLIST_SCOPE_LOCK()

	if (!abilityTag.IsValid())
	{
		return false;
	}


	// if we found a ability with the given tag try to activate it
	if (const auto foundGameplayAbilitySpec = ActivatableAbilities.Items.FindByPredicate([&](const FGameplayAbilitySpec& GameplayAbilitySpec)
	{
		return GameplayAbilitySpec.Ability && GameplayAbilitySpec.Ability->AbilityTags.HasTagExact(abilityTag);
	}))
	{
		out_gameplayAbilitySpecHandle = foundGameplayAbilitySpec->Handle;
		return true;
	}

	return false;
}

bool UArcherAbilitySystemComponent::IsAbilityActiveWithTag(FGameplayTag inputTag)
{
	FGameplayAbilitySpec foundAbilitySpec;
	FindAbilitySpecFromInputTag(inputTag, foundAbilitySpec);

	return foundAbilitySpec.Ability ? foundAbilitySpec.IsActive() : false;
}

FGameplayTagContainer UArcherAbilitySystemComponent::GetActivatableAbilitiesTagAsContainer()
{
	ABILITYLIST_SCOPE_LOCK()

	FGameplayTagContainer abilitiesTagContainer;

	for (const FGameplayAbilitySpec& gameplayAbilitySpec : ActivatableAbilities.Items)
	{
		if (gameplayAbilitySpec.Ability)
		{
			// we are only adding the first one because i don't plan to add a second tag to gameplay abilities
			abilitiesTagContainer.AppendTags(gameplayAbilitySpec.Ability->AbilityTags);
		}
	}

	return abilitiesTagContainer;
}

bool UArcherAbilitySystemComponent::FindAbilitySpecFromInputTag(FGameplayTag inputTag, FGameplayAbilitySpec& out_gameplayAbilitySpec)
{
	ABILITYLIST_SCOPE_LOCK()

	if (!inputTag.IsValid())
	{
		return false;
	}

	// if we found a ability with the given tag try to activate it
	if (const auto foundGameplayAbilitySpec = ActivatableAbilities.Items.FindByPredicate([&](const FGameplayAbilitySpec& GameplayAbilitySpec)
	{
		return GameplayAbilitySpec.Ability && GameplayAbilitySpec.DynamicAbilityTags.HasTagExact(inputTag);
	}))
	{
		out_gameplayAbilitySpec = *foundGameplayAbilitySpec;
		return true;
	}

	return false;
}

bool UArcherAbilitySystemComponent::FindAbilitySpecsFromInputTag(FGameplayTag inputTag, TArray<FGameplayAbilitySpec>& out_gameplayAbilitySpecs)
{
	ABILITYLIST_SCOPE_LOCK()

	if (!inputTag.IsValid())
	{
		return false;
	}

	for (const FGameplayAbilitySpec& gameplayAbilitySpec : ActivatableAbilities.Items)
	{
		if (gameplayAbilitySpec.Ability && gameplayAbilitySpec.DynamicAbilityTags.HasTagExact(inputTag))
		{
			out_gameplayAbilitySpecs.Add(gameplayAbilitySpec);
		}
	}

	return out_gameplayAbilitySpecs.Num() > 0;
}

bool UArcherAbilitySystemComponent::FindAbilitySpecHandlesFromInputTag(FGameplayTag inputTag, TArray<FGameplayAbilitySpecHandle>& out_gameplayAbilitySpecHandles)
{
	ABILITYLIST_SCOPE_LOCK()

	if (!inputTag.IsValid())
	{
		return false;
	}

	TArray<FGameplayAbilitySpec> gameplayAbilitySpecs;
	FindAbilitySpecsFromInputTag(inputTag, gameplayAbilitySpecs);

	for (const FGameplayAbilitySpec& gameplayAbilitySpec : gameplayAbilitySpecs)
	{
		out_gameplayAbilitySpecHandles.Add(gameplayAbilitySpec.Handle);
	}

	return out_gameplayAbilitySpecHandles.Num() > 0;
}

bool UArcherAbilitySystemComponent::FindAbilitySpecFromTag(FGameplayTag abilityTag, FGameplayAbilitySpec& out_abilitySpec)
{
	ABILITYLIST_SCOPE_LOCK()

	if (!abilityTag.IsValid())
	{
		return false;
	}

	FGameplayAbilitySpecHandle foundHandle;
	if (FindAbilitySpecHandleFromTag(abilityTag, foundHandle))
	{
		out_abilitySpec = *FindAbilitySpecFromHandle(foundHandle);
		return true;
	}

	UE_LOG(LogTemp, Warning, TEXT("Archer ability system component -> couldn't find handle in actor %s with tag %s"), *ArcherCharacter->GetActorNameOrLabel(), *abilityTag.GetTagName().ToString());
	return false;
}

void UArcherAbilitySystemComponent::AbilitySpecInputPressed(FGameplayAbilitySpec& Spec)
{
	Super::AbilitySpecInputPressed(Spec);

	// We don't support UGameplayAbility::bReplicateInputDirectly.
	// Use replicated events instead so that the WaitInputPress ability task works.
	if (Spec.IsActive())
	{
		// Invoke the InputPressed event. This is not replicated here. If someone is listening, they may replicate the InputPressed event to the server.
		InvokeReplicatedEvent(EAbilityGenericReplicatedEvent::InputPressed, Spec.Handle, Spec.ActivationInfo.GetActivationPredictionKey());
	}
}

void UArcherAbilitySystemComponent::AbilitySpecInputReleased(FGameplayAbilitySpec& Spec)
{
	Super::AbilitySpecInputReleased(Spec);

	// We don't support UGameplayAbility::bReplicateInputDirectly.
	// Use replicated events instead so that the WaitInputRelease ability task works.
	if (Spec.IsActive())
	{
		// Invoke the InputReleased event. This is not replicated here. If someone is listening, they may replicate the InputReleased event to the server.
		InvokeReplicatedEvent(EAbilityGenericReplicatedEvent::InputReleased, Spec.Handle, Spec.ActivationInfo.GetActivationPredictionKey());
	}
}
