// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PoolableComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "ParticlePoolableComponent.generated.h"

/**
 * 
 */
UCLASS()
class POOLING_API UParticlePoolableComponent : public UPoolableComponent
{
	GENERATED_BODY()

	UParticlePoolableComponent();

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
