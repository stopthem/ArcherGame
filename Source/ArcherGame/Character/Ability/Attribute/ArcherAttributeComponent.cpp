// Fill out your copyright notice in the Description page of Project Settings.


#include "ArcherAttributeComponent.h"

UArcherAttributeComponent::UArcherAttributeComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PrimaryComponentTick.bStartWithTickEnabled = false;
	PrimaryComponentTick.bCanEverTick = false;
}
