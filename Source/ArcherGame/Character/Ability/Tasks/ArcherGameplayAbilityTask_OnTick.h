// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "ArcherGameplayAbilityTask_OnTick.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTickTaskDelegate, float, DeltaTime);

/**
 * UArcherGameplayAbilityTask_OnTick
 *
 * Task for abilities that supply tick and its delta time.
 */
UCLASS()
class ARCHERGAME_API UArcherGameplayAbilityTask_OnTick : public UAbilityTask
{
	GENERATED_BODY()

	UPROPERTY(BlueprintAssignable)
	FOnTickTaskDelegate OnTickTaskDelegate;

public:
	UArcherGameplayAbilityTask_OnTick();

	UFUNCTION(BlueprintCallable, Category="Ability|Tasks", meta=(HidePin="OwningAbility", DefaultToSelf="OwningAbility", BlueprintInternalUseOnly = "TRUE"))
	static UArcherGameplayAbilityTask_OnTick* AbilityTask_OnTick(UGameplayAbility* owningAbility, FName taskInstanceName);

	virtual void Activate() override;
	virtual void TickTask(float deltaTime) override;

private:
	float TaskDeltaTime;
};
