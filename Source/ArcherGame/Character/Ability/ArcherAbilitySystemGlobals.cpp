// Fill out your copyright notice in the Description page of Project Settings.


#include "ArcherAbilitySystemGlobals.h"

#include "ArcherGameplayEffectContext.h"

FGameplayEffectContext* UArcherAbilitySystemGlobals::AllocGameplayEffectContext() const
{
	return new FArcherGameplayEffectContext();
}
