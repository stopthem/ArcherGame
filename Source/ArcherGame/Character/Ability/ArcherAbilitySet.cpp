// Fill out your copyright notice in the Description page of Project Settings.


#include "ArcherAbilitySet.h"

#include "ArcherAbilitySystemComponent.h"
#include "ArcherGameplayAbility.h"
#include "ArcherGameplayEffect.h"
#include "GameplayAbilitySpec.h"

UArcherAbilitySet::UArcherAbilitySet(const FObjectInitializer& ObjectInitializer)
{
}

void UArcherAbilitySet::GrantSetAbilityEffectAttributes(UArcherAbilitySystemComponent* ArcherASC)
{
	// Grant set abilities
	for (auto [Ability, AbilityLevel, InputTag] : GrantedGameplayAbilities)
	{
		UArcherGameplayAbility* gameplayAbility = Ability->GetDefaultObject<UArcherGameplayAbility>();

		FGameplayAbilitySpec setGameplayAbilitySpec(gameplayAbility, AbilityLevel);
		setGameplayAbilitySpec.SourceObject = this;
		setGameplayAbilitySpec.DynamicAbilityTags.AddTag(InputTag);

		ArcherASC->GiveAbility(setGameplayAbilitySpec);
	}

	// Grant set effects
	for (auto [GameplayEffect,EffectLevel] : GrantedGameplayEffects)
	{
		FGameplayEffectContextHandle EffectContextHandle = ArcherASC->MakeEffectContext();
		EffectContextHandle.AddSourceObject(this);

		const UArcherGameplayEffect* gameplayEffect = GameplayEffect->GetDefaultObject<UArcherGameplayEffect>();

		ArcherASC->ApplyGameplayEffectToSelf(gameplayEffect, EffectLevel, ArcherASC->MakeEffectContext());
	}

	for (FArcherAbilitySet_AttributeSet attributes : GrantedAttributes)
	{
		UAttributeSet* NewSet = NewObject<UAttributeSet>(ArcherASC->GetOwner(), attributes.AttributeSet);
		ArcherASC->AddAttributeSetSubobject(NewSet);
		NewSet->InitFromMetaDataTable(attributes.DefaultStartingDataTable);
	}
}

FGameplayTag UArcherAbilitySet::GetInputTagFromAbility(const TSubclassOf<UArcherGameplayAbility> gameplayAbility)
{
	check(gameplayAbility);

	if (const auto gameplayAbilitySet = GrantedGameplayAbilities.FindByPredicate([&](const FArcherAbilitySet_GameplayAbility gameplayAbilitySetElement)
	{
		return gameplayAbilitySetElement.Ability == gameplayAbility;
	}))
	{
		return gameplayAbilitySet->InputTag;
	}

	UE_LOG(LogTemp, Warning, TEXT("ArcherAbilitySet -> no gameplay ability was found with name %s"), *gameplayAbility->GetName());
	return FGameplayTag::EmptyTag;
}
