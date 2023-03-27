// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PoolableComponent.generated.h"

class APooler;

/**
 * UPoolableComponent
 *
 * The component that every pooled actor has, used mainly for storing information
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class POOLING_API UPoolableComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void ReturnToPool();

	virtual void Taken();

	bool GetIsTaken() const
	{
		return IsTaken;
	}

	void Init(APooler* pooler);

	APooler* GetPooler() const
	{
		return Pooler;
	}

	UPROPERTY(VisibleAnywhere)
	bool IsTaken;

private:
	UPROPERTY()
	APooler* Pooler;
};
