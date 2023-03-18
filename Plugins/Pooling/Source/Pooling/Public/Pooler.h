#pragma once

#include "CoreMinimal.h"
#include "GameplayTagAssetInterface.h"
#include "GameplayTagContainer.h"
#include "GameFramework/Actor.h"
#include "Pooler.generated.h"

class UPoolableComponent;
UCLASS()
class POOLING_API APooler : public AActor, public IGameplayTagAssetInterface
{
	GENERATED_BODY()

public:
	// The actor to pool
	UPROPERTY(EditAnywhere, Category="Pooling")
	TSubclassOf<AActor> ActorToPool = nullptr;

	// How many ActorToPool to spawn in BeginPlay
	UPROPERTY(EditAnywhere, Category="Pooling")
	int SpawnAtStart;

	// Get pooled object from the pool
	UFUNCTION(BlueprintCallable)
	AActor* GetPooledObj();

	void ReturnToPool(const UPoolableComponent* poolableComponent);

	// The gameplay tag for finding this pooler
	UPROPERTY(EditAnywhere, Category="Pooling")
	FGameplayTag GameplayTag;

#pragma region IGameplayTagAssetInterface
	virtual void GetOwnedGameplayTags(FGameplayTagContainer& TagContainer) const override
	{
		TagContainer.AddTag(GameplayTag);
	}

	virtual bool HasMatchingGameplayTag(FGameplayTag TagToCheck) const override
	{
		return TagToCheck == GameplayTag;
	}

	virtual bool HasAllMatchingGameplayTags(const FGameplayTagContainer& TagContainer) const override
	{
		return TagContainer.HasAll(FGameplayTagContainer(GameplayTag));
	}

	virtual bool HasAnyMatchingGameplayTags(const FGameplayTagContainer& TagContainer) const override
	{
		return TagContainer.HasTag(GameplayTag);
	}
#pragma endregion

protected:
	// Spawn starting pooled objects
	virtual void PreInitializeComponents() override;

	TArray<UPoolableComponent*> PooledActors;

	void ResetPooledObj(AActor* pooledActor);

private:
	AActor* SpawnPooledActor();
};
