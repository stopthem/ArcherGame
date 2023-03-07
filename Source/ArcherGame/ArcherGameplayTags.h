// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"

/**
 * ArcherGameplayTags
 *
 * Singleton containing native gameplay tags.
 */
struct FArcherGameplayTags
{
public:
	static const FArcherGameplayTags& Get() { return ArcherGameplayTags; }

	static void InitializeNativeTags();

	// Input tags
	FGameplayTag InputTag_Move;
	FGameplayTag InputTag_Jump;
	FGameplayTag InputTag_Dash;
	FGameplayTag InputTag_LookNFire;

protected:
	//Registers all of the tags with the GameplayTags Manager
	void AddAllTags();

	//Helper function used by AddAllTags to register a single tag with the GameplayTags Manager
	void AddTag(FGameplayTag& outTag, const ANSICHAR* tagName, const ANSICHAR* tagComment);

private:
	static FArcherGameplayTags ArcherGameplayTags;
};
