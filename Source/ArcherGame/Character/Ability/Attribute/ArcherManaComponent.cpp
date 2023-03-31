// Fill out your copyright notice in the Description page of Project Settings.


#include "ArcherManaComponent.h"

#include "ArcherHealthSet.h"
#include "ArcherManaSet.h"
#include "GameplayEffectExtension.h"
#include "ArcherGame/Character/Ability/ArcherAbilitySystemComponent.h"

UArcherManaComponent::UArcherManaComponent(const FObjectInitializer& objectInitializer)
	: Super(objectInitializer)
{
	PrimaryComponentTick.bStartWithTickEnabled = false;
	PrimaryComponentTick.bCanEverTick = false;
}

void UArcherManaComponent::InitializeWithAbilitySystem(UArcherAbilitySystemComponent* inASC)
{
	const AActor* owner = GetOwner();
	check(owner);

	if (AbilitySystemComponent)
	{
		UE_LOG(LogTemp, Error, TEXT("ArcherManaComponent: mana component for owner [%s] has already been initialized with an ability system."), *GetNameSafe(owner));
		return;
	}

	AbilitySystemComponent = inASC;
	if (!AbilitySystemComponent)
	{
		UE_LOG(LogTemp, Error, TEXT("ArcherManaComponent: Cannot initialize mana component for owner [%s] with NULL ability system."), *GetNameSafe(owner));
		return;
	}

	ManaSet = AbilitySystemComponent->GetSet<UArcherManaSet>();
	if (!ManaSet)
	{
		UE_LOG(LogTemp, Error, TEXT("ArcherManaComponent: Cannot initialize mana component for owner [%s] with NULL mana set on the ability system."), *GetNameSafe(owner));
		return;
	}

	inASC->GetGameplayAttributeValueChangeDelegate(UArcherManaSet::GetManaAttribute()).AddUObject(this, &ThisClass::HandleManaChanged);
	inASC->GetGameplayAttributeValueChangeDelegate(UArcherManaSet::GetMaxManaAttribute()).AddUObject(this, &ThisClass::HandleMaxManaChanged);

	OnManaChanged.Broadcast(this, ManaSet->GetMana(), ManaSet->GetMana(), nullptr);
	OnMaxManaChanged.Broadcast(this, ManaSet->GetMaxMana(), ManaSet->GetMaxMana(), nullptr);
}

float UArcherManaComponent::GetMana() const
{
	return ManaSet ? ManaSet->GetMana() : 0.0f;
}

float UArcherManaComponent::GetMaxMana() const
{
	return ManaSet ? ManaSet->GetMaxMana() : 0.0f;
}

float UArcherManaComponent::GetManaNormalized() const
{
	return ManaSet && ManaSet->GetMaxMana() > 0.0f ? ManaSet->GetMana() / ManaSet->GetMaxMana() : 0.0f;
}

void UArcherManaComponent::HandleManaChanged(const FOnAttributeChangeData& changeData)
{
	OnManaChanged.Broadcast(this, ManaSet->GetMana(), ManaSet->GetMana(), UArcherAbilitySystemComponent::GetInstigatorFromAttrChangeData(changeData));
}

void UArcherManaComponent::HandleMaxManaChanged(const FOnAttributeChangeData& changeData)
{
	OnMaxManaChanged.Broadcast(this, ManaSet->GetMaxMana(), ManaSet->GetMaxMana(), UArcherAbilitySystemComponent::GetInstigatorFromAttrChangeData(changeData));
}
