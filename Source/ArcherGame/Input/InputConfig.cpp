// Fill out your copyright notice in the Description page of Project Settings.


#include "InputConfig.h"

// Returns the first Input Action associated with a given tag.
const UInputAction* UInputConfig::FindInputActionForTag(const FGameplayTag& inputTag) const
{
	const TArray<FTaggedInputAction>::ElementType* foundTaggedInputAction = NativeInputActions.FindByPredicate([&](const FTaggedInputAction& TaggedInputAction)
	{
		return TaggedInputAction.InputAction && TaggedInputAction.InputTag == inputTag;
	});

	return foundTaggedInputAction ? foundTaggedInputAction->InputAction : nullptr;
}
