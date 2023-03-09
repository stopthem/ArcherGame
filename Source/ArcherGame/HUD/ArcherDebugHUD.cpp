// Fill out your copyright notice in the Description page of Project Settings.


#include "ArcherDebugHUD.h"
#include "ArcherGame/Character/ArcherCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

void AArcherDebugHUD::DrawHUD()
{
	Super::DrawHUD();

	const AArcherCharacter* archerCharacter = Cast<AArcherCharacter>(GetOwningPawn());

	if (!archerCharacter)return;

	AddBool(L"Is Grounded", archerCharacter->GetCharacterMovement()->IsMovingOnGround());
	// AddBool(L"Can Dash", archerCharacter->bCanDash);
}
