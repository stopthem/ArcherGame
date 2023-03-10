// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
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

#pragma region Animation

public:
	// The value of where angle between player forward and where the player wants to go in aiming mode
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Movement")
	float PlayerMovementAngle;

	// The montage that plays when player stops after running or dashing if can
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Animation|Stopping")
	UAnimMontage* StoppingMontage;

	// The percent that has to be less than current walk speed / max walk speed to play stopping montage
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Animation|Stopping", meta=(UIMin = "0", UIMax="1"))
	float StopMontagePlayConditionPercent = 0.8f;

private:
	float GetNormalizedWalkSpeed() const
	{
		return GetCharacterMovement()->Velocity.Size2D() / GetCharacterMovement()->MaxWalkSpeed;
	}

	TObjectPtr<UAnimInstance> AnimInstance;
#pragma endregion

#pragma region Input Handling

public:
	// The input config which filled with Gameplay Tagged Input Actions
	UPROPERTY(EditDefaultsOnly, Category="Input")
	UInputConfig* InputConfig;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(UInputComponent* playerInputComponent) override;

private:
	void Input_Move(const FInputActionValue& inputActionValue);

	void Input_Dash();

	void Input_FireHold();
	void Input_FireRelease();

	FVector InputVector;

#pragma endregion

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Get anim instance
	virtual void PostInitializeComponents() override;

	// Called every frame
	virtual void Tick(float deltaTime) override;

public:
	// only friend for hud purposes
	friend class AArcherDebugHUD;
};
