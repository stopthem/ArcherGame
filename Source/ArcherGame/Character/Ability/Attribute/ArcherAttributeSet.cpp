// Fill out your copyright notice in the Description page of Project Settings.


#include "ArcherAttributeSet.h"

#include "ArcherGame/Character/Ability/ArcherAbilitySystemComponent.h"

UArcherAttributeSet::UArcherAttributeSet()
{
}

UArcherAbilitySystemComponent* UArcherAttributeSet::GetArcherAbilitySystemComponent() const
{
	return Cast<UArcherAbilitySystemComponent>(GetOwningAbilitySystemComponent());
}
