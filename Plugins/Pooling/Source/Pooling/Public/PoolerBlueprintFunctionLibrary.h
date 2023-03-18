// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "PoolerBlueprintFunctionLibrary.generated.h"

/**
 * 
 */
UCLASS()
class POOLING_API UPoolerBlueprintFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	// Get associated pooler by given PoolGameplayTag
	UFUNCTION(BlueprintCallable, meta=(WorldContext="context"), Category="Pool")
	static APooler* GetPoolerByGameplayTag(UObject* context, struct FGameplayTag poolGameplayTag);
};
