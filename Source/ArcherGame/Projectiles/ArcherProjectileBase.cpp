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
#include "Kismet/KismetMathLibrary.h"
#include "Particles/ParticleSystemComponent.h"

UParticleSystemComponent* FProjectileHitParticleInfo::PlayHitParticle(const AActor* hitter, const AActor* hitActor, FParticlePlayingOptions particlePlayingOptions, EAttachLocation::Type attachLocation) const
{
	// If we got a block collision response that means we hit a world object.
	const bool bHitIsWorld = hitActor->GetComponentsCollisionResponseToChannel(Cast<AArcherProjectileBase>(hitter)->GetCollisionComponent()->GetCollisionObjectType()) == ECR_Block;

	// If tag is valid and we didn't hit world object, use player vfx pooler tag.
	const FGameplayTag particlePoolerTag = !bHitIsWorld && PlayerHitVfxPoolerTag.IsValid() ? PlayerHitVfxPoolerTag : HitVfxPoolerTag;

	// If player hit vfx is not null and we didn't hit world object, use player vfx.
	UParticleSystem* particleSystemTemplate = !bHitIsWorld && PlayerHitVfx ? PlayerHitVfx : HitVfx;

	// We should attach if attach options is AttachBoth or attach player and we didn't hit world or attach world and we hit world. 
	const bool bShouldAttach = ParticleCanAttachOptions == AttachBoth || ((ParticleCanAttachOptions == AttachPlayer && !bHitIsWorld) || (ParticleCanAttachOptions == AttachWorld && bHitIsWorld));

	// If we can attach, try to set optional variables in particle playing options like scene component, socket name and rotation towards bone.
	if (bShouldAttach)
	{
		if (USkinnedMeshComponent* foundSkinnedMeshComponent = hitActor->FindComponentByClass<USkinnedMeshComponent>())
		{
			FVector* socketLocation = nullptr;
			// If we found a skinned mesh component, find closest bone.
			particlePlayingOptions.SocketName = foundSkinnedMeshComponent->FindClosestBone(hitter->GetActorLocation(), socketLocation);

			if (socketLocation)
			{
				particlePlayingOptions.SceneComponent = foundSkinnedMeshComponent;

				if (bRotateTowardsFoundBone)
				{
					particlePlayingOptions.PlayRotation = UKismetMathLibrary::FindLookAtRotation(particlePlayingOptions.PlayLocation, *socketLocation);
				}
			}
		}
		else
		{
			particlePlayingOptions.SceneComponent = hitActor->GetRootComponent();
		}

		particlePlayingOptions.ParticleAttachmentRules = EParticleAttachmentRules::AttachGivenValuesAreWorld;
	}

	UParticleSystemComponent* spawnedParticleComponent;


	if (bPlayPooledParticle)
	{
		if (!bShouldAttach)
		{
			particlePlayingOptions.SceneComponent = nullptr;
			particlePlayingOptions.SocketName = NAME_None;
		}
		spawnedParticleComponent = UParticleBlueprintFunctionLibrary::PlayPooledParticle(particlePlayingOptions.PlayActor, particlePoolerTag, particlePlayingOptions);
	}
	else
	{
		if (bShouldAttach)
		{
			spawnedParticleComponent = UGameplayStatics::SpawnEmitterAttached(particleSystemTemplate, particlePlayingOptions.SceneComponent, particlePlayingOptions.SocketName, particlePlayingOptions.PlayLocation,
			                                                                  particlePlayingOptions.PlayRotation,
			                                                                  attachLocation);
		}
		else
		{
			spawnedParticleComponent = UGameplayStatics::SpawnEmitterAtLocation(hitter, particleSystemTemplate, particlePlayingOptions.PlayLocation, particlePlayingOptions.PlayRotation);
		}
	}

	return spawnedParticleComponent;
}

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


	if (bDestroyAfterTime)
	{
		// Start tween for scaling to zero and calling HandleActorEnding
		SetTimerForDestroy(TimeBeforeHandlingEnd);
	}

	if (!ProjectileCollisionComponent->OnComponentBeginOverlap.IsAlreadyBound(this, &ThisClass::OnBeginOverlap))
	{
		ProjectileCollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnBeginOverlap);
	}
}

void AArcherProjectileBase::SetTimerForDestroy(float time)
{
	if (EndScaleTween)
	{
		EndScaleTween->Destroy();
	}

	EndScaleTween = FCTween::Play(GetActorScale3D(), FVector(0), [&](const FVector& vector)
	                {
		                SetActorScale3D(vector);
	                }, DestroyEmitterScaleTweenParams.Duration, DestroyEmitterScaleTweenParams.Ease)
	                ->SetDelay(time - DestroyEmitterScaleTweenParams.Duration)
	                ->SetOnComplete([&]
	                {
		                HandleActorEnding();
	                });
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
	FParticlePlayingOptions particlePlayingOptions(otherActor);
	particlePlayingOptions.PlayLocation = GetActorLocation();
	particlePlayingOptions.PlayRotation = GetProjectileHitRotation();

	ProjectileHitParticleInfo.PlayHitParticle(this, otherActor, particlePlayingOptions);
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

		// If our instigator has ability system component, apply spec to target
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
	// Destroy SpawnedTween so FCTween doesn't try to update tween when this object is not used.
	if (EndScaleTween != nullptr)
	{
		EndScaleTween->Destroy();
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
