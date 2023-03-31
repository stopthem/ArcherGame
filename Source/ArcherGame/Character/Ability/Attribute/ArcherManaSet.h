// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NativeGameplayTags.h"
#include "ArcherGame/Character/Ability/Attribute/ArcherAttributeSet.h"
#include "ArcherManaSet.generated.h"

UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Gameplay_ManaImmunity)

/**
 * UArcherManaSet
 *
 * Class that defines attributes that are necessary for mana.
 */
UCLASS()
class ARCHERGAME_API UArcherManaSet : public UArcherAttributeSet
{
	GENERATED_BODY()

public:
	UArcherManaSet();

	ATTRIBUTE_ACCESSORS(UArcherManaSet, Mana);
	ATTRIBUTE_ACCESSORS(UArcherManaSet, MaxMana);
	ATTRIBUTE_ACCESSORS(UArcherManaSet, ManaRegen);
	
protected:
	virtual bool PreGameplayEffectExecute(FGameplayEffectModCallbackData& data) override;
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& data) override;

	virtual void PreAttributeBaseChange(const FGameplayAttribute& attribute, float& newValue) const override;
	virtual void PreAttributeChange(const FGameplayAttribute& attribute, float& newValue) override;
	virtual void PostAttributeChange(const FGameplayAttribute& attribute, float oldValue, float newValue) override;

	void ClampAttribute(const FGameplayAttribute& attribute, float& newValue) const;

private:
	// The current mana attribute.  The health will be capped by the max mana attribute.  Health is hidden from modifiers so only executions can modify it.
	UPROPERTY(BlueprintReadOnly, Category = "Archer|Health", Meta = (HideFromModifiers, AllowPrivateAccess = true))
	FGameplayAttributeData Mana;

	// The current max mana attribute.  Max mana is an attribute since gameplay effects can modify it.
	UPROPERTY(BlueprintReadOnly, Category = "Archer|Health", Meta = (HideFromModifiers, AllowPrivateAccess = true))
	FGameplayAttributeData MaxMana;

	// The current mana regen. The mana regen is an attribute since gameplay effects can modify it.
	UPROPERTY(BlueprintReadOnly, Category = "Archer|Health", Meta = (HideFromModifiers, AllowPrivateAccess = true))
	FGameplayAttributeData ManaRegen;
};
