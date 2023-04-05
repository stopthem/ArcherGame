// Fill out your copyright notice in the Description page of Project Settings.


#include "ArcherProjectileBase.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "PoolableComponent.h"
#include "ArcherGame/ArcherGameplayTags.h"
#include "ArcherGame/BlueprintFunctionLibraries/ParticleBlueprintFunctionLibrary.h"
#include "ArcherGame/Character/Ability/ArcherAbilitySystemComponent.h"
#include "ArcherGame/Character/Ability/ArcherGameplayEffect.h"
#include "Engine/DamageEvents.h"
#include "GameFramework/ProjectileMovementComponent.h"

// Sets default values
AArcherProjectileBase::AArcherProjectileBase()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	// PrimaryActorTick.bCanEverTick = true;

	// create projectile movement
	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile Movement Component"));
}

void AArcherProjectileBase::Shoot() const
{
	ProjectileMesh->SetGenerateOverlapEvents(true);

	// When we use initial speed ProjectileMovementComponent->Velocity becomes direction for unreal and we override it again with what unreal does (velocity * initial speed).
	ProjectileMovementComponent->Velocity = GetActorForwardVector() * ProjectileMovementComponent->InitialSpeed;
	ProjectileMovementComponent->Activate();
}

void AArcherProjectileBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	ProjectileMesh = FindComponentByClass<UStaticMeshComponent>();
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

	ProjectileMesh->SetGenerateOverlapEvents(false);

	ProjectileMovementComponent->StopMovementImmediately();

	PlayHitParticle(otherActor);

	DamageOverlappedActor(otherActor);

	if (bShouldReturnToPoolAfterOverlap)
	{
		ReturnToPool();
	}
	else
	{
		Destroy();
	}
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
		gameplayEffectContext.AddInstigator(this, this);

		const FGameplayEffectSpecHandle specHandle = otherASC->MakeOutgoingSpec(*DamageGameplayEffect, 0, gameplayEffectContext);

		// the actual damage is magnitude and we are getting it from TAG_Data_Damage which is set by damage causer
		UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(specHandle, TAG_Data_Damage, GetDamageAmount());
		otherASC->ApplyGameplayEffectSpecToSelf(*specHandle.Data.Get());

		FGameplayEventData gameplayEventData;
		gameplayEventData.Instigator = this;
		gameplayEventData.Target = otherActor;
		gameplayEventData.EventTag = TAG_GameplayEvent_Damaged;
		otherASC->GetOwnedGameplayTags(gameplayEventData.TargetTags);
		gameplayEventData.EventMagnitude = GetDamageAmount();
		gameplayEventData.ContextHandle = gameplayEffectContext;

		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(otherActor, TAG_GameplayEvent_Damaged, gameplayEventData);
	}


	if (otherActor->CanBeDamaged())
	{
		FDamageEvent damageEvent;
		otherActor->TakeDamage(GetDamageAmount(), damageEvent, GetInstigatorController(), this);
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
