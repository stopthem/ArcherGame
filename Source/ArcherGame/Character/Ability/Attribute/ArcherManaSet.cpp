// Fill out your copyright notice in the Description page of Project Settings.


#include "ArcherManaSet.h"

#include "GameplayEffectExtension.h"
#include "NativeGameplayTags.h"

UE_DEFINE_GAMEPLAY_TAG(TAG_Gameplay_ManaImmunity, "Gameplay.ManaImmunity");

UArcherManaSet::UArcherManaSet()
{
}

bool UArcherManaSet::PreGameplayEffectExecute(FGameplayEffectModCallbackData& data)
{
	if (!Super::PreGameplayEffectExecute(data))
	{
		return false;
	}

	if (data.Target.HasMatchingGameplayTag(TAG_Gameplay_ManaImmunity))
	{
		data.EvaluatedData.Magnitude = 0.0f;
		return false;
	}

	return Super::PreGameplayEffectExecute(data);
}

void UArcherManaSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& data)
{
	Super::PostGameplayEffectExecute(data);

	float minimumMana = 0.0f;

	auto GetClampedMana = [&](float value)
	{
		return FMath::Clamp(value, minimumMana, GetMaxMana());
	};

	if (data.EvaluatedData.Attribute == GetManaRegenAttribute())
	{
		SetMana(GetClampedMana(GetMana() + GetManaRegen()));
		SetManaRegen(0.0f);
	}
	else if (data.EvaluatedData.Attribute == GetManaAttribute())
	{
		SetMana(GetClampedMana(GetMana()));
	}
}

void UArcherManaSet::PreAttributeBaseChange(const FGameplayAttribute& attribute, float& newValue) const
{
	Super::PreAttributeBaseChange(attribute, newValue);
}

void UArcherManaSet::PreAttributeChange(const FGameplayAttribute& attribute, float& newValue)
{
	Super::PreAttributeChange(attribute, newValue);
}

void UArcherManaSet::PostAttributeChange(const FGameplayAttribute& attribute, float oldValue, float newValue)
{
	Super::PostAttributeChange(attribute, oldValue, newValue);
}

void UArcherManaSet::ClampAttribute(const FGameplayAttribute& attribute, float& newValue) const
{
	if (attribute == GetManaAttribute())
	{
		// Do not allow mana to go negative or above max mana.
		newValue = FMath::Clamp(newValue, 0.0f, GetMaxMana());
	}
	else if (attribute == GetMaxManaAttribute())
	{
		// Do not allow max mana to drop below 1.
		newValue = FMath::Max(newValue, 1.0f);
	}
}
