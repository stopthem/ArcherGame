// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ArcherGame/Character/ArcherCharacter.h"
#include "ArcherGame/Character/Ability/Attribute/ArcherCombatSet.h"
#include "..\..\Interfaces\OnHitFlashEffect.h"

#include "ArcherEnemyCharacter.generated.h"

/**
 * AArcherEnemyCharacter
 *
 * Base class that all enemies use.
 */
UCLASS()
class ARCHERGAME_API AArcherEnemyCharacter : public AArcherCharacter, public IOnHitFlashEffect
{
	GENERATED_BODY()

public:
	AArcherEnemyCharacter();

#pragma region OnHitFlashInterface

public:
	// The OnHitFlashEffectParams to be used with OnHitFlashEffect's blink
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="OnHitMaterialInterface")
	FOnHitFlashEffectParams OnHitFlashEffectParams;

private:
	// do blink of IOnHitFlashEffect
	UFUNCTION()
	void OnHealthChanged(UArcherHealthComponent* sentHealthComponent, float oldValue, float newValue, AActor* sentInstigator);

#pragma endregion

protected:
	virtual void BeginPlay() override;
};
