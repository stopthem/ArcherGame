// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PoolableComponent.generated.h"

class APooler;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class POOLING_API UPoolableComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	void ReturnToPool();

	void Taken();

	bool GetIsTaken() const
	{
		return IsTaken;
	}

	void Init(APooler* pooler);

	APooler* GetPooler() const
	{
		return Pooler;
	}

private:
	bool IsTaken;

	TObjectPtr<APooler> Pooler;
};
