// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "NativeGameplayTags.h"
#include "ArcherGame/Character/ArcherCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "ArcherPlayerCharacter.generated.h"

/**
 * 
 */
UCLASS()
class ARCHERGAME_API AArcherPlayerCharacter : public AArcherCharacter
{
	GENERATED_BODY()

public:
	AArcherPlayerCharacter();

	UPROPERTY(BlueprintReadWrite, Category="Aim")
	bool IsAiming = false;

	UFUNCTION(BlueprintCallable)
	APlayerController* GetPlayerController() const { return Cast<APlayerController>(GetController()); }

protected:
	virtual void PostInitializeComponents() override;

#pragma region Animation

public:
	// The value of where angle between player forward and where the player wants to go in aiming mode
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Movement")
	float PlayerMovementAngle;

	// The montage that plays when player stops after running or dashing if can
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Animation|Stopping")
	TObjectPtr<UAnimMontage> StoppingMontage;

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
	TObjectPtr<UInputConfig> InputConfig;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(UInputComponent* playerInputComponent) override;

private:
	void Input_Move(const FInputActionValue& inputActionValue);
	void Input_AbilityInputTagPressed(FGameplayTag gameplayTag);
	void Input_AbilityInputTagReleased(FGameplayTag gameplayTag);

	FVector InputVector;

#pragma endregion

public:
	// only friend for hud purposes
	friend class AArcherDebugHUD;
};
