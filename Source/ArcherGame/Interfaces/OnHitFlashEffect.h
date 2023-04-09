// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FCEasing.h"

#include "OnHitFlashEffect.generated.h"

/*
 * FOnHitFlashEffectParams
 *
 * Used by IOnHitFlashEffect.
 * Holds information about material and blinking interpolation
 */
USTRUCT(BlueprintType)
struct FOnHitFlashEffectParams
{
	GENERATED_BODY()

public:
	FOnHitFlashEffectParams()
	{
	}

	FOnHitFlashEffectParams(UPrimitiveComponent* mesh, int materialIndex, FName hitBrightnessScalarValueName)
	{
		Mesh = mesh;
		MaterialIndex = materialIndex;
		HitBrightnessScalarValueName = hitBrightnessScalarValueName;
	}

public:
	// The mesh where we find the material and create material dynamic instance
	UPROPERTY()
	TObjectPtr<UPrimitiveComponent> Mesh = nullptr;

	// To be copied material index
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="OnHitParams|Init")
	int MaterialIndex = 0;

	// To be changed scalar value name 
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="OnHitParams|Init")
	FName HitBrightnessScalarValueName = "HitFlashBrightness";

public:
	// Duration of the blinking animation. Will be considered as full length because animation is YoYo
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="OnHitParams|Play")
	float Duration = 0.1f;

	// Intensity of the blink animation. Basically the scalar value's target
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="OnHitParams|Play")
	float Intensity = 6.5f;

	// Ease of the blinking animation
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="OnHitParams|Play")
	EFCEase Ease = EFCEase::InOutSine;
};

class FCTweenInstanceFloat;
enum class EFCEase : uint8;

UINTERFACE(MinimalAPI, Blueprintable)
class UOnHitFlashEffect : public UInterface
{
	GENERATED_BODY()
};

/*
 * IOnHitFlashEffect
 *
 * Interface that plays a blinking animation on given meshes given material with changing the scalar value of given name
 */
class IOnHitFlashEffect
{
	GENERATED_BODY()

public:
	IOnHitFlashEffect();

protected:
	void DoHitFlashBlink();

	FOnHitFlashEffectParams OnHitEffectParams;

private:
	// created material instance from 
	TWeakObjectPtr<UMaterialInstanceDynamic> MaterialInstance;

	// Animation tween
	TObjectPtr<FCTweenInstanceFloat> BlinkTweenInstance;
};
