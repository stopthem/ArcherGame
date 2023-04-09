// Fill out your copyright notice in the Description page of Project Settings.


#include "ArcherAbilityIconsPrimaryAsset.h"

UArcherAbilityIconsPrimaryAsset::UArcherAbilityIconsPrimaryAsset()
{
}

FAbilityIconInfo UArcherAbilityIconsPrimaryAsset::GetAbilityIconInfoFromTag(FGameplayTag abilityTag)
{
	for (const FPrioritizedAbilityIconInfos prioritizedAbilityIconInfos : PrioritizedAbilityIconInfosArray)
	{
		if (const auto foundAbilityIconInfo = prioritizedAbilityIconInfos.AbilityIconInfos.FindByPredicate([&](const FAbilityIconInfo& abilityIconInfo)
		{
			return abilityIconInfo.AbilityTag == abilityTag;
		}))
		{
			return *foundAbilityIconInfo;
		}
	}

	UE_LOG(LogTemp, Warning, TEXT("ArcherAbilityIconsPrimaryAsset -> no ability icon info found with tag %s"), *abilityTag.GetTagName().ToString());
	return FAbilityIconInfo();
}

TArray<FAbilityIconInfo> UArcherAbilityIconsPrimaryAsset::GetOrderedAbilityIconInfosFromGameplayTagContainer(FGameplayTagContainer gameplayTagContainer)
{
	TArray<FAbilityIconInfo> AbilityIconInfos;

	// ordering prioritized ability icon infos in case of they were not set ordered in editor
	TArray<FPrioritizedAbilityIconInfos> orderedPrioAbilityIconInfos = TArray(PrioritizedAbilityIconInfosArray);
	orderedPrioAbilityIconInfos.Sort([](const FPrioritizedAbilityIconInfos& prioritizedAbilityIconInfo1, const FPrioritizedAbilityIconInfos& prioritizedAbilityIconInfos2)
	{
		return prioritizedAbilityIconInfo1.SpawnPriority < prioritizedAbilityIconInfos2.SpawnPriority;
	});

	for (const FPrioritizedAbilityIconInfos prioritizedAbilityIconInfos : orderedPrioAbilityIconInfos)
	{
		for (const FAbilityIconInfo abilityIconInfo : prioritizedAbilityIconInfos.AbilityIconInfos)
		{
			if (gameplayTagContainer.HasTagExact(abilityIconInfo.AbilityTag))
			{
				AbilityIconInfos.Add(abilityIconInfo);
			}
		}
	}

	return AbilityIconInfos;
}
