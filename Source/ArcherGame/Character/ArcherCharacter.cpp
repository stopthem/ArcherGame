// Fill out your copyright notice in the Description page of Project Settings.

#include "ArcherCharacter.h"

#include "Abilities/Async/AbilityAsync_WaitGameplayEvent.h"
#include "Ability/ArcherAbilitySet.h"
#include "Ability/Attribute/ArcherAttributeSet.h"
#include "Ability/Attribute/ArcherHealthComponent.h"
#include "ArcherGame/ArcherGameplayTags.h"

// Sets default values
AArcherCharacter::AArcherCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	AbilitySystemComponent = CreateDefaultSubobject<UArcherAbilitySystemComponent>(TEXT("ArcherAbilitySystemComponent"));

	HealthComponent = CreateDefaultSubobject<UArcherHealthComponent>("ArcherCharacterHealthComponent");
}

void AArcherCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	check(AbilitySystemComponent);

	if (bAbilitySystemInitted)return;
	bAbilitySystemInitted = true;

	AbilitySystemComponent->InitAbilityActorInfo(this, this);

	AbilitySet->GrantSetAbilityEffectAttributes(AbilitySystemComponent);

	HealthComponent->InitializeWithAbilitySystem(AbilitySystemComponent);
}

void AArcherCharacter::GetOwnedGameplayTags(FGameplayTagContainer& TagContainer) const
{
	if (AbilitySystemComponent)
	{
		AbilitySystemComponent->GetOwnedGameplayTags(TagContainer);
	}
}

bool AArcherCharacter::HasMatchingGameplayTag(FGameplayTag TagToCheck) const
{
	return AbilitySystemComponent ? AbilitySystemComponent->HasMatchingGameplayTag(TagToCheck) : false;
}

bool AArcherCharacter::HasAllMatchingGameplayTags(const FGameplayTagContainer& TagContainer) const
{
	return AbilitySystemComponent ? AbilitySystemComponent->HasAllMatchingGameplayTags(TagContainer) : false;
}

bool AArcherCharacter::HasAnyMatchingGameplayTags(const FGameplayTagContainer& TagContainer) const
{
	return AbilitySystemComponent ? AbilitySystemComponent->HasAnyMatchingGameplayTags(TagContainer) : false;
}
