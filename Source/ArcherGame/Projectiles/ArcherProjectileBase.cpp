// Fill out your copyright notice in the Description page of Project Settings.


#include "ArcherProjectileBase.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "FCTween.h"
#include "PoolableComponent.h"
#include "ArcherGame/ArcherGameplayTags.h"
#include "ArcherGame/BlueprintFunctionLibraries/ParticleBlueprintFunctionLibrary.h"
#include "ArcherGame/Character/Ability/ArcherAbilitySystemComponent.h"
#include "ArcherGame/Character/Ability/ArcherGameplayEffect.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AArcherProjectileBase::AArcherProjectileBase()
{
	// create projectile movement
	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile Movement Component"));
}

void AArcherProjectileBase::Shoot(AActor* projectileInstigator)
{
	ProjectileInstigator = projectileInstigator;

	ProjectileCollisionComponent->SetGenerateOverlapEvents(true);

	// When we use initial speed ProjectileMovementComponent->Velocity becomes direction for unreal and we override it again with what unreal does (velocity * initial speed).
	ProjectileMovementComponent->Velocity = GetActorForwardVector() * ProjectileMovementComponent->InitialSpeed;
	ProjectileMovementComponent->Activate();

	// Start tween for scaling to zero and calling HandleActorEnding
	ProjectileHitParticleInfo.SpawnedTween = FCTween::Play(GetActorScale3D(), FVector(0), [&](const FVector& vector)
	                                         {
		                                         SetActorScale3D(vector);
	                                         }, ProjectileHitParticleInfo.DestroyEmitterScaleTweenParams.Duration, ProjectileHitParticleInfo.DestroyEmitterScaleTweenParams.Ease)
	                                         ->SetDelay(ProjectileHitParticleInfo.DestroyEmitterScaleTweenParams.OneValue)
	                                         ->SetOnComplete([&]
	                                         {
		                                         HandleActorEnding();
	                                         });

	if (!ProjectileCollisionComponent->OnComponentBeginOverlap.IsAlreadyBound(this, &ThisClass::OnBeginOverlap))
	{
		ProjectileCollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnBeginOverlap);
	}
}

void AArcherProjectileBase::BeginPlay()
{
	Super::BeginPlay();

	ProjectileCollisionComponent = FindComponentByClass<UPrimitiveComponent>();
}

void AArcherProjectileBase::OnBeginOverlap(UPrimitiveComponent* overlappedComponent, AActor* otherActor, UPrimitiveComponent* otherComponent, int32 otherBodyIndex, bool bFromSweep, const FHitResult& hit)
{
	// Local function that handles before actor ending
	auto HandleBeforeActorEnding = [&]
	{
		ProjectileCollisionComponent->SetGenerateOverlapEvents(false);

		ProjectileMovementComponent->StopMovementImmediately();

		HandleActorEnding();
	};


	bool bDamaged = false;
	// Can we damage anything and if we can did we ?
	if (DamagingCollisionCount < HowManyDamagingCollisions && DamageOverlappedActor(otherActor))
	{
		bDamaged = true;
		DamagingCollisionCount++;
	}

	// Did overlapped actor blocked us?
	const bool bBlocked = otherComponent->GetCollisionResponseToChannel(ProjectileCollisionComponent->GetCollisionObjectType()) == ECR_Block;

	// did we get blocked or we only have one damaging collisions and we don't want to move after collision
	if (bBlocked || (HowManyDamagingCollisions == 1 && !bDoesMoveUntilBlockedAfterDamagingCollisionsFull))
	{
		HandleBeforeActorEnding();
	}
	else if (DamagingCollisionCount == HowManyDamagingCollisions && !bDoesMoveUntilBlockedAfterDamagingCollisionsFull) // if we filled our DamagingCollisionAmount and we don't want to move until blocked
	{
		HandleBeforeActorEnding();
	}

	// Only play hit particle when we damaged something or we hit our concrete object
	if (bBlocked || bDamaged)
	{
		PlayHitParticle(otherActor);
	}
}

void AArcherProjectileBase::PlayHitParticle(AActor* otherActor)
{
	if (ProjectileHitParticleInfo.bPlayPooledParticle)
	{
		FParticlePlayingOptions particlePlayingOptions(otherActor);
		particlePlayingOptions.PlayLocation = GetActorLocation();
		particlePlayingOptions.PlayRotation = GetProjectileHitRotation();
		UParticleBlueprintFunctionLibrary::PlayPooledParticle(particlePlayingOptions.PlayActor, ProjectileHitParticleInfo.HitVfxPoolerTag, particlePlayingOptions);
	}
	else
	{
		UGameplayStatics::SpawnEmitterAtLocation(this, ProjectileHitParticleInfo.HitVfx, GetActorLocation(),
		                                         GetProjectileHitRotation());
	}
}

bool AArcherProjectileBase::DamageOverlappedActor(AActor* otherActor)
{
	if (UArcherAbilitySystemComponent* otherASC = otherActor->FindComponentByClass<UArcherAbilitySystemComponent>())
	{
		FGameplayEffectContextHandle gameplayEffectContext = otherASC->MakeEffectContext();
		gameplayEffectContext.AddInstigator(ProjectileInstigator, this);

		const FGameplayEffectSpecHandle specHandle = otherASC->MakeOutgoingSpec(*DamageGameplayEffect, 1, gameplayEffectContext);

		// the actual damage is magnitude and we are getting it from TAG_Data_Damage which is set by damage causer
		UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(specHandle, TAG_Data_Damage, GetDamageAmount());

		if (UArcherAbilitySystemComponent* causerASC = ProjectileInstigator->FindComponentByClass<UArcherAbilitySystemComponent>())
		{
			causerASC->ApplyGameplayEffectSpecToTarget(*specHandle.Data.Get(), otherASC);
		}
		else
		{
			otherASC->ApplyGameplayEffectSpecToSelf(*specHandle.Data.Get());
		}

		return true;
	}

	return false;
}

void AArcherProjectileBase::HandleActorEnding()
{
	if (ProjectileHitParticleInfo.SpawnedTween != nullptr)
	{
		ProjectileHitParticleInfo.SpawnedTween->Destroy();
	}

	// wait for next tick because there are probably some calculations by other classes like gameplay effects
	GetWorldTimerManager().SetTimerForNextTick(FTimerDelegate::CreateLambda([&]
	{
		if (bShouldReturnToPoolAfterOverlap)
		{
			ReturnToPool();
		}
		else
		{
			Destroy();
		}
	}));
}

void AArcherProjectileBase::ReturnToPool()
{
	// poolable component is added in BeginPlay so this variable can be null
	if (!PoolableComponent)
	{
		PoolableComponent = FindComponentByClass<UPoolableComponent>();
	}

	DamagingCollisionCount = 0;

	ProjectileMovementComponent->Deactivate();
	PoolableComponent->ReturnToPool();
}
