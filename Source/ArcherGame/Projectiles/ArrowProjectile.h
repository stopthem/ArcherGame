// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ArcherProjectileBase.h"
#include "ArrowProjectile.generated.h"

/**
 * AArrowProjectile
 *
 * Arrow projectile class which player fires
 */
UCLASS()
class ARCHERGAME_API AArrowProjectile : public AArcherProjectileBase
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AArrowProjectile();

	// Arrow will go ActorForwardVector * this value when overlapped something
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Arrow Projectile")
	float stickLocationMultiplier = 25.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Arrow Projectile", meta=(UIMin= "0", UIMax="1", ClampMin="0", ClampMax="1"))
	float ShootingPower;

	virtual void Shoot(AActor* effectCauser) override;

protected:
	virtual void NotifyActorBeginOverlap(AActor* otherActor) override;

	// play particle at hit actor(for attaching) but our location and rotations
	virtual void PlayHitParticle(AActor* otherActor) override;

protected:
	virtual float GetDamageAmount() override { return Super::GetDamageAmount() * ShootingPower; }
};
