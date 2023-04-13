// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FCTweenUObject.h"
#include "GameplayTagContainer.h"
#include "GameFramework/Actor.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "ArcherProjectileBase.generated.h"

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

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Projectile|VFX", meta=(EditCondition="bPlayPooledParticle == false", EditConditionHides))
	UParticleSystem* HitVfx = nullptr;

	TObjectPtr<FCTweenInstance> SpawnedTween = nullptr;

	// OneValue will be used as time before destroying the projectile 
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Projectile|VFX")
	FTweenParamsOneValue DestroyEmitterScaleTweenParams;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Projectile|VFX")
	bool bUseActorRotation = false;
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
	virtual void Shoot(AActor* effectCauser);

private:
	// Probably spawner of this object.
	UPROPERTY()
	TObjectPtr<AActor> EffectCauser;

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Projectile|Damage")
	float DamageAmount;

public:
	UPROPERTY(EditAnywhere, Category="Projectile|Damaging Collisions", meta=(UIMin="1", ClampMin="1"))
	bool bDoesMoveUntilBlockedAfterDamagingCollisionsFull = false;
	// How many objects this projectile can damage
	UPROPERTY(EditAnywhere, Category="Projectile|Damaging Collisions", meta=(UIMin="1", ClampMin="1"))
	int HowManyDamagingCollisions = 1;

private:
	int DamagingCollisionCount;

public:
	// This will be applied to found ability system component
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Projectile|Damage")
	TSubclassOf<UArcherGameplayEffect> DamageGameplayEffect;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Projectile|Movement")
	TObjectPtr<UProjectileMovementComponent> ProjectileMovementComponent;

	// The main hit vfx info
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Projectile|VFX")
	FProjectileHitParticleInfo ProjectileHitParticleInfo;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

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

private:
	void HandleActorEnding();

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
