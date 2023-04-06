// Fill out your copyright notice in the Description page of Project Settings.


#include "ArcherGameplayEffectContext.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(ArcherGameplayEffectContext)

FArcherGameplayEffectContext* FArcherGameplayEffectContext::ExtractEffectContext(FGameplayEffectContextHandle handle)
{
	FGameplayEffectContext* baseEffectContext = handle.Get();
	if (baseEffectContext != nullptr && baseEffectContext->GetScriptStruct()->IsChildOf(StaticStruct()))
	{
		// it only works with c style cast
		return (FArcherGameplayEffectContext*)baseEffectContext;
	}

	return nullptr;
}
