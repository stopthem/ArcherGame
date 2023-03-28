// Fill out your copyright notice in the Description page of Project Settings.


#include "ArcherProjectileBase.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "PoolableComponent.h"
#include "ArcherGame/ArcherGameplayTags.h"
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
}

void AArcherProjectileBase::PlayHitParticle(AActor* otherActor)
{
	FParticlePlayingOptions particlePlayingOptions(this);
	particlePlayingOptions.PlayRotation = ProjectileHitParticleInfo.bUseActorRotation ? GetActorRotation() : FRotator::ZeroRotator;
	ProjectileHitParticleInfo.PlayParticle(particlePlayingOptions);
}

void AArcherProjectileBase::DamageOverlappedActor(AActor* otherActor)
{
	// create gameplayEvent data and only fill basic variables
	FGameplayEventData gameplayEventData;
	gameplayEventData.Instigator = this;
	gameplayEventData.Target = otherActor;

	if (UArcherAbilitySystemComponent* otherASC = otherActor->FindComponentByClass<UArcherAbilitySystemComponent>())
	{
		const FGameplayEffectContextHandle gameplayEffectContext = otherASC->MakeEffectContext();

		const FGameplayEffectSpecHandle specHandle = otherASC->MakeOutgoingSpec(*DamageGameplayEffect, 0, gameplayEffectContext);
		// the actual damage is magnitude
		UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(specHandle, TAG_Data_Damage, GetDamageAmount());

		otherASC->ApplyGameplayEffectSpecToSelf(*specHandle.Data.Get());

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
	if (!PoolableComponent)
	{
		PoolableComponent = FindComponentByClass<UPoolableComponent>();
	}

	// UE_LOG(LogTemp, Warning, TEXT("returned to pool %s"), *GetActorNameOrLabel());
	ProjectileMovementComponent->Deactivate();
	PoolableComponent->ReturnToPool();
}
