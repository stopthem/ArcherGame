// Fill out your copyright notice in the Description page of Project Settings.


#include "ArcherGameplayCueManager.h"

#include "AbilitySystemGlobals.h"

UArcherGameplayCueManager::UArcherGameplayCueManager(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

UArcherGameplayCueManager* UArcherGameplayCueManager::Get()
{
	return Cast<UArcherGameplayCueManager>(UAbilitySystemGlobals::Get().GetGameplayCueManager());
}

bool UArcherGameplayCueManager::ShouldAsyncLoadRuntimeObjectLibraries() const
{
	return false;
}
