// Fill out your copyright notice in the Description page of Project Settings.


#include "WaveSpawner.h"

#include "NavigationSystem.h"
#include "ArcherGame/Character/Ability/Attribute/ArcherHealthComponent.h"
#include "ArcherGame/Character/Enemy/ArcherEnemyCharacter.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetStringLibrary.h"

bool FWaveInfo::GetAliveEnemies(TArray<AArcherEnemyCharacter*>& out_aliveEnemies)
{
	for (AArcherEnemyCharacter* enemyCharacter : SpawnedEnemies)
	{
		if (IsValid(enemyCharacter) && !UArcherHealthComponent::FindHealthComponent(enemyCharacter)->IsDead())
		{
			out_aliveEnemies.Add(enemyCharacter);
		}
	}

	return out_aliveEnemies.Num() > 0;
}

// Sets default values
AWaveSpawner::AWaveSpawner()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	// PrimaryActorTick.bCanEverTick = true;
}

void AWaveSpawner::BeginPlay()
{
	Super::BeginPlay();

	NavigationSystemV1 = UNavigationSystemV1::GetCurrent(this);

	SpawnWave();
}

void AWaveSpawner::SpawnWave()
{
	check(NavigationSystemV1);

	CurrentWaveNumber++;

	if (!WaveInfos.IsValidIndex(CurrentWaveNumber))
	{
		// if there is not a next wave, waves are complete
		UE_LOG(LogTemp, Warning, TEXT("waves complete!"));
		return;
	}

	CurrentWave = WaveInfos[CurrentWaveNumber];

	const FVector playerLocation = UGameplayStatics::GetPlayerCharacter(this, 0)->GetActorLocation();

	for (const FWaveSpawnInfo& waveSpawnInfo : CurrentWave.WaveSpawnInfos)
	{
		// Get a random unit vector which we will be use it like a direction.
		const FVector selectedRandomDirection = UKismetMathLibrary::RandomUnitVector();

		// We don't want to spawn near or too far from the player. We add the random direction multiplied with where in distance we can spawn which is a random inside SpawnDistanceRange.
		FVector selectedRandomPoint = playerLocation + (selectedRandomDirection * FMath::RandRange(SpawnDistanceRange.GetLowerBoundValue(), SpawnDistanceRange.GetUpperBoundValue()));

		// And then we add our radius to the point because if we don't do this, we can spawn in a distance less than SpawnDistanceRange's lower bound.
		selectedRandomPoint += selectedRandomDirection * SpawnRadius;

		FNavLocation foundNavLocation;
		if (!NavigationSystemV1->GetRandomReachablePointInRadius(selectedRandomPoint, SpawnRadius, foundNavLocation))
		{
			// If we couldn't find a random reachable point in radius, find closest nav mesh point by projecting.
			NavigationSystemV1->ProjectPointToNavigation(selectedRandomPoint, foundNavLocation);
		}

		// Find the look rotation towards player
		FRotator rotationToPlayer = UKismetMathLibrary::FindLookAtRotation(foundNavLocation.Location, playerLocation);
		// We only want yaw.
		rotationToPlayer.Roll = rotationToPlayer.Pitch = 0;

		// Its a wave so it needs to always spawn.
		FActorSpawnParameters actorSpawnParameters;
		actorSpawnParameters.Owner = this;
		actorSpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

		// Spawn the enemy
		AArcherEnemyCharacter* spawnedEnemyCharacter = GetWorld()->SpawnActor<AArcherEnemyCharacter>(waveSpawnInfo.ToBeSpawnedBP, foundNavLocation.Location, rotationToPlayer, actorSpawnParameters);

		// Bind to enemy's HealthComponent.OnDeathStarted so we can check wave's status when they die.
		UArcherHealthComponent::FindHealthComponent(spawnedEnemyCharacter)->OnDeathStarted.AddDynamic(this, &ThisClass::OnWaveEnemyDied);

		// Add spawned enemy to current wave's enemies so it has the info for its functions.
		CurrentWave.SpawnedEnemies.Add(spawnedEnemyCharacter);
	}
}

void AWaveSpawner::OnWaveEnemyDied(AActor* owningActor)
{
	TArray<AArcherEnemyCharacter*> aliveEnemies;
	if (!CurrentWave.GetAliveEnemies(aliveEnemies))
	{
		// If there is no alive enemies, wave completed.
		UE_LOG(LogTemp, Warning, TEXT("wave complete!"));
	}
}
