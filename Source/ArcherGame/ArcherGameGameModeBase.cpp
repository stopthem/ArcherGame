// Copyright Epic Games, Inc. All Rights Reserved.


#include "ArcherGameGameModeBase.h"

#include "ArcherGameplayTags.h"
#include "FCTween.h"
#include "Blueprint/UserWidget.h"
#include "Character/Ability/Messages/ArcherLevelEndMessage.h"
#include "GameFramework/GameplayMessageSubsystem.h"

AArcherGameGameModeBase::AArcherGameGameModeBase()
{
}

void AArcherGameGameModeBase::SetGameStatus(EGameStatus gameStatus)
{
	GameStatus = gameStatus;
}

void AArcherGameGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	GameplayMessageListenerHandle = UGameplayMessageSubsystem::Get(this).RegisterListener<FArcherLevelEndMessage, AArcherGameGameModeBase>(TAG_GameplayEvent_LevelEnd, this, &ThisClass::OnLevelEnd);
}

void AArcherGameGameModeBase::OnLevelEnd(FGameplayTag messageChannelTag, const FArcherLevelEndMessage& levelEndMessage)
{
	// Unregister from listening message so nothing weird happens.
	UGameplayMessageSubsystem::Get(this).UnregisterListener(GameplayMessageListenerHandle);

	GameStatus = levelEndMessage.bWin ? Win : Fail;

	// Using a FCTween just because it can ignore time dilation.
	FCTween::Play(0, 1, [&](float)
		{
		}, DelayBeforeShowingWidget, EFCEase::Linear)
		->SetOnComplete([&]
		{
			CreateWidget<UUserWidget>(GetWorld(), levelEndMessage.bWin ? WinScreenWidgetClass : FailScreenWidgetClass, FName(TEXT("LevelEndScreenWidget")))->AddToViewport();
		})
		->SetUseGlobalTimeDilation(false);
}
