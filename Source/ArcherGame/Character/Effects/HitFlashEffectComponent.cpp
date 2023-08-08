// Fill out your copyright notice in the Description page of Project Settings.


#include "HitFlashEffectComponent.h"

#include "FCTween.h"


// Sets default values for this component's properties
UHitFlashEffectComponent::UHitFlashEffectComponent(const FObjectInitializer& objectInitializer)
	: Super(objectInitializer)
{
}

void UHitFlashEffectComponent::DoHitFlashBlink()
{
	USkeletalMeshComponent* SkeletalMeshComponent = OnHitEffectParams.SkeletalMeshComponent;
	if (!SkeletalMeshComponent)
	{
		OnHitEffectParams.SkeletalMeshComponent = SkeletalMeshComponent = GetOwner()->FindComponentByClass<USkeletalMeshComponent>();
		check(SkeletalMeshComponent);
	}

	if (!IsValid(MaterialInstance))
	{
		// if there is no material instance, create one
		MaterialInstance = UMaterialInstanceDynamic::Create(OnHitEffectParams.SkeletalMeshComponent->GetMaterial(OnHitEffectParams.MaterialIndex), SkeletalMeshComponent);
		// and set the new material instance dynamic to the correct index
		SkeletalMeshComponent->SetMaterial(OnHitEffectParams.MaterialIndex, MaterialInstance);
	}

	// If there is a previous animation tween, destroy it so it clears
	if (BlinkTweenInstance)
	{
		BlinkTweenInstance->Destroy();
	}

	// Reset scalar value so our effect is always correct
	MaterialInstance->SetScalarParameterValue(OnHitEffectParams.HitBrightnessScalarValueName, 0);

	BlinkTweenInstance = FCTween::Play(0, OnHitEffectParams.Intensity, [&](const float val)
	{
		MaterialInstance->SetScalarParameterValue(OnHitEffectParams.HitBrightnessScalarValueName, val);
	}, OnHitEffectParams.Duration / 2, OnHitEffectParams.Ease);

	BlinkTweenInstance->SetYoyo(true);
}
