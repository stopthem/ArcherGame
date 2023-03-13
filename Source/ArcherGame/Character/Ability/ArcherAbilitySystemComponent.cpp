// Fill out your copyright notice in the Description page of Project Settings.


#include "ArcherAbilitySystemComponent.h"

#include "ArcherGameplayAbility.h"
#include "ArcherGameplayEffect.h"

UArcherAbilitySystemComponent::UArcherAbilitySystemComponent()
{
}

void UArcherAbilitySystemComponent::InitializeDefaultAbilitiesEffects()
{
	// Add default abilities
	for (TSubclassOf<UArcherGameplayAbility> DefaultAbility : DefaultAbilities)
	{
		GiveAbility(FGameplayAbilitySpec(DefaultAbility, DefaultAbility.GetDefaultObject()->AbilityLevel, static_cast<int32>(DefaultAbility.GetDefaultObject()->AbilityInputID)));
	}

	// Add default effects
	for (TSubclassOf<UArcherGameplayEffect> DefaultEffect : DefaultEffects)
	{
		FGameplayEffectContextHandle EffectContextHandle = MakeEffectContext();
		EffectContextHandle.AddSourceObject(this);

		FGameplayEffectSpecHandle SpecHandle = MakeOutgoingSpec(DefaultEffect, DefaultEffect.GetDefaultObject()->EffectLevel, EffectContextHandle);

		ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
	}
}

void UArcherAbilitySystemComponent::AbilityInputPressed(const FGameplayTag& InputTag)
{
	// if we found a ability with the given tag try to activate it
	if (const auto foundGameplayAbilitySpec = ActivatableAbilities.Items.FindByPredicate([&](const FGameplayAbilitySpec GameplayAbilitySpec)
	{
		return GameplayAbilitySpec.Ability && GameplayAbilitySpec.Ability->AbilityTags.HasTagExact(InputTag);
	}))
	{
		TryActivateAbility(foundGameplayAbilitySpec->Handle);
	}
}

void UArcherAbilitySystemComponent::AbilityInputReleased(const FGameplayTag& InputTag)
{
}
