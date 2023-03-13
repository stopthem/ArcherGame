// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnhancedInputComponent.h"
#include "InputConfig.h"
#include "NativeGameplayTags.h"
#include "ArcherEnhancedInputComponent.generated.h"

struct FGameplayTag;
class UInputConfig;
/**
 * 
 */
UCLASS()
class ARCHERGAME_API UArcherEnhancedInputComponent : public UEnhancedInputComponent
{
	GENERATED_BODY()

public:
	template <class UserClass, typename FuncType>
	void BindActionByTag(const UInputConfig* inputConfig, const FGameplayTag& inputTag, ETriggerEvent triggerEvent, UserClass* object, FuncType func);

	template <class UserClass, typename PressedFuncType, typename ReleasedFuncType>
	void BindAbilityActions(const UInputConfig* inputConfig, UserClass* object, PressedFuncType pressedFunc, ReleasedFuncType releasedFunc, TArray<uint32>& bindHandles);
};

template <class UserClass, typename FuncType>
void UArcherEnhancedInputComponent::BindActionByTag(const UInputConfig* inputConfig, const FGameplayTag& inputTag, ETriggerEvent triggerEvent, UserClass* object, FuncType func)
{
	// Make sure of input config
	check(inputConfig);

	// Bind action if given tag was given at inputConfig
	if (const UInputAction* inputAction = inputConfig->FindInputActionForTag(inputTag))
	{
		BindAction(inputAction, triggerEvent, object, func);
	}
}

template <class UserClass, typename PressedFuncType, typename ReleasedFuncType>
void UArcherEnhancedInputComponent::BindAbilityActions(const UInputConfig* inputConfig, UserClass* object, PressedFuncType pressedFunc, ReleasedFuncType releasedFunc, TArray<uint32>& bindHandles)
{
	check(inputConfig);

	for (const FTaggedInputAction& Action : inputConfig->AbilityInputActions)
	{
		if (Action.InputAction && Action.InputTag.IsValid())
		{
			if (pressedFunc)
			{
				bindHandles.Add(BindAction(Action.InputAction, ETriggerEvent::Triggered, object, pressedFunc, Action.InputTag).GetHandle());
			}

			if (releasedFunc)
			{
				bindHandles.Add(BindAction(Action.InputAction, ETriggerEvent::Completed, object, releasedFunc, Action.InputTag).GetHandle());
			}
		}
	}
}
