// Fill out your copyright notice in the Description page of Project Settings.


#include "InputConfig.h"

// Returns the first Input Action associated with a given tag.
const UInputAction* UInputConfig::FindInputActionForTag(const FGameplayTag& inputTag) const
{
	for (const FTaggedInputAction& TaggedInputAction : TaggedInputActions)
	{
		if (TaggedInputAction.InputAction && TaggedInputAction.InputTag == inputTag)
		{
			return TaggedInputAction.InputAction;
		}
	}

	return nullptr;
}
