// Fill out your copyright notice in the Description page of Project Settings.

#include "ArcherCharacter.h"

#include "Ability/ArcherAbilitySet.h"
#include "Ability/Attribute/ArcherAttributeSet.h"
#include "Ability/Attribute/ArcherHealthComponent.h"
#include "Components/AudioComponent.h"

// Sets default values
AArcherCharacter::AArcherCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	AbilitySystemComponent = CreateDefaultSubobject<UArcherAbilitySystemComponent>(TEXT("ArcherAbilitySystemComponent"));

	ArcherHealthComponent = CreateDefaultSubobject<UArcherHealthComponent>("ArcherCharacterHealthComponent");

	AudioComponent = CreateDefaultSubobject<UAudioComponent>("AudioComponent");
}

void AArcherCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	check(AbilitySystemComponent);

	if (bAbilitySystemInitted)return;
	bAbilitySystemInitted = true;

	AfterPossessedBy();
}

void AArcherCharacter::AfterPossessedBy()
{
	AbilitySystemComponent->InitAbilityActorInfo(this, this);

	AbilitySet->GrantSetAbilityEffectAttributes(AbilitySystemComponent);

	ArcherHealthComponent->InitializeWithAbilitySystem(AbilitySystemComponent);
}

void AArcherCharacter::PlaySoundEffect(USoundBase* soundBase, float playChance) const
{
	check(AudioComponent);

	if (FMath::RandRange(0, 1) > playChance)
	{
		return;
	}

	AudioComponent->SetSound(soundBase);
	AudioComponent->Play();
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
