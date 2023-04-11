// Fill out your copyright notice in the Description page of Project Settings.


#include "ArcherInputKeyNamesDataAsset.h"

UArcherInputKeyNamesDataAsset::UArcherInputKeyNamesDataAsset()
{
}

FText UArcherInputKeyNamesDataAsset::GetKeyNameFromKey(const FKey& Key)
{
	if (const auto foundKeyName = CustomKeyNames.FindByPredicate([&](const FCustomKeyName& customKeyName)
	{
		return customKeyName.Key == Key;
	}))
	{
		return foundKeyName->KeyName;
	}

	UE_LOG(LogTemp, Warning, TEXT("ArcherInputKeyNamesDataAsset -> couldn't find custom key name with key %s"), *Key.GetDisplayName().ToString());
	return FText();
}
