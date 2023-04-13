// Fill out your copyright notice in the Description page of Project Settings.


#include "ArcherAbilityTagRelationshipMapping.h"

void UArcherAbilityTagRelationshipMapping::GetAbilityTagsToBlockAndCancel(const FGameplayTagContainer& abilityTags, FGameplayTagContainer* outTagsToBlock, FGameplayTagContainer* outTagsToCancel) const
{
	// Simple iteration for now
	for (int32 i = 0; i < AbilityTagRelationships.Num(); i++)
	{
		if (const auto& [abilityTag, abilityTagsToBlock, abilityTagsToCancel, activationRequiredTags, activationBlockedTags] = AbilityTagRelationships[i]; abilityTags.HasTag(abilityTag))
		{
			if (outTagsToBlock)
			{
				outTagsToBlock->AppendTags(abilityTagsToBlock);
			}
			if (outTagsToCancel)
			{
				outTagsToCancel->AppendTags(abilityTagsToCancel);
			}
		}
	}
}

void UArcherAbilityTagRelationshipMapping::GetRequiredAndBlockedActivationTags(const FGameplayTagContainer& abilityTags, FGameplayTagContainer* outActivationRequired, FGameplayTagContainer* outActivationBlocked) const
{
	// Simple iteration for now
	for (int32 i = 0; i < AbilityTagRelationships.Num(); i++)
	{
		if (const auto& [abilityTag, abilityTagsToBlock, abilityTagsToCancel, activationRequiredTags, activationBlockedTags] = AbilityTagRelationships[i]; abilityTags.HasTag(abilityTag))
		{
			if (outActivationRequired)
			{
				outActivationRequired->AppendTags(activationRequiredTags);
			}
			if (outActivationBlocked)
			{
				outActivationBlocked->AppendTags(activationBlockedTags);
			}
		}
	}
}

bool UArcherAbilityTagRelationshipMapping::IsAbilityCancelledByTag(const FGameplayTagContainer& abilityTags, const FGameplayTag& actionTag) const
{
	// Simple iteration for now
	for (int32 i = 0; i < AbilityTagRelationships.Num(); i++)
	{
		if (const auto& [abilityTag, abilityTagsToBlock, abilityTagsToCancel, activationRequiredTags, activationBlockedTags] = AbilityTagRelationships[i]; abilityTag == actionTag && abilityTagsToCancel.HasAny(abilityTags))
		{
			return true;
		}
	}

	return false;
}
