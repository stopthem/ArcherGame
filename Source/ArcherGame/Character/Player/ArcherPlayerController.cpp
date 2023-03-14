// Fill out your copyright notice in the Description page of Project Settings.


#include "ArcherPlayerController.h"

#include "ArcherPlayerCharacter.h"

AArcherPlayerController::AArcherPlayerController()
{
}

void AArcherPlayerController::PostProcessInput(const float DeltaTime, const bool bGamePaused)
{
	if (const AArcherPlayerCharacter* archerPlayerCharacter = Cast<AArcherPlayerCharacter>(GetCharacter()))
	{
		archerPlayerCharacter->GetArcherAbilitySystemComponent()->ProcessAbilityInput(DeltaTime, bGamePaused);
	}
	Super::PostProcessInput(DeltaTime, bGamePaused);
}
