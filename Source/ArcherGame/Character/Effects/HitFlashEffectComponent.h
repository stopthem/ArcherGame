// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FCEasing.h"
#include "Components/ActorComponent.h"
#include "Components/GameFrameworkComponent.h"
#include "HitFlashEffectComponent.generated.h"

class FCTweenInstanceFloat;
/* FOnHitFlashEffectParams
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

public:
	// The mesh where we find the material and create material dynamic instance
	UPROPERTY()
	TObjectPtr<USkeletalMeshComponent> SkeletalMeshComponent = nullptr;

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

/*
 * UHitFlashEffectComponent
 *
 * Component that handles the hit effect of paragon characters.
 */
UCLASS(Blueprintable, meta=(BlueprintSpawnableComponent))
class ARCHERGAME_API UHitFlashEffectComponent : public UGameFrameworkComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	explicit UHitFlashEffectComponent(const FObjectInitializer& objectInitializer);

public:
	void DoHitFlashBlink();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="OnHitEffect")
	FOnHitFlashEffectParams OnHitEffectParams;

private:
	// created material instance from
	UPROPERTY()
	TObjectPtr<UMaterialInstanceDynamic> MaterialInstance;

	// Animation tween
	TObjectPtr<FCTweenInstanceFloat> BlinkTweenInstance;
};
