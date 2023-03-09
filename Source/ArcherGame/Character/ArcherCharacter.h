// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ArcherCharacter.generated.h"

struct FInputActionValue;
class UInputConfig;
UCLASS()
class ARCHERGAME_API AArcherCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AArcherCharacter();

#pragma region Input Handling

public:
	// The input config which filled with Gameplay Tagged Input Actions
	UPROPERTY(EditDefaultsOnly, Category="Input")
	UInputConfig* InputConfig;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(UInputComponent* playerInputComponent) override;

private:
	void Input_Move(const FInputActionValue& inputActionValue);

	void Input_Jump();

	void Input_Dash();

	void Input_FireHold();
	void Input_FireRelease();

	FVector InputVector;

#pragma endregion

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float deltaTime) override;

	// only friend for hud purposes
	friend class AArcherDebugHUD;
};
