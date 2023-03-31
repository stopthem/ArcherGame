// Fill out your copyright notice in the Description page of Project Settings.


#include "ArcherHealthComponent.h"

#include "ArcherHealthSet.h"
#include "GameplayEffectExtension.h"
#include "ArcherGame/ArcherGameplayTags.h"
#include "ArcherGame/Character/Ability/ArcherAbilitySystemComponent.h"

UArcherHealthComponent::UArcherHealthComponent(const FObjectInitializer& objectInitializer)
	: Super(objectInitializer)
{
	PrimaryComponentTick.bStartWithTickEnabled = false;
	PrimaryComponentTick.bCanEverTick = false;
}

void UArcherHealthComponent::InitializeWithAbilitySystem(UArcherAbilitySystemComponent* inASC)
{
	const AActor* owner = GetOwner();
	check(owner);

	if (AbilitySystemComponent)
	{
		UE_LOG(LogTemp, Error, TEXT("ArcherHealthComponent: Health component for owner [%s] has already been initialized with an ability system."), *GetNameSafe(owner));
		return;
	}

	AbilitySystemComponent = inASC;
	if (!AbilitySystemComponent)
	{
		UE_LOG(LogTemp, Error, TEXT("ArcherHealthComponent: Cannot initialize health component for owner [%s] with NULL ability system."), *GetNameSafe(owner));
		return;
	}

	HealthSet = AbilitySystemComponent->GetSet<UArcherHealthSet>();
	if (!HealthSet)
	{
		UE_LOG(LogTemp, Error, TEXT("ArcherHealthComponent: Cannot initialize health component for owner [%s] with NULL health set on the ability system."), *GetNameSafe(owner));
		return;
	}

	// Register to listen for attribute changes.
	inASC->GetGameplayAttributeValueChangeDelegate(UArcherHealthSet::GetHealthAttribute()).AddUObject(this, &ThisClass::HandleHealthChanged);
	inASC->GetGameplayAttributeValueChangeDelegate(UArcherHealthSet::GetMaxHealthAttribute()).AddUObject(this, &ThisClass::HandleMaxHealthChanged);

	HealthSet->OnOutOfHealth.AddUObject(this, &ThisClass::HandleOutOfHealth);
	// TEMP: Reset attributes to default values.  Eventually this will be driven by a spread sheet.
	// inASC->SetNumericAttributeBase(UArcherHealthSet::GetHealthAttribute(), HealthSet->GetMaxHealth());

	OnHealthChanged.Broadcast(this, HealthSet->GetHealth(), HealthSet->GetHealth(), nullptr);
	OnMaxHealthChanged.Broadcast(this, HealthSet->GetHealth(), HealthSet->GetHealth(), nullptr);
}

float UArcherHealthComponent::GetHealth() const
{
	return HealthSet ? HealthSet->GetHealth() : 0.0f;
}

float UArcherHealthComponent::GetMaxHealth() const
{
	return HealthSet ? HealthSet->GetMaxHealth() : 0.0f;
}

float UArcherHealthComponent::GetHealthNormalized() const
{
	return HealthSet && HealthSet->GetMaxHealth() > 0.0f ? HealthSet->GetHealth() / HealthSet->GetMaxHealth() : 0.0f;
}

void UArcherHealthComponent::StartDeath()
{
	OnDeathStarted.Broadcast(GetOwner());
}

void UArcherHealthComponent::FinishDeath()
{
	OnDeathFinished.Broadcast(GetOwner());
}

static AActor* GetInstigatorFromAttrChangeData(const FOnAttributeChangeData& ChangeData)
{
	if (ChangeData.GEModData != nullptr)
	{
		const FGameplayEffectContextHandle& EffectContext = ChangeData.GEModData->EffectSpec.GetEffectContext();
		return EffectContext.GetOriginalInstigator();
	}

	return nullptr;
}

void UArcherHealthComponent::OnUnregister()
{
	Super::OnUnregister();

	if (!HealthSet)
	{
		return;
	}

	HealthSet->OnOutOfHealth.RemoveAll(this);
}

void UArcherHealthComponent::HandleHealthChanged(const FOnAttributeChangeData& changeData)
{
	OnHealthChanged.Broadcast(this, changeData.OldValue, changeData.NewValue, GetInstigatorFromAttrChangeData(changeData));
}

void UArcherHealthComponent::HandleMaxHealthChanged(const FOnAttributeChangeData& changeData)
{
	OnMaxHealthChanged.Broadcast(this, changeData.OldValue, changeData.NewValue, GetInstigatorFromAttrChangeData(changeData));
}

void UArcherHealthComponent::HandleOutOfHealth(AActor* damageInstigator, AActor* damageCauser, const FGameplayEffectSpec& damageEffectSpec, float damageMagnitude)
{
	if (!AbilitySystemComponent)
	{
		return;
	}
	FGameplayEventData Payload;
	Payload.EventTag = TAG_GameplayEvent_Death;
	Payload.Instigator = damageInstigator;
	Payload.Target = AbilitySystemComponent->GetAvatarActor();
	Payload.OptionalObject = damageEffectSpec.Def;
	Payload.ContextHandle = damageEffectSpec.GetEffectContext();
	Payload.InstigatorTags = *damageEffectSpec.CapturedSourceTags.GetAggregatedTags();
	Payload.TargetTags = *damageEffectSpec.CapturedTargetTags.GetAggregatedTags();
	Payload.EventMagnitude = damageMagnitude;

	FScopedPredictionWindow NewScopedWindow(AbilitySystemComponent, true);
	AbilitySystemComponent->HandleGameplayEvent(Payload.EventTag, &Payload);

	StartDeath();
}
