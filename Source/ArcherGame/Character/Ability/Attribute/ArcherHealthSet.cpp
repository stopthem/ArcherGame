// Fill out your copyright notice in the Description page of Project Settings.


#include "ArcherHealthSet.h"

#include "GameplayEffectExtension.h"
#include "ArcherGame/Character/Ability/ArcherAbilitySystemComponent.h"

UE_DEFINE_GAMEPLAY_TAG(TAG_Gameplay_Damage, "Gameplay.Damage");
UE_DEFINE_GAMEPLAY_TAG(TAG_Gameplay_DamageImmunity, "Gameplay.DamageImmunity");

UArcherHealthSet::UArcherHealthSet()
	: Health(100.0f)
	  , MaxHealth(100.0f)
{
	bOutOfHealth = false;
}

bool UArcherHealthSet::PreGameplayEffectExecute(FGameplayEffectModCallbackData& data)
{
	if (!Super::PreGameplayEffectExecute(data))
	{
		return false;
	}

	if (data.EvaluatedData.Attribute == GetDamageAttribute() && data.EvaluatedData.Magnitude <= 0.0f)
	{
		return false;
	}

	if (data.Target.HasMatchingGameplayTag(TAG_Gameplay_DamageImmunity))
	{
		data.EvaluatedData.Magnitude = 0.0f;
		return false;
	}

	return true;
}

void UArcherHealthSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& data)
{
	Super::PostGameplayEffectExecute(data);

	float minimumHealth = 0.0f;
	
	if (data.EvaluatedData.Attribute == GetDamageAttribute())
	{
		SetHealth(FMath::Clamp(GetHealth() - GetDamage(), minimumHealth, GetMaxHealth()));
		SetDamage(0.0f);
	}
	else if (data.EvaluatedData.Attribute == GetHealingAttribute())
	{
		// Convert into +Health and then clamp
		UE_LOG(LogTemp, Warning, TEXT("health %f healing %f max health %f"), GetHealth() + GetHealing(), GetHealing(), GetMaxHealth());

		SetHealth(FMath::Clamp(GetHealth() + GetHealing(), minimumHealth, GetMaxHealth()));
		SetHealing(0.0f);
	}
	else if (data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		// Clamp and fall into out of health handling below
		SetHealth(FMath::Clamp(GetHealth(), minimumHealth, GetMaxHealth()));
	}

	if (GetHealth() <= 0.0f && !bOutOfHealth)
	{
		if (OnOutOfHealth.IsBound())
		{
			const FGameplayEffectContextHandle& EffectContext = data.EffectSpec.GetEffectContext();
			AActor* Instigator = EffectContext.GetOriginalInstigator();
			AActor* Causer = EffectContext.GetEffectCauser();

			OnOutOfHealth.Broadcast(Instigator, Causer, data.EffectSpec, data.EvaluatedData.Magnitude);
		}
	}

	// Check health again in case an event above changed it.
	bOutOfHealth = GetHealth() <= 0.0f;
}

void UArcherHealthSet::PreAttributeBaseChange(const FGameplayAttribute& attribute, float& newValue) const
{
	Super::PreAttributeBaseChange(attribute, newValue);

	ClampAttribute(attribute, newValue);
}

void UArcherHealthSet::PreAttributeChange(const FGameplayAttribute& attribute, float& newValue)
{
	Super::PreAttributeChange(attribute, newValue);

	ClampAttribute(attribute, newValue);
}

void UArcherHealthSet::PostAttributeChange(const FGameplayAttribute& attribute, float oldValue, float newValue)
{
	Super::PostAttributeChange(attribute, oldValue, newValue);

	if (bOutOfHealth && GetHealth() > 0.0f)
	{
		bOutOfHealth = false;
	}
}

void UArcherHealthSet::ClampAttribute(const FGameplayAttribute& attribute, float& newValue) const
{
	if (attribute == GetHealthAttribute())
	{
		// Do not allow health to go negative or above max health.
		newValue = FMath::Clamp(newValue, 0.0f, GetMaxHealth());
	}
	else if (attribute == GetMaxHealthAttribute())
	{
		// Do not allow max health to drop below 1.
		newValue = FMath::Max(newValue, 1.0f);
	}
}
