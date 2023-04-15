// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "WaveSpawner.generated.h"

class UArcherHealthComponent;
class UNavigationSystemV1;
class AArcherEnemyCharacter;

/*
 * FWaveSpawnInfo
 *
 * Holds information about what to spawn and how many
 */
USTRUCT()
struct FWaveSpawnInfo
{
	GENERATED_BODY()

public:
	// The BP Class we will spawn.
	UPROPERTY(EditAnywhere, Category="Wave|Spawn")
	TSubclassOf<AArcherEnemyCharacter> ToBeSpawnedBP = nullptr;

	UPROPERTY(EditAnywhere, Category="Wave|Spawn", meta=(UIMin="0", ClampMin="0"))
	int HowManyToSpawn = 1;
};

/*
 * FWaveInfo
 *
 * Holds a WaveSpawnInfo array
 */
USTRUCT()
struct FWaveInfo
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category="Wave", meta=(TitleProperty = "ToBeSpawnedBP"))
	TArray<FWaveSpawnInfo> WaveSpawnInfos;

	TArray<AArcherEnemyCharacter*> SpawnedEnemies;

	bool GetAliveEnemies(TArray<AArcherEnemyCharacter*>& out_aliveEnemies);
};

/*
 * AWaveSpawner
 *
 * The wave spawner actor.
 */
UCLASS()
class ARCHERGAME_API AWaveSpawner : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AWaveSpawner();

	// How much time we wait before spawning a new wave ?
	UPROPERTY(EditAnywhere, Category="Wave", meta=(UIMin="0", ClampMin="0"))
	float TimeBetweenWaves = 3.0f;

	// The range of distance where we can spawn enemies
	UPROPERTY(EditAnywhere, Category="Wave")
	FFloatRange SpawnDistanceRange;

	// The radius of the find random point on navigation
	UPROPERTY(EditAnywhere, Category="Wave", meta=(UIMin="0", ClampMin="0"))
	float SpawnRadius = 100.0f;

public:
	// Wave infos that we will spawn and iterate through
	UPROPERTY(EditAnywhere, Category="Wave")
	TArray<FWaveInfo> WaveInfos;

private:
	// We will use this to get the actual wave from array
	int CurrentWaveNumber = -1;

	FWaveInfo CurrentWave;

	void SpawnWave();

	// Bounded to enemy's HealthComponent.OnDeathStarted and checks if the wave is complete.
	UFUNCTION()
	void OnWaveEnemyDied(AActor* owningActor);

protected:
	// Get navigation system and spawn the first wave
	virtual void BeginPlay() override;

private:
	UPROPERTY()
	UNavigationSystemV1* NavigationSystemV1;
};
