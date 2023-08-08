// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FCTweenUObject.h"
#include "ArcherGame/Character/ArcherCharacter.h"

#include "ArcherEnemyCharacter.generated.h"

class UHitFlashEffectComponent;
/**
 * AArcherEnemyCharacter
 *
 * Base class that all enemies use.
 */
UCLASS(Abstract)
class ARCHERGAME_API AArcherEnemyCharacter : public AArcherCharacter
{
	GENERATED_BODY()

public:
	AArcherEnemyCharacter();

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "ArcherEnemyCharacter|Effects", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UHitFlashEffectComponent> HitFlashEffectComponent;
	
	// do blink of IOnHitFlashEffect
	UFUNCTION()
	void OnHealthChanged(UArcherHealthComponent* sentHealthComponent, float oldValue, float newValue, AActor* sentInstigator);

#pragma region Spawning

public:
	// Params that scaling tween will use.
	UPROPERTY(EditAnywhere, Category="ArcherEnemyCharacter|Spawn")
	FTweenParams SpawnScaleTweenParams;

	// The vfx that will play when spawned
	UPROPERTY(EditAnywhere, Category="ArcherEnemyCharacter|Spawn")
	TObjectPtr<UParticleSystem> SpawnVFX;

	virtual void Spawn();
#pragma endregion

protected:
	virtual void BeginPlay() override;
};
