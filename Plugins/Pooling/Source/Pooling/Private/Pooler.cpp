// Fill out your copyright notice in the Description page of Project Settings.


#include "Pooler.h"

#include "PoolableComponent.h"
#include "Kismet/KismetSystemLibrary.h"

// Spawn the to be pooled objects
void APooler::BeginPlay()
{
	Super::BeginPlay();

	// Get world
	UWorld* world = GetWorld();

	for (int i = 0; i < SpawnAtStart; ++i)
	{
		// Spawn the to be pooled actor
		AActor* spawnedActor = world->SpawnActor<AActor>(ActorToPool, FVector::ZeroVector, FRotator::ZeroRotator);

		// Reset spawnedActor
		ResetPooledObj(spawnedActor);

		// Is UPoolableComponent added manually?
		UPoolableComponent* poolableComponent = spawnedActor->FindComponentByClass<UPoolableComponent>();
		// If not create one
		if (!poolableComponent)
		{
			poolableComponent = Cast<UPoolableComponent>(spawnedActor->AddComponentByClass(UPoolableComponent::StaticClass(), true, spawnedActor->GetTransform(), true));
		}

		// Add spawnedActor's UPoolableComponent to the list
		PooledActors.Add(poolableComponent);
		// Init this pooler to poolableComponent
		poolableComponent->Init(this);
	}
}

void APooler::ResetPooledObj(AActor* pooledActor)
{
	const FAttachmentTransformRules attachmentTransformRules = FAttachmentTransformRules(EAttachmentRule::KeepWorld, false);
	pooledActor->AttachToActor(this, attachmentTransformRules);

	// Set spawned actor world outliner folder and hide it
	pooledActor->SetFolderPath(FName("/Pools/" + UKismetSystemLibrary::GetDisplayName(this)));
	pooledActor->SetActorHiddenInGame(true);

	// Reset position, scale and rotation
	pooledActor->SetActorRelativeLocation(FVector::ZeroVector);
	pooledActor->SetActorScale3D(ActorToPool.GetDefaultObject()->GetActorScale());
	pooledActor->SetActorRotation(FRotator::ZeroRotator);
}

// Get pooled object from the pool
AActor* APooler::GetPooledObj()
{
	const TObjectPtr<UPoolableComponent>* foundPoolableComponent = PooledActors.FindByPredicate([&](const UPoolableComponent* poolableComponent)
	{
		return !poolableComponent->GetIsTaken();
	});

	UPoolableComponent* returnedPoolableComponent = *foundPoolableComponent;
	returnedPoolableComponent->Taken();

	AActor* returnedActor = returnedPoolableComponent->GetOwner();
	returnedActor->SetActorHiddenInGame(false);

	return returnedActor;
}

void APooler::ReturnToPool(const UPoolableComponent* poolableComponent)
{
	if (!PooledActors.Contains(poolableComponent))return;

	ResetPooledObj(poolableComponent->GetOwner());
}
