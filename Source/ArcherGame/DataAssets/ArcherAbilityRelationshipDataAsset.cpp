// Fill out your copyright notice in the Description page of Project Settings.


#include "ArcherAbilityRelationshipDataAsset.h"

UArcherAbilityRelationshipDataAsset::UArcherAbilityRelationshipDataAsset()
{
}

bool UArcherAbilityRelationshipDataAsset::GetAbilityRelationshipFromTagContainer(const FGameplayTagContainer& tagContainer, FAbilityRelationshipInfo& out_abilityRelationshipInfo)
{
	if (const auto foundAbilityRelationship = AbilityRelationshipInfos.FindByPredicate([&](const FAbilityRelationshipInfo& abilityRelationshipInfo)
	{
		return tagContainer.HasTagExact(abilityRelationshipInfo.AbilityTag);
	}))
	{
		out_abilityRelationshipInfo = *foundAbilityRelationship;
		return true;
	}

	return false;
}
