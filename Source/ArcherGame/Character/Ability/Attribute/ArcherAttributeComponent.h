// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/GameFrameworkComponent.h"
#include "ArcherAttributeComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FArcherAttribue_AttributeChanged, UArcherAttributeComponent*, AttributeComponent, float, OldValue, float, NewValue, AActor*, Instigator);

/**
 * 
 */
UCLASS(Abstract)
class ARCHERGAME_API UArcherAttributeComponent : public UGameFrameworkComponent
{
	GENERATED_BODY()

public:
	UArcherAttributeComponent(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(BlueprintAssignable)
	FArcherAttribue_AttributeChanged OnAttributeChanged;
};
