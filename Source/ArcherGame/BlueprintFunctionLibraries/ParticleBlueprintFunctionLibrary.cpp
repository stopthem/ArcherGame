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

	SetParticle(particleActor, particlePlayingOptions);

	UParticleSystemComponent* particleSystemComponent = particleActor->FindComponentByClass<UParticleSystemComponent>();
	particleSystemComponent->Activate();

	return particleSystemComponent;
}

void UParticleBlueprintFunctionLibrary::SetParticle(AActor* spawnedActor, const FParticlePlayingOptions& particlePlayingOptions)
{
	spawnedActor->SetActorScale3D(particlePlayingOptions.PlayScale);
	// attach to actor if needed and set its relative location,rotation and scale
	if (particlePlayingOptions.bAttachToActor)
	{
		spawnedActor->AttachToActor(particlePlayingOptions.PlayActor, FAttachmentTransformRules::KeepWorldTransform);

		spawnedActor->SetActorRelativeLocation(particlePlayingOptions.bConvertInfosToRelative
			                                       ? UKismetMathLibrary::InverseTransformLocation(particlePlayingOptions.PlayActor->GetTransform(), particlePlayingOptions.PlayLocation)
			                                       : particlePlayingOptions.PlayLocation);
		spawnedActor->SetActorRelativeRotation(particlePlayingOptions.bConvertInfosToRelative
			                                       ? UKismetMathLibrary::InverseTransformRotation(particlePlayingOptions.PlayActor->GetTransform(), particlePlayingOptions.PlayRotation)
			                                       : particlePlayingOptions.PlayRotation);
	}
	else
	{
		// if not set its relative location,rotation and scale
		spawnedActor->SetActorLocationAndRotation(particlePlayingOptions.PlayLocation, particlePlayingOptions.PlayRotation);
	}
}
