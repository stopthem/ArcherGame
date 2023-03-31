// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NativeGameplayTags.h"
#include "Components/GameFrameworkComponent.h"
#include "ArcherHealthComponent.generated.h"


struct FGameplayAttribute;
class UArcherHealthSet;
class UArcherAbilitySystemComponent;
struct FOnAttributeChangeData;
struct FGameplayEffectSpec;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FArcherHealth_DeathEvent, AActor*, OwningActor);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FArcherHealth_AttributeChanged, UArcherHealthComponent*, HealthComponent, float, OldValue, float, NewValue, AActor*, Instigator);

/**
 * UArcherHealthComponent
 * 
 * An actor component used to handle anything related to health.
 */
UCLASS(Blueprintable, meta=(BlueprintSpawnableComponent))
class ARCHERGAME_API UArcherHealthComponent : public UGameFrameworkComponent
{
	GENERATED_BODY()

public:
	UArcherHealthComponent(const FObjectInitializer& objectInitializer);

	// Returns the health component if one exists on the specified actor.
	UFUNCTION(BlueprintPure, Category = "Archer|Health")
	static UArcherHealthComponent* FindHealthComponent(const AActor* actor) { return actor ? actor->FindComponentByClass<UArcherHealthComponent>() : nullptr; }

	// Initialize the component using an ability system component.
	UFUNCTION(BlueprintCallable, Category = "Archer|Health")
	void InitializeWithAbilitySystem(UArcherAbilitySystemComponent* inASC);

	// Returns the current health value.
	UFUNCTION(BlueprintCallable, Category = "Archer|Health")
	float GetHealth() const;

	// Returns the current maximum health value.
	UFUNCTION(BlueprintCallable, Category = "Archer|Health")
	float GetMaxHealth() const;

	// Returns the current health in the range [0.0, 1.0].
	UFUNCTION(BlueprintCallable, Category = "Archer|Health")
	float GetHealthNormalized() const;

	// Begins the death sequence for the owner.
	virtual void StartDeath();

	// Ends the death sequence for the owner.
	virtual void FinishDeath();

public:
	// Delegate fired when the health value has changed.
	UPROPERTY(BlueprintAssignable)
	FArcherHealth_AttributeChanged OnHealthChanged;

	// Delegate fired when the max health value has changed.
	UPROPERTY(BlueprintAssignable)
	FArcherHealth_AttributeChanged OnMaxHealthChanged;

	// Delegate fired when the death sequence has started.
	UPROPERTY(BlueprintAssignable)
	FArcherHealth_DeathEvent OnDeathStarted;

	// Delegate fired when the death sequence has finished.
	UPROPERTY(BlueprintAssignable)
	FArcherHealth_DeathEvent OnDeathFinished;

protected:
	virtual void OnUnregister() override;

	virtual void HandleHealthChanged(const FOnAttributeChangeData& changeData);
	virtual void HandleMaxHealthChanged(const FOnAttributeChangeData& changeData);
	UFUNCTION()
	virtual void HandleOutOfHealth(AActor* damageInstigator, AActor* damageCauser, const FGameplayEffectSpec& damageEffectSpec, float damageMagnitude);

protected:
	// Ability system used by this component.
	UPROPERTY()
	TObjectPtr<UArcherAbilitySystemComponent> AbilitySystemComponent;

	// Health set used by this component.
	UPROPERTY()
	TObjectPtr<const UArcherHealthSet> HealthSet;
};
