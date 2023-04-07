// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectTypes.h"

#include "ArcherGameplayEffectContext.generated.h"
/*
 * FArcherGameplayEffectContext
 *
 * Base effect context class that abilities use 
 */
USTRUCT()
struct FArcherGameplayEffectContext : public FGameplayEffectContext
{
	GENERATED_BODY()

	FArcherGameplayEffectContext()
	{
	}

	// Returns the wrapped FArcherGameplayEffectContext from the handle, or nullptr if it doesn't exist or is the wrong type //
	static ARCHERGAME_API FArcherGameplayEffectContext* ExtractEffectContext(FGameplayEffectContextHandle handle);

	virtual UScriptStruct* GetScriptStruct() const override
	{
		return StaticStruct();
	}

	virtual FGameplayEffectContext* Duplicate() const override
	{
		FArcherGameplayEffectContext* NewContext = new FArcherGameplayEffectContext();
		*NewContext = *this;
		if (GetHitResult())
		{
			// Does a deep copy of the hit result
			NewContext->AddHitResult(*GetHitResult(), true);
		}
		return NewContext;
	}
};

template <>
struct TStructOpsTypeTraits<FArcherGameplayEffectContext> : TStructOpsTypeTraitsBase2<FArcherGameplayEffectContext>
{
	enum
	{
		WithNetSerializer = true,
		WithCopy = true
	};
};
