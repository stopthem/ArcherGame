// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NativeGameplayTags.h"
#include "ArcherGame/ArcherGameplayTags.h"
#include "ArcherGame/Character/ArcherCharacter.h"
#include "ArcherPlayerCharacter.generated.h"

class UKismetStringLibrary;

/**
 * AArcherPlayerCharacter
 *
 * The character class that player uses
 */
UCLASS()
class ARCHERGAME_API AArcherPlayerCharacter : public AArcherCharacter
{
	GENERATED_BODY()

public:
	AArcherPlayerCharacter();

	UFUNCTION(BlueprintCallable, Category="Aim Ability")
	bool GetIsAiming()
	{
		return GetArcherAbilitySystemComponent()->IsAbilityActiveWithTag(TAG_Input_Aim);
	}

	UFUNCTION(BlueprintCallable)
	APlayerController* GetPlayerController() const { return Cast<APlayerController>(GetController()); }

protected:
	virtual void PostInitializeComponents() override;

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
