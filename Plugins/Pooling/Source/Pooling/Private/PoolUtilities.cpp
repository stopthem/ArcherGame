// Fill out your copyright notice in the Description page of Project Settings.


#include "PoolUtilities.h"

#include "BlueprintGameplayTagLibrary.h"
#include "Pooler.h"
#include "Kismet/KismetSystemLibrary.h"

// Get associated pooler by given PoolGameplayTag
APooler* PoolUtilities::GetPoolerByGameplayTag(UWorld* world, const FGameplayTag& poolGameplayTag)
{
	// create a query based on given poolGameplayTag
	const FGameplayTagQuery gameplayTagQuery = FGameplayTagQuery::BuildQuery(FGameplayTagQueryExpression().AllTagsMatch().AddTag(poolGameplayTag));

	// find all actors of the corresponding gameplayTagQuery
	TArray<AActor*> foundActors;
	UBlueprintGameplayTagLibrary::GetAllActorsOfClassMatchingTagQuery(world, APooler::StaticClass(), gameplayTagQuery, foundActors);

	// if nothing found return
	if (foundActors.Num() == 0)
	{
		UE_LOG(LogTemp, Error, TEXT("no pooler was found with tag %s"), *poolGameplayTag.GetTagName().ToString());
		return nullptr;
	}

	return Cast<APooler>(foundActors[0]);
}
