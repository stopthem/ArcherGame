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

/**
 * FParticlePlayingOptions
 *
 * Stores information about location, scale, rotation, attached actor about to be played particle system
 */
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

	explicit FParticlePlayingOptions(AActor* playActor, USkinnedMeshComponent* skinnedMeshComponent, FName socketName)
	{
		SocketName = socketName;
		SkinnedMeshComponent = skinnedMeshComponent;
	}

	FName SocketName;
	TObjectPtr<USkinnedMeshComponent> SkinnedMeshComponent;

public:
	TObjectPtr<AActor> PlayActor = nullptr;

	FVector PlayLocation = FVector::ZeroVector;

	FVector PlayScale = FVector::OneVector;

	EParticleAttachmentRules ParticleAttachmentRules = EParticleAttachmentRules::DontAttach;

	FRotator PlayRotation = FRotator::ZeroRotator;
};

/**
 * UParticleBlueprintFunctionLibrary
 *
 * A blueprint function library class for Particle functions
 */
UCLASS()
class ARCHERGAME_API UParticleBlueprintFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	/**
	 * @brief Play pooled particle from given poolTag's found pooler
	 * @param context The world context which will be auto filled by blueprint but not on c++
	 * @param poolTag The pool tag of the pooler which pools the particles in world
	 * @param particlePlayingOptions Location, rotation, scale and attached actor information
	 * @return The spawned particle
	 */
	UFUNCTION(BlueprintPure, meta=(WorldContext="context"))
	static UParticleSystemComponent* PlayPooledParticle(UObject* context, const FGameplayTag poolTag, const FParticlePlayingOptions& particlePlayingOptions);

private:
	// Set particles location,scale and rotation
	static void SetParticle(AActor* spawnedActor, const FParticlePlayingOptions& particlePlayingOptions);
};
