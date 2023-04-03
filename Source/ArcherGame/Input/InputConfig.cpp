// Fill out your copyright notice in the Description page of Project Settings.


#include "InputConfig.h"

// Returns the first Input Action associated with a given tag.
const UInputAction* UInputConfig::FindInputActionForTag(const FGameplayTag& inputTag) const
{
	// combine both native and ability input actions
	TArray<FTaggedInputAction> TaggedInputActionsAll;
	TaggedInputActionsAll.Append(NativeInputActions);
	TaggedInputActionsAll.Append(AbilityInputActions);

	if (const auto foundTaggedInputAction = TaggedInputActionsAll.FindByPredicate([&](const FTaggedInputAction& TaggedInputAction)
	{
		return TaggedInputAction.InputAction && TaggedInputAction.InputTag == inputTag;
	}))
	{
		return foundTaggedInputAction->InputAction;
	}

	UE_LOG(LogTemp, Warning, TEXT("InputConfig -> no input action found with tag %s"), *inputTag.GetTagName().ToString());
	return nullptr;
}
