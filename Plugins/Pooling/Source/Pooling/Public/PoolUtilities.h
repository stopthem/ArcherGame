// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

struct FGameplayTag;
class APooler;
/**
 *  A provider of pool utilities
 */
namespace PoolUtilities
{
	// Get associated pooler by given PoolGameplayTag
	APooler* GetPoolerByGameplayTag(UWorld* world, const FGameplayTag& poolGameplayTag);
}
