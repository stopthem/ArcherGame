// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PoolableComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "ParticlePoolableComponent.generated.h"

/**
 * UParticlePoolableComponent
 *
 * This component is specific to particle systems.
 * What it does extra is, return to pool when the particle system finished
 */
UCLASS()
class POOLING_API UParticlePoolableComponent : public UPoolableComponent
{
	GENERATED_BODY()

	UParticlePoolableComponent();

	// get particle system if null and bind to particle system component's on system finished 
	virtual void Taken() override;

private:
	TObjectPtr<UParticleSystemComponent> ParticleSystemComponent;

	// when the system is finished, return to pool
	UFUNCTION()
	void OnParticleSystemFinished(UParticleSystemComponent* finishedComponent)
	{
		ParticleSystemComponent->Deactivate();
		ReturnToPool();
	}
};
