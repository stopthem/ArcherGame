// Fill out your copyright notice in the Description page of Project Settings.


#include "ArrowProjectile.h"

#include "ArcherGame/BlueprintFunctionLibraries/ParticleBlueprintFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AArrowProjectile::AArrowProjectile()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	// PrimaryActorTick.bCanEverTick = true;
}

void AArrowProjectile::Shoot(AActor* effectCauser)
{
	ProjectileCollisionComponent->SetGenerateOverlapEvents(true);

	Super::Shoot(effectCauser);
}

void AArrowProjectile::OnBeginOverlap(UPrimitiveComponent* overlappedComponent, AActor* otherActor, UPrimitiveComponent* otherComponent, int32 otherBodyIndex, bool bFromSweep, const FHitResult& hit)
{
	SetActorLocation(GetActorLocation() + GetActorForwardVector() * stickLocationMultiplier);

	Super::OnBeginOverlap(overlappedComponent, otherActor, otherComponent, otherBodyIndex, bFromSweep, hit);
}

// play particle at hit actor(for attaching) but our location and rotations
void AArrowProjectile::PlayHitParticle(AActor* otherActor)
{
	// create particle playing options for using its variables multiple times. playing particle can be done without it 
	FParticlePlayingOptions particlePlayingOptions;

	particlePlayingOptions.PlayLocation = GetActorLocation();

	particlePlayingOptions.PlayRotation = GetProjectileHitRotation();

	if (USkinnedMeshComponent* otherActorMesh = otherActor->FindComponentByClass<USkinnedMeshComponent>())
	{
		FVector* socketLocation = nullptr;
		const FName socketName = otherActorMesh->FindClosestBone(GetActorLocation(), socketLocation);

		if (socketName.IsNone())
		{
			UE_LOG(LogTemp, Warning, TEXT("ArrowProjectile -> couldn't find closest bone this actor %s overlapped actor %s"), *GetActorNameOrLabel(), *otherActor->GetActorNameOrLabel());
			return;
		}

		UGameplayStatics::SpawnEmitterAttached(ProjectileHitParticleInfo.HitVfx, otherActorMesh, socketName, particlePlayingOptions.PlayLocation,
		                                       particlePlayingOptions.PlayRotation, FVector(1), EAttachLocation::KeepWorldPosition);
	}
	else
	{
		UGameplayStatics::SpawnEmitterAtLocation(this, ProjectileHitParticleInfo.HitVfx, particlePlayingOptions.PlayLocation, particlePlayingOptions.PlayRotation);
	}
}
