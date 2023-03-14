// Fill out your copyright notice in the Description page of Project Settings.


#include "ArcherGameplayAbilityTask_OnTick.h"

UArcherGameplayAbilityTask_OnTick::UArcherGameplayAbilityTask_OnTick()
{
	bTickingTask = true;
}

UArcherGameplayAbilityTask_OnTick* UArcherGameplayAbilityTask_OnTick::AbilityTask_OnTick(UGameplayAbility* owningAbility, FName taskInstanceName)
{
	UArcherGameplayAbilityTask_OnTick* myObj = NewAbilityTask<UArcherGameplayAbilityTask_OnTick>(owningAbility);
	return myObj;
}

void UArcherGameplayAbilityTask_OnTick::Activate()
{
	Super::Activate();
}

void UArcherGameplayAbilityTask_OnTick::TickTask(float deltaTime)
{
	Super::TickTask(deltaTime);
	OnTickTaskDelegate.Broadcast(deltaTime);
}
