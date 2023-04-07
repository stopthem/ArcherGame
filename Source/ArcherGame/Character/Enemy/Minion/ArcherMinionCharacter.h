// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ArcherGame/Character/Enemy/ArcherEnemyCharacter.h"

#include "ArcherMinionCharacter.generated.h"

/**
 * AArcherMinionCharacter
 *
 * Class that all minion type characters uses.
 */
UCLASS()
class ARCHERGAME_API AArcherMinionCharacter : public AArcherEnemyCharacter
{
	GENERATED_BODY()

public:
	AArcherMinionCharacter();
};
