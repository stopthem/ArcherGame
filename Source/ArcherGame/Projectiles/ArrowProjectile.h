// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ArcherProjectileBase.h"
#include "ArrowProjectile.generated.h"

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

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Pooling")
	float timeAfterOverlapReturnToPool = 5.0f;

	virtual void Shoot() const override;

protected:
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

private:
	FTimerHandle ReturnToPoolTimerHandle;
};
