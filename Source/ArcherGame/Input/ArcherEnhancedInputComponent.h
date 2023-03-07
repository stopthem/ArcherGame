// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnhancedInputComponent.h"
#include "InputConfig.h"
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
};

template <class UserClass, typename FuncType>
void UArcherEnhancedInputComponent::BindActionByTag(const UInputConfig* inputConfig, const FGameplayTag& inputTag, ETriggerEvent triggerEvent, UserClass* object, FuncType func)
{
	check(inputConfig);

	if (const UInputAction* inputAction = inputConfig->FindInputActionForTag(inputTag))
	{
		BindAction(inputAction, triggerEvent, object, func);
	}
}
