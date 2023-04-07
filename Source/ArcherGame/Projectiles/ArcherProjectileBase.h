// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GameFramework/Actor.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
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
	UPROPERTY()
	TObjectPtr<AActor> EffectCauser;

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Projectile|Damage")
	float DamageAmount;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Projectile|Damage")
	TSubclassOf<UArcherGameplayEffect> DamageGameplayEffect;

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
	virtual void DamageOverlappedActor(AActor* otherActor);

	virtual float GetDamageAmount()
	{
		return DamageAmount;
	}

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Projectile|Pooling")
	TObjectPtr<UPoolableComponent> PoolableComponent;

	FTimerHandle ReturnToPoolTimerHandle;
	// calls this actors poolable component's return to pool
	void ReturnToPool();

public:
	// do we want to return to pool when projectile hits something?
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Projectile|Pooling")
	bool bShouldReturnToPoolAfterOverlap = true;
};
