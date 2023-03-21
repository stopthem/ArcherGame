// Fill out your copyright notice in the Description page of Project Settings.


#include "ParticlePoolableComponent.h"
#include "Particles/ParticleSystemComponent.h"

UParticlePoolableComponent::UParticlePoolableComponent()
{
}

void UParticlePoolableComponent::Taken()
{
	Super::Taken();

	if (!ParticleSystemComponent)
	{
		ParticleSystemComponent = GetOwner()->FindComponentByClass<UParticleSystemComponent>();

		ParticleSystemComponent->OnSystemFinished.AddDynamic(this, &UParticlePoolableComponent::OnParticleSystemFinished);
	}
}
