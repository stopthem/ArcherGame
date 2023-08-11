// Fill out your copyright notice in the Description page of Project Settings.


#include "Pooler.h"

#include "ParticlePoolableComponent.h"
#include "PoolableComponent.h"
#include "PoolerSubsystem.h"

APooler::APooler()
{
}

void APooler::BeginPlay()
{
	Super::BeginPlay();

	for (int i = 0; i < SpawnAtStart; ++i)
	{
		SpawnPooledActor();
	}

	GetWorld()->GetSubsystem<UPoolerSubsystem>()->AddToPoolerArray(this);
}

AActor* APooler::SpawnPooledActor()
{
	if (!GetWorld())return nullptr;

	// Spawn the to be pooled actor
	AActor* spawnedActor = GetWorld()->SpawnActor<AActor>(ActorToPool, FVector::ZeroVector, FRotator::ZeroRotator);

	// Reset spawnedActor
	ResetPooledObj(spawnedActor);

	// Is UPoolableComponent added manually?
	UPoolableComponent* poolableComponent = spawnedActor->FindComponentByClass<UPoolableComponent>();
	// If not create one
	if (!poolableComponent)
	{
		UClass* poolableComponentClassToAdd = bPoolingParticle ? UParticlePoolableComponent::StaticClass() : UPoolableComponent::StaticClass();

		poolableComponent = Cast<UPoolableComponent>(spawnedActor->AddComponentByClass(poolableComponentClassToAdd, true, spawnedActor->GetTransform(), true));
	}

	// Add spawnedActor's UPoolableComponent to the list
	PooledActors.Add(poolableComponent);
	// Init this pooler to poolableComponent
	poolableComponent->Init(this);
	return spawnedActor;
}


// Make given actor reset to a newly spawned state
void APooler::ResetPooledObj(AActor* pooledActor)
{
	const FAttachmentTransformRules attachmentTransformRules = FAttachmentTransformRules(EAttachmentRule::KeepWorld, false);
	pooledActor->AttachToActor(this, attachmentTransformRules);

	// Set spawned actor world outliner folder and hide it
	pooledActor->SetFolderPath(GetFolderPath());
	pooledActor->SetActorHiddenInGame(true);

	// Reset position, scale and rotation
	pooledActor->SetActorRelativeLocation(FVector::ZeroVector);
	pooledActor->SetActorScale3D(FVector::OneVector);
	pooledActor->SetActorRotation(FRotator::ZeroRotator);
}


// Get pooled object from the pool
AActor* APooler::GetPooledObj()
{
	UPoolableComponent* poolableComponent;

	// is there an available pooled actor?
	if (const auto foundPoolableComponent = PooledActors.FindByPredicate([](const UPoolableComponent* poolableComponent)
	{
		return !poolableComponent->GetIsTaken();
	}))
	{
		poolableComponent = *foundPoolableComponent;
	}
	else
	{
		// if not spawn one
		const AActor* spawnedActor = SpawnPooledActor();
		poolableComponent = spawnedActor->FindComponentByClass<UPoolableComponent>();
	}

	poolableComponent->Activate();
	poolableComponent->Taken();
	AActor* returnedActor = poolableComponent->GetOwner();
	returnedActor->SetActorHiddenInGame(false);

	return returnedActor;
}

void APooler::ReturnToPool(const UPoolableComponent* poolableComponent)
{
	// does given poolableComponent is pooled by this pooler ?
	if (!PooledActors.Contains(poolableComponent))return;

	ResetPooledObj(poolableComponent->GetOwner());
}
