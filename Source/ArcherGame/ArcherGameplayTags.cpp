// Fill out your copyright notice in the Description page of Project Settings.


#include "ArcherGameplayTags.h"

#include "GameplayTagsManager.h"

FArcherGameplayTags FArcherGameplayTags::ArcherGameplayTags;

void FArcherGameplayTags::InitializeNativeTags()
{
	UGameplayTagsManager& GameplayTagsManager = UGameplayTagsManager::Get();

	ArcherGameplayTags.AddAllTags();
	GameplayTagsManager.DoneAddingNativeTags();
}

void FArcherGameplayTags::AddAllTags()
{
	AddTag(InputTag_Dash, "InputTag.Move", "Move input");
	AddTag(InputTag_Jump, "InputTag.Jump", "Jumping input");
	AddTag(InputTag_Move, "InputTag.Move", "Moving input");
	AddTag(InputTag_Dash, "InputTag.Dash", "Dashing input");
	AddTag(InputTag_LookNFire, "InputTag.LookNFire", "Looking and firing input");
}

void FArcherGameplayTags::AddTag(FGameplayTag& outTag, const ANSICHAR* tagName, const ANSICHAR* tagComment)
{
	outTag = UGameplayTagsManager::Get().AddNativeGameplayTag(FName(tagName), FString(TEXT("(Native)") + FString(tagComment)));
}
