// Fill out your copyright notice in the Description page of Project Settings.


#include "ArcherDebugHUD.h"
#include "ArcherGame/Character/Player/ArcherPlayerCharacter.h"

void AArcherDebugHUD::DrawHUD()
{
	Super::DrawHUD();

	const AArcherPlayerCharacter* archerCharacter = Cast<AArcherPlayerCharacter>(GetOwningPawn());

	if (!archerCharacter)return;

	// AddBool(L"Can Dash", archerCharacter->bCanDash);
}
