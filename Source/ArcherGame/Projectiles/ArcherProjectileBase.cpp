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

void AArcherProjectileBase::Shoot(AActor* effectCauser)
{
	EffectCauser = effectCauser;

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
}

void AArcherProjectileBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	ProjectileCollisionComponent = FindComponentByClass<UPrimitiveComponent>();
}

// Called when the game starts or when spawned
void AArcherProjectileBase::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AArcherProjectileBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AArcherProjectileBase::NotifyActorBeginOverlap(AActor* otherActor)
{
	Super::NotifyActorBeginOverlap(otherActor);

	ProjectileCollisionComponent->SetGenerateOverlapEvents(false);

	ProjectileMovementComponent->StopMovementImmediately();

	PlayHitParticle(otherActor);

	DamageOverlappedActor(otherActor);

	HandleActorEnding();
}

void AArcherProjectileBase::PlayHitParticle(AActor* otherActor)
{
	if (ProjectileHitParticleInfo.bPlayPooledParticle)
	{
		FParticlePlayingOptions particlePlayingOptions(otherActor);
		particlePlayingOptions.PlayLocation = GetActorLocation();
		particlePlayingOptions.PlayRotation = ProjectileHitParticleInfo.bUseActorRotation ? GetActorRotation() : FRotator::ZeroRotator;
		UParticleBlueprintFunctionLibrary::PlayPooledParticle(particlePlayingOptions.PlayActor, ProjectileHitParticleInfo.HitVfxPoolerTag, particlePlayingOptions);
	}
	else
	{
		UGameplayStatics::SpawnEmitterAtLocation(this, ProjectileHitParticleInfo.HitVfx, GetActorLocation(),
		                                         ProjectileHitParticleInfo.bUseActorRotation ? GetActorRotation() : FRotator::ZeroRotator);
	}
}

void AArcherProjectileBase::DamageOverlappedActor(AActor* otherActor)
{
	if (UArcherAbilitySystemComponent* otherASC = otherActor->FindComponentByClass<UArcherAbilitySystemComponent>())
	{
		FGameplayEffectContextHandle gameplayEffectContext = otherASC->MakeEffectContext();
		gameplayEffectContext.AddInstigator(EffectCauser, this);

		const FGameplayEffectSpecHandle specHandle = otherASC->MakeOutgoingSpec(*DamageGameplayEffect, 0, gameplayEffectContext);

		// the actual damage is magnitude and we are getting it from TAG_Data_Damage which is set by damage causer
		UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(specHandle, TAG_Data_Damage, GetDamageAmount());

		auto OtherActorApplyGameEffectToSelf = [&]
		{
			otherASC->ApplyGameplayEffectSpecToSelf(*specHandle.Data.Get());
		};

		if (EffectCauser)
		{
			if (UArcherAbilitySystemComponent* causerASC = EffectCauser->FindComponentByClass<UArcherAbilitySystemComponent>())
			{
				causerASC->ApplyGameplayEffectSpecToTarget(*specHandle.Data.Get(), otherASC);
			}
			else
			{
				OtherActorApplyGameEffectToSelf();
			}
		}
		else
		{
			OtherActorApplyGameEffectToSelf();
		}
	}
}

void AArcherProjectileBase::HandleActorEnding()
{
	if (ProjectileHitParticleInfo.SpawnedTween != nullptr)
	{
		ProjectileHitParticleInfo.SpawnedTween->Destroy();
	}

	if (bShouldReturnToPoolAfterOverlap)
	{
		ReturnToPool();
	}
	else
	{
		Destroy();
	}
}

void AArcherProjectileBase::ReturnToPool()
{
	// poolable component is added in BeginPlay so this variable can be null
	if (!PoolableComponent)
	{
		PoolableComponent = FindComponentByClass<UPoolableComponent>();
	}

	// wait for next tick so damage calculations go correct
	GetWorldTimerManager().SetTimerForNextTick(FTimerDelegate::CreateLambda([&]
	{
		// UE_LOG(LogTemp, Warning, TEXT("returned to pool %s"), *GetActorNameOrLabel());
		ProjectileMovementComponent->Deactivate();
		PoolableComponent->ReturnToPool();
	}));
}
