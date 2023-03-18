// Fill out your copyright notice in the Description page of Project Settings.


#include "ArcherGameplayAbility.h"

#include "ArcherGame/Character/Player/ArcherPlayerCharacter.h"

UArcherGameplayAbility::UArcherGameplayAbility()
{
}

AArcherPlayerCharacter* UArcherGameplayAbility::GetArcherPlayerCharacterFromActorInfo() const
{
	return CurrentActorInfo ? Cast<AArcherPlayerCharacter>(CurrentActorInfo->AvatarActor.Get()) : nullptr;
}
