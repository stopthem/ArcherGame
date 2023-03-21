// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "ArcherGame/BlueprintFunctionLibraries/ParticleBlueprintFunctionLibrary.h"
#include "GameFramework/Actor.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "ArcherProjectileBase.generated.h"

// class UGameplayStatics;
class UParticleBlueprintFunctionLibrary;
class UPoolerBlueprintFunctionLibrary;
// Struct of what kind of action needs to be taken about particles when projectile hit something
USTRUCT(BlueprintType)
struct FProjectileHitParticleInfo
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Projectile|VFX")
	bool bPlayPooledParticle = true;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Projectile|VFX", meta=(EditCondition="bPlayPooledParticle == true", EditConditionHides))
	FGameplayTag HitVfxPoolerTag;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Projectile|VFX", meta=(EditCondition="bPlayPooledParticle == false", EditConditionHides))
	UParticleSystem* HitVfx;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Projectile|VFX")
	bool bUseActorRotation = false;

	void PlayParticle(FParticlePlayingOptions particlePlayingOptions) const
	{
		if (bPlayPooledParticle)
		{
			UParticleBlueprintFunctionLibrary::PlayPooledParticle(particlePlayingOptions.PlayActor, HitVfxPoolerTag, particlePlayingOptions);
		}
		else
		{
			UGameplayStatics::SpawnEmitterAtLocation(particlePlayingOptions.PlayActor, HitVfx, particlePlayingOptions.PlayActor->GetActorLocation(),
			                                         bUseActorRotation ? particlePlayingOptions.PlayActor->GetActorRotation() : FRotator::ZeroRotator);
		}
	}
};

UCLASS()
class ARCHERGAME_API AArcherProjectileBase : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AArcherProjectileBase();

	UFUNCTION(BlueprintCallable)
	virtual void Shoot() const;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Projectile|Movement")
	TObjectPtr<UProjectileMovementComponent> ProjectileMovementComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Projectile|VFX")
	FProjectileHitParticleInfo ProjectileHitParticleInfo;

protected:
	virtual void PostInitializeComponents() override;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Projectile|Mesh")
	TObjectPtr<UStaticMeshComponent> ProjectileMesh;
	virtual void NotifyActorBeginOverlap(AActor* otherActor) override;

	virtual void PlayHitParticle(AActor* otherActor);

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Projectile|Pooling")
	TObjectPtr<class UPoolableComponent> PoolableComponent;

	// calls this actors poolable component's return to pool
	void ReturnToPool();

public:
	// do we want to return to pool when projectile hits something?
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Projectile|Pooling")
	bool bShouldReturnToPoolAfterOverlap = true;
};
