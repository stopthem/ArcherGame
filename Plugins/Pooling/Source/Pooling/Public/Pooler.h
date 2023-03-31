#pragma once

#include "CoreMinimal.h"
#include "GameplayTagAssetInterface.h"
#include "GameplayTagContainer.h"
#include "GameFramework/Actor.h"
#include "Pooler.generated.h"

class UPoolableComponent;
/**
 * APooler
 *
 * The class that pools objects
 */
UCLASS()
class POOLING_API APooler : public AActor, public IGameplayTagAssetInterface
{
	GENERATED_BODY()

public:
	APooler();

	UPROPERTY(EditAnywhere, Category="Pooling")
	bool bPoolingParticle = false;

	// The actor to pool
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Pooling")
	TSubclassOf<AActor> ActorToPool = nullptr;

	// How many ActorToPool to spawn in PreInitializeComponents
	UPROPERTY(EditAnywhere, Category="Pooling")
	int SpawnAtStart;

	// Get pooled object from the pool
	UFUNCTION(BlueprintCallable)
	AActor* GetPooledObj();

	UPROPERTY(BlueprintReadOnly, Category="Pooling")
	TArray<UPoolableComponent*> PooledActors;

	// Get pooled object from the pool and find given component
	template <typename T>
	UFUNCTION(BlueprintCallable)
	typename TEnableIf<TIsDerivedFrom<T, UActorComponent>::IsDerived, T*>::Type GetPooledObj()
	{
		return GetPooledObj()->FindComponentByClass<T>();
	}

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
	virtual void BeginPlay() override;

	// Make given actor reset to a newly spawned state
	void ResetPooledObj(AActor* pooledActor);

private:
	AActor* SpawnPooledActor();
};
