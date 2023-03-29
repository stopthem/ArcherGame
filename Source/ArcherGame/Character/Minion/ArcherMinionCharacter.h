// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ArcherGame/Character/ArcherCharacter.h"
#include "ArcherMinionCharacter.generated.h"

/**
 * AArcherMinionCharacter
 *
 * Class that all minion type characters uses.
 */
UCLASS()
class ARCHERGAME_API AArcherMinionCharacter : public AArcherCharacter
{
	GENERATED_BODY()

public:
	AArcherMinionCharacter();
};
