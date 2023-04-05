// Fill out your copyright notice in the Description page of Project Settings.


#include "ArrowProjectile.h"

#include "ArcherGame/BlueprintFunctionLibraries/ParticleBlueprintFunctionLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"

// Sets default values
AArrowProjectile::AArrowProjectile()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	// PrimaryActorTick.bCanEverTick = true;
}

void AArrowProjectile::Shoot() const
{
	// UE_LOG(LogTemp, Warning, TEXT("timer remaining %f shot actor %s"), GetWorld()->GetTimerManager().GetTimerRemaining(ReturnToPoolTimerHandle), *GetActorNameOrLabel());

	// if this projectile was used once, this might be zeroed out
	ProjectileMovementComponent->ProjectileGravityScale = 1.0f;

	Super::Shoot();
}

void AArrowProjectile::NotifyActorBeginOverlap(AActor* otherActor)
{
	SetActorLocation(GetActorLocation() + GetActorForwardVector() * stickLocationMultiplier);

	// UE_LOG(LogTemp, Warning, TEXT("notify overlap actor %s other actor %s"), *GetActorNameOrLabel(), *OtherActor->GetActorNameOrLabel());
	Super::NotifyActorBeginOverlap(otherActor);

	// zero out gravity scale so arrow doesn't fall down
	ProjectileMovementComponent->ProjectileGravityScale = 0.0f;
}

// play particle at hit actor(for attaching) but our location and rotations
void AArrowProjectile::PlayHitParticle(AActor* otherActor)
{
	// create particle playing options for using its variables multiple times. playing particle can be done without it 
	FParticlePlayingOptions particlePlayingOptions;

	particlePlayingOptions.PlayLocation = GetActorLocation();

	particlePlayingOptions.PlayRotation = ProjectileHitParticleInfo.bUseActorRotation ? GetActorRotation() : FRotator(0);

	if (USkinnedMeshComponent* otherActorMesh = otherActor->FindComponentByClass<USkinnedMeshComponent>())
	{
		FVector* socketLocation = nullptr;
		const FName socketName = otherActorMesh->FindClosestBone(GetActorLocation(), socketLocation);

		if (socketName.IsNone())
		{
			UE_LOG(LogTemp, Warning, TEXT("couldn't find closest bone in actor %s overlapped actor %s"), *GetActorNameOrLabel(), *otherActor->GetActorNameOrLabel());
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
