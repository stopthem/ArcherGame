// Fill out your copyright notice in the Description page of Project Settings.


#include "PoolableComponent.h"

#include "Pooler.h"

void UPoolableComponent::Taken()
{
	IsTaken = true;
}

void UPoolableComponent::ReturnToPool()
{
	if (!this)
	{
		UE_LOG(LogTemp, Warning, TEXT("a poolable component is returned null when tried to return to pool! probably reference you are holding is null"));
		return;
	}
	Pooler->ReturnToPool(this);
	IsTaken = false;
}

void UPoolableComponent::Init(APooler* pooler)
{
	Pooler = pooler;
}
