// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "ParticleBlueprintFunctionLibrary.generated.h"

UENUM()
enum class EParticleAttachmentRules
{
	DontAttach,
	// Given Location and Rotation values will be treated like relative after attaching to actor
	AttachGivenValuesAreRelative,
	// Given Location and Rotation values will be treated like world and will be converted to relative after attaching
	AttachGivenValuesAreWorld
};

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

	EParticleAttachmentRules ParticleAttachmentRules = EParticleAttachmentRules::DontAttach;

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
	// Play pooled particle from given poolTag's found pooler. 
	UFUNCTION(BlueprintPure, meta=(WorldContext="context"))
	static UParticleSystemComponent* PlayPooledParticle(UObject* context, const FGameplayTag poolTag, const FParticlePlayingOptions& particlePlayingOptions);

private:
	// Set particles location,scale and rotation
	static void SetParticle(AActor* spawnedActor, const FParticlePlayingOptions& particlePlayingOptions);
};
