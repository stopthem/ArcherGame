// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GenericTeamAgentInterface.h"
#include "NativeGameplayTags.h"
#include "ArcherGame/Character/ArcherCharacter.h"
#include "ArcherPlayerCharacter.generated.h"

class ULyraCameraMode;
class UArcherManaComponent;
class UKismetStringLibrary;

/**
 * AArcherPlayerCharacter
 *
 * The character class that player uses
 */
UCLASS()
class ARCHERGAME_API AArcherPlayerCharacter : public AArcherCharacter, public IGenericTeamAgentInterface
{
	GENERATED_BODY()

public:
	AArcherPlayerCharacter();

public:
	UPROPERTY(EditAnywhere, Category="ArcherPlayerCharacter|Aim")
	FGameplayTagContainer AimingTagsContainer;

	UFUNCTION(BlueprintCallable, Category="ArcherPlayerCharacter|Aim")
	bool GetIsAiming()
	{
		return HasAnyMatchingGameplayTags(AimingTagsContainer);
	}

public:
	UFUNCTION(BlueprintCallable)
	APlayerController* GetPlayerController() const { return Cast<APlayerController>(GetController()); }

protected:
	virtual void AfterPossessedBy() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ArcherPlayerCharacter|Mana", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UArcherManaComponent> ArcherPlayerManaComponent;

	virtual void BeginPlay() override;

#pragma region Input Handling

public:
	// The input config which filled with Gameplay Tagged Input Actions
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="ArcherPlayerCharacter|Input")
	TObjectPtr<UInputConfig> InputConfig;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(UInputComponent* playerInputComponent) override;

private:
	void Input_Move(const FInputActionValue& inputActionValue);
	void Input_AbilityInputTagPressed(FGameplayTag gameplayTag);
	void Input_AbilityInputTagReleased(FGameplayTag gameplayTag);

	FVector InputVector;

#pragma endregion

private:
	FGenericTeamId TeamId;

	virtual FGenericTeamId GetGenericTeamId() const override
	{
		return TeamId;
	}

public:
	// only friend for hud purposes
	friend class AArcherDebugHUD;

private:
	UFUNCTION()
	void OnConsoleVariableChanged() const;

#pragma region Camera Handling

public:
	UFUNCTION(BlueprintCallable)
	// Overrides the camera from an active gameplay ability
	void SetAbilityCameraMode(TSubclassOf<ULyraCameraMode> cameraMode, const FGameplayAbilitySpecHandle& gameplayAbilitySpecHandle);

	UFUNCTION(BlueprintCallable)
	// Clears the camera override if it is set 
	void ClearAbilityCameraMode(const FGameplayAbilitySpecHandle& owningSpecHandle, const FGameplayAbilitySpecHandle& gameplayAbilitySpecHandle);

private:
	// Camera component calls this with its special multicast RetVal.
	TSubclassOf<ULyraCameraMode> DetermineCameraMode() const;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ArcherPlayerCharacter|Camera", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<ULyraCameraComponent> CameraComponent;

	// This is what camera mode we will use if there is no ability override.
	UPROPERTY(EditAnywhere, Category = "ArcherPlayerCharacter|Camera")
	TSubclassOf<ULyraCameraMode> DefaultCameraMode;

	// The camera mode set by a ability.
	UPROPERTY()
	TSubclassOf<ULyraCameraMode> AbilityCameraMode;

	// Latest camera mod setter ability spec handle.
	FGameplayAbilitySpecHandle AbilityCameraModeOwningSpecHandle;
#pragma endregion
};
