// Fill out your copyright notice in the Description page of Project Settings.


#include "ArcherAssetManager.h"

#include "ArcherGameplayTags.h"

UArcherAssetManager& UArcherAssetManager::Get()
{
	check(GEngine);

	UArcherAssetManager* archerAssetManager = Cast<UArcherAssetManager>(GEngine->AssetManager);
	return *archerAssetManager;
}

void UArcherAssetManager::StartInitialLoading()
{
	Super::StartInitialLoading();

	// Load native tags
	FArcherGameplayTags::InitializeNativeTags();
}
