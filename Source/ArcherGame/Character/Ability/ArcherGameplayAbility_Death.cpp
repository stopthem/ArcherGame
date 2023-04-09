// Fill out your copyright notice in the Description page of Project Settings.


#include "ArcherGameplayAbility_Death.h"

#include "ArcherGame/ArcherGameplayTags.h"

UArcherGameplayAbility_Death::UArcherGameplayAbility_Death()
{
}

void UArcherGameplayAbility_Death::OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
{
	if (AbilityTriggers.Num() == 0)
	{
		// Add the ability trigger tag as default to the CDO.
		FAbilityTriggerData TriggerData;
		TriggerData.TriggerTag = TAG_GameplayEvent_Death;
		TriggerData.TriggerSource = EGameplayAbilityTriggerSource::GameplayEvent;
		AbilityTriggers.Add(TriggerData);
	}

	Super::OnGiveAbility(ActorInfo, Spec);
}
