// Fill out your copyright notice in the Description page of Project Settings.


#include "ArcherAbilitySystemComponent.h"
#include "ArcherGameplayAbility.h"
#include "ArcherGameplayEffect.h"
#include "Abilities/Async/AbilityAsync_WaitGameplayEvent.h"
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
		// archerAbilitiesWithCostBroadcast->CheckCost(specHandle, AbilityActorInfo.Get(), nullptr);
	}
}

void UArcherAbilitySystemComponent::AbilityInputPressed(const FGameplayTag& InputTag)
{
	if (FGameplayAbilitySpec foundGameplayAbilitySpec; FindAbilitySpecFromInputTag(InputTag, foundGameplayAbilitySpec))
	{
		InputPressedSpecHandles.AddUnique(foundGameplayAbilitySpec.Handle);
		InputHeldSpecHandles.AddUnique(foundGameplayAbilitySpec.Handle);
	}
}

void UArcherAbilitySystemComponent::AbilityInputReleased(const FGameplayTag& InputTag)
{
	if (FGameplayAbilitySpec foundGameplayAbilitySpec; FindAbilitySpecFromInputTag(InputTag, foundGameplayAbilitySpec))
	{
		InputReleasedSpecHandles.AddUnique(foundGameplayAbilitySpec.Handle);
		InputHeldSpecHandles.Remove(foundGameplayAbilitySpec.Handle);
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

void UArcherAbilitySystemComponent::ProcessAbilityInput(float DeltaTime, bool bGamePaused)
{
	static TArray<FGameplayAbilitySpecHandle> abilitiesToActivate;
	abilitiesToActivate.Reset();

	//
	// Process all abilities that activate when the input is held.
	//
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

	//
	// Process all abilities that had their input pressed this frame.
	//
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

	//
	// Try to activate all the abilities that are from presses and holds.
	// We do it all at once so that held inputs don't activate the ability
	// and then also send a input event to the ability because of the press.
	//
	for (const FGameplayAbilitySpecHandle& AbilitySpecHandle : abilitiesToActivate)
	{
		TryActivateAbility(AbilitySpecHandle);
	}

	//
	// Process all abilities that had their input released this frame.
	//
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

	//
	// Clear the cached ability handles.
	//
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
	if (!abilityTag.IsValid())
	{
		return false;
	}


	// if we found a ability with the given tag try to activate it
	if (const auto foundGameplayAbilitySpec = ActivatableAbilities.Items.FindByPredicate([&](const FGameplayAbilitySpec GameplayAbilitySpec)
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
	FGameplayTagContainer abilitiesTagContainer;

	for (const FGameplayAbilitySpec gameplayAbilitySpec : ActivatableAbilities.Items)
	{
		if (gameplayAbilitySpec.Ability)
		{
			// we are only adding the first one because i don't plan to add a second tag to gameplay abilities
			abilitiesTagContainer.AddTag(gameplayAbilitySpec.Ability->AbilityTags.First());
		}
	}

	return abilitiesTagContainer;
}

bool UArcherAbilitySystemComponent::FindAbilitySpecFromInputTag(FGameplayTag inputTag, FGameplayAbilitySpec& out_gameplayAbilitySpec)
{
	if (!inputTag.IsValid())
	{
		return false;
	}


	// if we found a ability with the given tag try to activate it
	if (const auto foundGameplayAbilitySpec = ActivatableAbilities.Items.FindByPredicate([&](const FGameplayAbilitySpec GameplayAbilitySpec)
	{
		return GameplayAbilitySpec.Ability && GameplayAbilitySpec.DynamicAbilityTags.HasTagExact(inputTag);
	}))
	{
		out_gameplayAbilitySpec = *foundGameplayAbilitySpec;
		return true;
	}

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
