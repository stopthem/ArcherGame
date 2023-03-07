// Fill out your copyright notice in the Description page of Project Settings.


#include "PoolableComponent.h"

void UPoolableComponent::Taken()
{
	IsTaken = true;
}

void UPoolableComponent::ReturnToPool()
{
	IsTaken = false;
}

void UPoolableComponent::Init(APooler* pooler)
{
	Pooler = pooler;
}
