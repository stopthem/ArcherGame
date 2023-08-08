// Fill out your copyright notice in the Description page of Project Settings.


#include "ArcherEnemyCharacter.h"

#include "FCTween.h"
#include "ArcherGame/Character/Ability/Attribute/ArcherHealthComponent.h"
#include "ArcherGame/Character/Ability/Attribute/ArcherHealthSet.h"
#include "ArcherGame/Character/Effects/HitFlashEffectComponent.h"
#include "Kismet/GameplayStatics.h"

AArcherEnemyCharacter::AArcherEnemyCharacter()
{
	HitFlashEffectComponent = CreateDefaultSubobject<UHitFlashEffectComponent>("HitFlashEffectComponent");
}

void AArcherEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();

	// set interface variable and bind to health component's OnHealthChanged
	// OnHitEffectParams = FOnHitFlashEffectParams(OnHitFlashEffectParams);
	ArcherHealthComponent->OnHealthChanged.AddDynamic(this, &ThisClass::OnHealthChanged);
}

void AArcherEnemyCharacter::Spawn()
{
	// Adding immunity so player does not damage us when we are spawning.
	GetArcherAbilitySystemComponent()->AddLooseGameplayTag(TAG_Gameplay_DamageImmunity);

	// Spawn the spawning vfx
	UGameplayStatics::SpawnEmitterAtLocation(this, SpawnVFX, GetActorLocation(), FRotator(0));

	const USkeletalMeshComponent* skeletalMesh = GetMesh();

	// Alter on hit effect params(interface one) duration to match our scale tween duration so it matches exactly.
	FOnHitFlashEffectParams& HitFlashEffectParams = HitFlashEffectComponent->OnHitEffectParams;

	const float prevHitEffectDuration = HitFlashEffectParams.Duration;
	HitFlashEffectParams.Duration = SpawnScaleTweenParams.Duration;

	HitFlashEffectComponent->DoHitFlashBlink();

	// Play the scaling tween.
	FCTween::Play(FVector(0), skeletalMesh->GetComponentScale(), [&](const FVector& scaleVector)
		{
			// It crashes when i use skeletalMesh local variable, don't know why.
			GetMesh()->SetWorldScale3D(scaleVector);
		}, SpawnScaleTweenParams.Duration, SpawnScaleTweenParams.Ease)
		->SetOnComplete([=]() mutable
		{
			// Remove damage immunity.
			GetArcherAbilitySystemComponent()->RemoveLooseGameplayTag(TAG_Gameplay_DamageImmunity);

			// Restore on hit flash effect params duration
			HitFlashEffectParams.Duration = prevHitEffectDuration;
		});
}

void AArcherEnemyCharacter::OnHealthChanged(UArcherHealthComponent* sentHealthComponent, float oldValue, float newValue, AActor* sentInstigator)
{
	// don't apply hit flash effect if we are dying
	if (oldValue == 0)
	{
		return;
	}

	HitFlashEffectComponent->DoHitFlashBlink();
}
