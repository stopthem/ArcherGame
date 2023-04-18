// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "GameFramework/GameplayMessageSubsystem.h"
#include "ArcherGameGameModeBase.generated.h"

struct FArcherLevelEndMessage;

UENUM()
enum EGameStatus
{
	Win,
	Fail,
	WaveOnProgress,
	WaitingWave
};

/**
 * AArcherGameGameModeBase
 *
 * Game mode this project uses. 
 */
UCLASS()
class ARCHERGAME_API AArcherGameGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	AArcherGameGameModeBase();

	UPROPERTY(EditAnywhere, Category="GameMode|Level End")
	float DelayBeforeShowingWidget = 3.0f;

	// Widget class to spawn when we win.
	UPROPERTY(EditAnywhere, Category="GameMode|Level End")
	TSubclassOf<UUserWidget> WinScreenWidgetClass;

	// Widget class to spawn when we fail.
	UPROPERTY(EditAnywhere, Category="GameMode|Level End")
	TSubclassOf<UUserWidget> FailScreenWidgetClass;

	void SetGameStatus(EGameStatus gameStatus);
	EGameStatus GetGameStatus() const { return GameStatus; }

	static AArcherGameGameModeBase* Get(const UWorld* world) { return Cast<AArcherGameGameModeBase>(world->GetAuthGameMode()); }

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	virtual void OnLevelEnd(FGameplayTag messageChannelTag, const FArcherLevelEndMessage& levelEndMessage);

private:
	EGameStatus GameStatus = WaitingWave;

	FGameplayMessageListenerHandle GameplayMessageListenerHandle;
};
