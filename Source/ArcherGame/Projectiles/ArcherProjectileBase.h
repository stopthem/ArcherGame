// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FCTweenUObject.h"
#include "GameplayTagContainer.h"
#include "ArcherGame/Character/Ability/ArcherAbilitySystemComponent.h"
#include "GameFramework/Actor.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "ArcherProjectileBase.generated.h"

struct FParticlePlayingOptions;
class UPoolableComponent;
class UArcherGameplayEffect;
class UParticleBlueprintFunctionLibrary;
class UPoolerBlueprintFunctionLibrary;
/**
 * FProjectileHitParticleInfo
 *
 * Struct of what kind of action needs to be taken about particles when projectile hit something
 */
USTRUCT(BlueprintType)
struct FProjectileHitParticleInfo
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Projectile|VFX")
	bool bPlayPooledParticle = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Projectile|VFX", meta=(EditCondition="bPlayPooledParticle == true", EditConditionHides))
	FGameplayTag HitVfxPoolerTag;

	// If null, HitVfxPoolerTag will be used on players.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Projectile|VFX", meta=(EditCondition="bPlayPooledParticle == true", EditConditionHides))
	FGameplayTag PlayerHitVfxPoolerTag;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Projectile|VFX", meta=(EditCondition="bPlayPooledParticle == false", EditConditionHides))
	UParticleSystem* HitVfx = nullptr;

	// If null, HitVfx will be used on players.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Projectile|VFX", meta=(EditCondition="bPlayPooledParticle == false", EditConditionHides))
	UParticleSystem* PlayerHitVfx = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Projectile|VFX")
	bool bUseActorRotation = false;

	UParticleSystemComponent* PlayHitParticle(const AActor* hitter, const AActor* hitActor, FParticlePlayingOptions particlePlayingOptions, bool bCanAttach = false, bool bRotateTowardsFoundBone = true,
	                                          EAttachLocation::Type attachLocation = EAttachLocation::KeepWorldPosition) const;
};

/**
 * AArcherProjectileBase
 *
 * Base Projectile class that this game uses
 */
UCLASS()
class ARCHERGAME_API AArcherProjectileBase : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AArcherProjectileBase();

public:
	UFUNCTION(BlueprintCallable)
	virtual void Shoot(AActor* projectileInstigator);

	UFUNCTION(BlueprintCallable)
	void SetTimerForDestroy(float time);

private:
	// Probably spawner of this object.
	UPROPERTY()
	TObjectPtr<AActor> ProjectileInstigator;

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Projectile|Damage")
	float DamageAmount;

public:
	// Does projectile move until blocked by a collision after damaging collisions are full?
	UPROPERTY(EditAnywhere, Category="Projectile|Damaging Collisions", meta=(UIMin="1", ClampMin="1"))
	bool bDoesMoveUntilBlockedAfterDamagingCollisionsFull = false;

	// How many objects this projectile can damage
	UPROPERTY(EditAnywhere, Category="Projectile|Damaging Collisions", meta=(UIMin="1", ClampMin="1"))
	int HowManyDamagingCollisions = 1;

private:
	// Current damaging collision count
	int DamagingCollisionCount;

public:
	// This will be applied to found ability system component
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Projectile|Damage")
	TSubclassOf<UArcherGameplayEffect> DamageGameplayEffect;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Projectile|Movement")
	TObjectPtr<UProjectileMovementComponent> ProjectileMovementComponent;

public:
	// The main hit vfx info
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Projectile|VFX")
	FProjectileHitParticleInfo ProjectileHitParticleInfo;

protected:
	FRotator GetProjectileHitRotation() const
	{
		return ProjectileHitParticleInfo.bUseActorRotation ? GetActorRotation() : FRotator(0);
	}

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	UPrimitiveComponent* GetCollisionComponent() const
	{
		return ProjectileCollisionComponent;
	}

protected:
	// Our collision component
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Projectile|Mesh")
	TObjectPtr<UPrimitiveComponent> ProjectileCollisionComponent;

	UFUNCTION()
	virtual void OnBeginOverlap(UPrimitiveComponent* overlappedComponent, AActor* otherActor, UPrimitiveComponent* otherComponent, int32 otherBodyIndex, bool bFromSweep, const FHitResult& hit);

	virtual void PlayHitParticle(AActor* otherActor);

protected:
	virtual bool DamageOverlappedActor(AActor* otherActor);

	virtual float GetDamageAmount()
	{
		return DamageAmount;
	}

public:
	// Should we destroy the particle after TimeBeforeHandlingEnd passed ?
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Projectile|End")
	bool bDestroyAfterTime = true;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Projectile|End", meta=(EditCondition="bDestroyAfterTime == true"))
	float TimeBeforeHandlingEnd = 5.0f;

	// Tween params to use in ending scale tween.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Projectile|End")
	FTweenParams DestroyEmitterScaleTweenParams;

	// Return to pool or destroy actor.
	UFUNCTION(BlueprintCallable)
	void HandleActorEnding();

private:
	TObjectPtr<FCTweenInstance> EndScaleTween = nullptr;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Projectile|Pooling")
	TObjectPtr<UPoolableComponent> PoolableComponent;

	FTimerHandle ReturnToPoolTimerHandle;

	// calls this actors poolable component's return to pool
	virtual void ReturnToPool();

public:
	// do we want to return to pool when projectile hits something?
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Projectile|Pooling")
	bool bShouldReturnToPoolAfterOverlap = true;
};
