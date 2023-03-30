// Fill out your copyright notice in the Description page of Project Settings.


#include "PoolerGameInstanceSubsystem.h"

#include "GameplayTagContainer.h"
#include "Pooler.h"

UPoolerGameInstanceSubsystem::UPoolerGameInstanceSubsystem()
{
}

void UPoolerGameInstanceSubsystem::AddToPoolerArray(APooler* pooler)
{
	Poolers.AddUnique(pooler);
}

APooler* UPoolerGameInstanceSubsystem::GetPoolerFromGameplayTag(FGameplayTag gameplayTag)
{
	APooler* pooler = nullptr;
	if (const auto foundPooler = Poolers.FindByPredicate([&](const APooler* arrayPooler)
	{
		return arrayPooler->HasMatchingGameplayTag(gameplayTag);
	}))
	{
		pooler = *foundPooler;
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("no pooler was found with tag %s"), *gameplayTag.GetTagName().ToString());
	}

	return pooler;
}
