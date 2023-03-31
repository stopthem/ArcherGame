// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/GameFrameworkComponent.h"
#include "ArcherManaComponent.generated.h"

struct FOnAttributeChangeData;
class UArcherManaSet;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FArcherMana_AttributeChanged, UArcherManaComponent*, ManaComponent, float, OldValue, float, NewValue, AActor*, Instigator);

/**
 * UArcherManaComponent
 *
 * An actor component used to handle anything related to mana.
 */
UCLASS(Blueprintable, meta=(BlueprintSpawnableComponent))
class ARCHERGAME_API UArcherManaComponent : public UGameFrameworkComponent
{
	GENERATED_BODY()

public:
	UArcherManaComponent(const FObjectInitializer& objectInitializer);

	// Returns the mana component if one exists on the specified actor.
	UFUNCTION(BlueprintPure, Category = "Archer|Mana")
	static UArcherManaComponent* FindManaComponent(const AActor* actor) { return actor ? actor->FindComponentByClass<UArcherManaComponent>() : nullptr; }

	// Initialize the component using an ability system component.
	UFUNCTION(BlueprintCallable, Category = "Archer|Mana")
	void InitializeWithAbilitySystem(UArcherAbilitySystemComponent* inASC);

	// Returns the current Mana value.
	UFUNCTION(BlueprintCallable, Category = "Archer|Mana")
	float GetMana() const;

	// Returns the current maximum Mana value.
	UFUNCTION(BlueprintCallable, Category = "Archer|Mana")
	float GetMaxMana() const;

	// Returns the current Mana in the range [0.0, 1.0].
	UFUNCTION(BlueprintCallable, Category = "Archer|Mana")
	float GetManaNormalized() const;

public:
	// Delegate fired when the mana value has changed.
	UPROPERTY(BlueprintAssignable)
	FArcherMana_AttributeChanged OnManaChanged;

	// Delegate fired when the max mana value has changed.
	UPROPERTY(BlueprintAssignable)
	FArcherMana_AttributeChanged OnMaxManaChanged;

protected:
	virtual void HandleManaChanged(const FOnAttributeChangeData& changeData);
	virtual void HandleMaxManaChanged(const FOnAttributeChangeData& changeData);

protected:
	// Ability system used by this component.
	UPROPERTY()
	TObjectPtr<UArcherAbilitySystemComponent> AbilitySystemComponent;

	// Health set used by this component.
	UPROPERTY()
	TObjectPtr<const UArcherManaSet> ManaSet;
};
