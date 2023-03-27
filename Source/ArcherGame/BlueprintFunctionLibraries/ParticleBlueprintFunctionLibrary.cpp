// Fill out your copyright notice in the Description page of Project Settings.


#include "ParticleBlueprintFunctionLibrary.h"

#include "Pooler.h"
#include "Kismet/KismetMathLibrary.h"
#include "Particles/ParticleSystemComponent.h"
#include "Pooling/Public/PoolerBlueprintFunctionLibrary.h"

UParticleSystemComponent* UParticleBlueprintFunctionLibrary::PlayPooledParticle(UObject* context, const FGameplayTag poolTag, const FParticlePlayingOptions& particlePlayingOptions)
{
	// get pooler
	APooler* particlePooler = UPoolerBlueprintFunctionLibrary::GetPoolerByGameplayTag(context, poolTag);

	// get particle
	AActor* particleActor = particlePooler->GetPooledObj();

	// Set particle scale, location, owning actor and rotation
	SetParticle(particleActor, particlePlayingOptions);

	UParticleSystemComponent* particleSystemComponent = particleActor->FindComponentByClass<UParticleSystemComponent>();
	particleSystemComponent->Activate();

	return particleSystemComponent;
}

void UParticleBlueprintFunctionLibrary::SetParticle(AActor* spawnedActor, const FParticlePlayingOptions& particlePlayingOptions)
{
	spawnedActor->SetActorScale3D(particlePlayingOptions.PlayScale);

	// attach to actor if needed and set its relative location and rotation
	if (particlePlayingOptions.ParticleAttachmentRules != EParticleAttachmentRules::DontAttach)
	{
		spawnedActor->AttachToActor(particlePlayingOptions.PlayActor, FAttachmentTransformRules::KeepWorldTransform);

		// If ParticleAttachmentRules of the given particlePlayingOptions is AttachGivenValuesAreWorld, treat given location and rotation values like world values and transform that values to relative

		const FVector particleLocation = particlePlayingOptions.ParticleAttachmentRules == EParticleAttachmentRules::AttachGivenValuesAreWorld
			                                 ? UKismetMathLibrary::InverseTransformLocation(particlePlayingOptions.PlayActor->GetTransform(), particlePlayingOptions.PlayLocation)
			                                 : particlePlayingOptions.PlayLocation;

		const FRotator particleRotation = particlePlayingOptions.ParticleAttachmentRules == EParticleAttachmentRules::AttachGivenValuesAreWorld
			                                  ? UKismetMathLibrary::InverseTransformRotation(particlePlayingOptions.PlayActor->GetTransform(), particlePlayingOptions.PlayRotation)
			                                  : particlePlayingOptions.PlayRotation;

		spawnedActor->SetActorRelativeLocation(particleLocation);
		spawnedActor->SetActorRelativeRotation(particleRotation);
	}
	else
	{
		// if not set its relative location and rotation
		spawnedActor->SetActorLocationAndRotation(particlePlayingOptions.PlayLocation, particlePlayingOptions.PlayRotation);
	}
}
