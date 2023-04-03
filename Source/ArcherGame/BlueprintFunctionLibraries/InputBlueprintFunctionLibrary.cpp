// Fill out your copyright notice in the Description page of Project Settings.


#include "InputBlueprintFunctionLibrary.h"

#include "InputMappingContext.h"

UInputBlueprintFunctionLibrary::UInputBlueprintFunctionLibrary()
{
}

FEnhancedActionKeyMapping UInputBlueprintFunctionLibrary::GetEnhancedKeyMappingFromAction(const UInputMappingContext* mappingContext, const UInputAction* inputAction)
{
	check(inputAction);

	if (const auto foundActionKeyMapping = mappingContext->GetMappings().FindByPredicate([&](const FEnhancedActionKeyMapping keyMapping)
	{
		return keyMapping.Action == inputAction;
	}))
	{
		return *foundActionKeyMapping;
	}

	UE_LOG(LogTemp, Warning, TEXT("InputBlueprintFunctionLibrary -> no EnhancedActionKeyMapping was found with mapping %s action %s"), *mappingContext->GetName(), *inputAction->GetName());
	return FEnhancedActionKeyMapping();
}
