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
	UPROPERTY(EditAnywhere, Category="Pooling")
	bool bPoolingParticle = false;

	// The actor to pool
	UPROPERTY(EditAnywhere, Category="Pooling")
	TSubclassOf<AActor> ActorToPool = nullptr;

	// How many ActorToPool to spawn in BeginPlay
	UPROPERTY(EditAnywhere, Category="Pooling")
	int SpawnAtStart;

	// Get pooled object from the pool
	UFUNCTION(BlueprintCallable)
	AActor* GetPooledObj();

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
	virtual void PreInitializeComponents() override;

	TArray<UPoolableComponent*> PooledActors;

	void ResetPooledObj(AActor* pooledActor);

private:
	AActor* SpawnPooledActor();
};
