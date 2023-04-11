// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GenericTeamAgentInterface.h"
#include "NativeGameplayTags.h"
#include "ArcherGame/ArcherGameplayTags.h"
#include "ArcherGame/Character/ArcherCharacter.h"
#include "ArcherPlayerCharacter.generated.h"

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

	UPROPERTY(EditAnywhere, Category="ArcherPlayerCharacter|Aim")
	FGameplayTagContainer AimingTagsContainer;

	UFUNCTION(BlueprintCallable, Category="ArcherPlayerCharacter|Aim")
	bool GetIsAiming()
	{
		return HasAnyMatchingGameplayTags(AimingTagsContainer);
	}

	UFUNCTION(BlueprintCallable)
	APlayerController* GetPlayerController() const { return Cast<APlayerController>(GetController()); }

protected:
	virtual void InitializeAbilitySystem() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ArcherPlayerCharacter|Mana", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UArcherManaComponent> ManaComponent;

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
};
