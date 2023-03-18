// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "ArcherProjectileBase.generated.h"

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

protected:
	virtual void PostInitializeComponents() override;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Projectile|Mesh")
	TObjectPtr<UStaticMeshComponent> ProjectileMesh;
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Pooling")
	TObjectPtr<class UPoolableComponent> PoolableComponent;

	void ReturnToPool();

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Pooling")
	bool bShouldReturnToPoolAfterOverlap = true;
};
