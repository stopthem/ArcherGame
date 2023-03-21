// Fill out your copyright notice in the Description page of Project Settings.


#include "ArrowProjectile.h"
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
	// UE_LOG(LogTemp, Warning, TEXT("notify overlap actor %s other actor %s"), *GetActorNameOrLabel(), *OtherActor->GetActorNameOrLabel());
	Super::NotifyActorBeginOverlap(otherActor);

	// zero out gravity scale so arrow doesnt fall down
	ProjectileMovementComponent->ProjectileGravityScale = 0.0f;

	// attach to overlapped actor
	AttachToActor(otherActor, FAttachmentTransformRules::KeepWorldTransform);

	SetActorLocation(GetActorLocation() + GetActorForwardVector() * stickLocationMultiplier);
}

void AArrowProjectile::PlayHitParticle(AActor* otherActor)
{
	// play particle at hit actor(for attaching) but our location and rotations
	FParticlePlayingOptions particlePlayingOptions(otherActor);
	particlePlayingOptions.ParticleAttachmentRules = EParticleAttachmentRules::AttachGivenValuesAreWorld;
	particlePlayingOptions.PlayLocation = GetActorLocation() + GetActorForwardVector() * stickLocationMultiplier;
	particlePlayingOptions.PlayRotation = ProjectileHitParticleInfo.bUseActorRotation ? GetActorRotation() : FRotator::ZeroRotator;

	ProjectileHitParticleInfo.PlayParticle(particlePlayingOptions);
}
