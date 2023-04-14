// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ArcherAttributeSet.h"
#include "NativeGameplayTags.h"
#include "ArcherHealthSet.generated.h"

UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Gameplay_Damage);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Gameplay_DamageImmunity);

/**
 * UArcherHealthSet
 *
 *	Class that defines attributes that are necessary for taking damage and health.
 */
UCLASS(BlueprintType)
class ARCHERGAME_API UArcherHealthSet : public UArcherAttributeSet
{
	GENERATED_BODY()

public:
	UArcherHealthSet();

	ATTRIBUTE_ACCESSORS(UArcherHealthSet, Health);
	ATTRIBUTE_ACCESSORS(UArcherHealthSet, MaxHealth);
	ATTRIBUTE_ACCESSORS(UArcherHealthSet, Healing);
	ATTRIBUTE_ACCESSORS(UArcherHealthSet, Damage);

	mutable FArcherAttributeEvent OnOutOfHealth;

protected:
	virtual bool PreGameplayEffectExecute(FGameplayEffectModCallbackData& data) override;
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& data) override;

	virtual void PreAttributeBaseChange(const FGameplayAttribute& attribute, float& newValue) const override;
	virtual void PreAttributeChange(const FGameplayAttribute& attribute, float& newValue) override;
	virtual void PostAttributeChange(const FGameplayAttribute& attribute, float oldValue, float newValue) override;

	void ClampAttribute(const FGameplayAttribute& attribute, float& newValue) const;

private:
	// The current health attribute.  The health will be capped by the max health attribute.  Health is hidden from modifiers so only executions can modify it.
	UPROPERTY(BlueprintReadOnly, Category = "Archer|Health", Meta = (HideFromModifiers, AllowPrivateAccess = true))
	FGameplayAttributeData Health;

	// The current max health attribute.  Max health is an attribute since gameplay effects can modify it.
	UPROPERTY(BlueprintReadOnly, Category = "Archer|Health", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData MaxHealth;

	// Used to track when the health reaches 0.
	bool bOutOfHealth;

private:
	// Incoming healing. This is mapped directly to +Health
	UPROPERTY(BlueprintReadOnly, Category="Archer|Health", Meta=(AllowPrivateAccess=true))
	FGameplayAttributeData Healing;

	// Incoming damage. This is mapped directly to -Health
	UPROPERTY(BlueprintReadOnly, Category="Archer|Health", Meta=(HideFromModifiers, AllowPrivateAccess=true))
	FGameplayAttributeData Damage;
};
