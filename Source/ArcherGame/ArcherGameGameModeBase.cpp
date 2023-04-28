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

	GEngine->bSubtitlesEnabled = false;
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
			// I don't know why the struct or a local variable changing in timers levelEndMessage.bWin changes to fail so i need to check game status.
			CreateWidget<UUserWidget>(GetWorld(), GameStatus == Win ? WinScreenWidgetClass : FailScreenWidgetClass, FName(TEXT("LevelEndScreenWidget")))->AddToViewport();
		})
		->SetUseGlobalTimeDilation(false);
}
