// Fill out your copyright notice in the Description page of Project Settings.


#include "ArcherEnemyCharacter.h"

#include "AIController.h"
#include "BrainComponent.h"
#include "FCTween.h"
#include "ArcherGame/Character/Ability/Attribute/ArcherHealthComponent.h"
#include "ArcherGame/Character/Ability/Attribute/ArcherHealthSet.h"
#include "Kismet/GameplayStatics.h"

AArcherEnemyCharacter::AArcherEnemyCharacter()
{
}

void AArcherEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();

	// set interface variable and bind to health component's OnHealthChanged
	OnHitEffectParams = FOnHitFlashEffectParams(OnHitFlashEffectParams);
	ArcherHealthComponent->OnHealthChanged.AddDynamic(this, &ThisClass::OnHealthChanged);
}

void AArcherEnemyCharacter::Spawn()
{
	// Adding immunity so player does not damage us when we are spawning.
	GetArcherAbilitySystemComponent()->AddLooseGameplayTag(TAG_Gameplay_DamageImmunity);

	// Spawn the spawning vfx
	UGameplayStatics::SpawnEmitterAtLocation(this, SpawnVFX, GetActorLocation(), FRotator(0));

	USkeletalMeshComponent* skeletalMesh = GetMesh();

	// Alter on hit effect params(interface one) duration to match our scale tween duration so it matches exactly.
	OnHitEffectParams.Duration = SpawnScaleTweenParams.Duration;

	OnHitEffectParams.Mesh = skeletalMesh;

	DoHitFlashBlink();

	// Play the scaling tween.
	FCTween::Play(FVector(0), skeletalMesh->GetComponentScale(), [&](const FVector& scaleVector)
		{
			// It crashes when i use skeletalMesh local variable, don't know why.
			GetMesh()->SetWorldScale3D(scaleVector);
		}, SpawnScaleTweenParams.Duration, SpawnScaleTweenParams.Ease)
		->SetOnComplete([&]
		{
			// Remove damage immunity.
			GetArcherAbilitySystemComponent()->RemoveLooseGameplayTag(TAG_Gameplay_DamageImmunity);

			// Restore on hit flash effect params duration
			OnHitEffectParams.Duration = OnHitFlashEffectParams.Duration;
		});
}

void AArcherEnemyCharacter::OnHealthChanged(UArcherHealthComponent* sentHealthComponent, float oldValue, float newValue, AActor* sentInstigator)
{
	// don't apply hit flash effect if we are dying
	if (oldValue == 0)
	{
		return;
	}

	// maybe its better to call once but i wanted this interface to be simple and maybe in the future this mesh will change? :D
	OnHitEffectParams.Mesh = GetMesh();

	DoHitFlashBlink();
}
