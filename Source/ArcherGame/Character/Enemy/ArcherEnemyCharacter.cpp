// Fill out your copyright notice in the Description page of Project Settings.


#include "ArcherEnemyCharacter.h"

#include "ArcherGame/Character/Ability/Attribute/ArcherHealthComponent.h"

AArcherEnemyCharacter::AArcherEnemyCharacter()
{
}

void AArcherEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();

	// set interface variable and bind to health component's OnHealthChanged
	OnHitEffectParams = FOnHitFlashEffectParams(OnHitFlashEffectParams);
	HealthComponent->OnHealthChanged.AddDynamic(this, &ThisClass::OnHealthChanged);
}

void AArcherEnemyCharacter::OnHealthChanged(UArcherHealthComponent* sentHealthComponent, float oldValue, float newValue, AActor* sentInstigator)
{
	// maybe its better to call once but i wanted this interface to be simple and maybe in the future this mesh will change? :D
	OnHitEffectParams.Mesh = GetMesh();

	DoHitFlashBlink();
}
