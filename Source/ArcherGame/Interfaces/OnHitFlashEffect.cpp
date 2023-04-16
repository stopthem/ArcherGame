// Fill out your copyright notice in the Description page of Project Settings.


#include "OnHitFlashEffect.h"
#include "FCTween.h"
#include "FCTweenUObject.h"

IOnHitFlashEffect::IOnHitFlashEffect()
{
}

void IOnHitFlashEffect::DoHitFlashBlink()
{
	// if there is no material instance, create one
	if (MaterialInstance == nullptr)
	{
		MaterialInstance = OnHitEffectParams.Mesh->CreateDynamicMaterialInstance(OnHitEffectParams.MaterialIndex, OnHitEffectParams.Mesh->GetMaterial(OnHitEffectParams.MaterialIndex));
	}

	// If there is a previous animation tween, destroy it so it clears
	if (BlinkTweenInstance)
	{
		BlinkTweenInstance->Destroy();
	}

	BlinkTweenInstance = FCTween::Play(0, OnHitEffectParams.Intensity, [&](float val)
	{
		MaterialInstance->SetScalarParameterValue(OnHitEffectParams.HitBrightnessScalarValueName, val);
	}, OnHitEffectParams.Duration / 2, OnHitEffectParams.Ease);

	BlinkTweenInstance->SetYoyo(true);
}
