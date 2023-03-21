// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "ParticleBlueprintFunctionLibrary.generated.h"

USTRUCT(BlueprintType)
struct FParticlePlayingOptions
{
	GENERATED_BODY()

	FParticlePlayingOptions()
	{
	}

	explicit FParticlePlayingOptions(AActor* playActor)
	{
		PlayActor = playActor;
		PlayLocation = PlayActor->GetActorLocation();
	}

public:
	TObjectPtr<AActor> PlayActor = nullptr;

	FVector PlayLocation = FVector::ZeroVector;

	FVector PlayScale = FVector::OneVector;

	bool bAttachToActor = false;
	// if true, takes PlayRotation and PlayLocation as world and converts them to given PlayActor's relative. 
	bool bConvertInfosToRelative = true;

	FRotator PlayRotation = FRotator::ZeroRotator;
};

/**
 * 
 */
UCLASS()
class ARCHERGAME_API UParticleBlueprintFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	/**
	 * @brief Play particle at given particlePlayingOptions.
	 * @note if you set particlePlayingOptions.bAttachToActor true, given location and rotation will be used as relative
	 * @param context 
	 * @param poolTag 
	 * @param particlePlayingOptions 
	 * @return 
	 */
	UFUNCTION(BlueprintPure, meta=(WorldContext="context"))
	static UParticleSystemComponent* PlayPooledParticle(UObject* context, const FGameplayTag poolTag, const FParticlePlayingOptions& particlePlayingOptions);

private:
	// Set particles location,scale and rotation
	static void SetParticle(AActor* spawnedActor, const FParticlePlayingOptions& particlePlayingOptions);
};
