// Fill out your copyright notice in the Description page of Project Settings.


#include "ArcherDebugHUD.h"

#include "ArcherGame/ArcherGameGameModeBase.h"
#include "ArcherGame/Character/Ability/Attribute/ArcherHealthSet.h"
#include "ArcherGame/Character/Ability/Attribute/ArcherManaSet.h"
#include "ArcherGame/Character/Player/ArcherPlayerCharacter.h"

void AArcherDebugHUD::DrawHUD()
{
	Super::DrawHUD();

	const AArcherPlayerCharacter* archerCharacter = Cast<AArcherPlayerCharacter>(GetOwningPawn());

	if (!archerCharacter)return;

#if !WITH_EDITOR
	return;
#endif

	AddBool(L"Is Damage Immune", archerCharacter->HasMatchingGameplayTag(TAG_Gameplay_DamageImmunity));
	AddBool(L"Is Mana Immune", archerCharacter->HasMatchingGameplayTag(TAG_Gameplay_ManaImmunity));

	bool bBehavioursActive = false;
	IConsoleManager::Get().FindConsoleVariable(TEXT("ai.AllBehaviorTrees.Activated"))->GetValue(bBehavioursActive);
	AddBool(L"Is Behaviour Trees active", bBehavioursActive);

	AddString(L"Game Status", UEnum::GetValueAsString(AArcherGameGameModeBase::Get(GetWorld())->GetGameStatus()));
}
