// Fill out your copyright notice in the Description page of Project Settings.


#include "ArcherPlayerCharacter.h"

#include "Pooler.h"
#include "ArcherGame/ArcherGameplayTags.h"
#include "ArcherGame/Camera/LyraCameraComponent.h"
#include "ArcherGame/Camera/LyraCameraMode.h"
#include "ArcherGame/Character/Ability/Attribute/ArcherHealthSet.h"
#include "ArcherGame/Character/Ability/Attribute/ArcherManaComponent.h"
#include "ArcherGame/Character/Ability/Attribute/ArcherManaSet.h"
#include "ArcherGame/Input/ArcherEnhancedInputComponent.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"

// Sets default values
AArcherPlayerCharacter::AArcherPlayerCharacter()
{
	CameraComponent = CreateDefaultSubobject<ULyraCameraComponent>("CameraComponent");

	ArcherPlayerManaComponent = CreateDefaultSubobject<UArcherManaComponent>("ArcherPlayerCharacterManaComponent");

	TeamId = 0;
}

void AArcherPlayerCharacter::AfterPossessedBy()
{
	Super::AfterPossessedBy();

	ArcherPlayerManaComponent->InitializeWithAbilitySystem(AbilitySystemComponent);

	CameraComponent->DetermineCameraModeDelegate.BindUObject(this, &ThisClass::DetermineCameraMode);
}

void AArcherPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

#if WITH_EDITOR
	FAutoConsoleVariableSink CVarSink(FConsoleCommandDelegate::CreateUObject(this, &ThisClass::OnConsoleVariableChanged));

	GetWorldTimerManager().SetTimerForNextTick([&]
	{
		OnConsoleVariableChanged();
	});
#endif
}

// Called to bind functionality to input
void AArcherPlayerCharacter::SetupPlayerInputComponent(UInputComponent* playerInputComponent)
{
	Super::SetupPlayerInputComponent(playerInputComponent);

	UArcherEnhancedInputComponent* archerEnhancedInputComponent = Cast<UArcherEnhancedInputComponent>(playerInputComponent);

	// make sure of archerEnhancedInputComponent
	check(archerEnhancedInputComponent);

	// bind tagged input's to functions
	archerEnhancedInputComponent->BindActionByTag(InputConfig, TAG_Input_Move, ETriggerEvent::Triggered, this, &AArcherPlayerCharacter::Input_Move);

	// bind tagged ability actions to pressed and released functions
	TArray<uint32> bindHandles;
	archerEnhancedInputComponent->BindAbilityActions(InputConfig, this, &AArcherPlayerCharacter::Input_AbilityInputTagPressed, &AArcherPlayerCharacter::Input_AbilityInputTagReleased, bindHandles);
}

void AArcherPlayerCharacter::Input_Move(const FInputActionValue& inputActionValue)
{
	if (Controller != nullptr)
	{
		// x is vertical, y is horizontal 
		InputVector = inputActionValue.Get<FVector>();

		const FRotator MovementRotation(0.0f, Controller->GetControlRotation().Yaw, 0.0f);

		auto AddInputToCharMovement = [&](const FVector& direction, float inputValue)
		{
			if (inputValue == 0)
			{
				return;
			}

			const FVector movementDirection = MovementRotation.RotateVector(direction);
			AddMovementInput(movementDirection, inputValue);
		};

		AddInputToCharMovement(FVector::RightVector, InputVector.X);
		AddInputToCharMovement(FVector::ForwardVector, InputVector.Y);
	}
}

void AArcherPlayerCharacter::Input_AbilityInputTagPressed(FGameplayTag gameplayTag)
{
	AbilitySystemComponent->AbilityInputPressed(gameplayTag);
}

void AArcherPlayerCharacter::Input_AbilityInputTagReleased(FGameplayTag gameplayTag)
{
	AbilitySystemComponent->AbilityInputReleased(gameplayTag);
}

void AArcherPlayerCharacter::OnConsoleVariableChanged() const
{
	auto HandleLooseGameplayTag = [&](const FGameplayTag gameplayTag, const bool bAdd)
	{
		if (bAdd)
		{
			GetArcherAbilitySystemComponent()->AddLooseGameplayTag(gameplayTag);
		}
		else
		{
			GetArcherAbilitySystemComponent()->RemoveLooseGameplayTag(gameplayTag);
		}
	};

	bool bDamageImmune = false;
	IConsoleManager::Get().FindConsoleVariable(TEXT("player.DamageImmunity.Activated"))->GetValue(bDamageImmune);
	HandleLooseGameplayTag(TAG_Gameplay_DamageImmunity, bDamageImmune);


	bool bManaImmune = false;
	IConsoleManager::Get().FindConsoleVariable(TEXT("player.ManaImmunity.Activated"))->GetValue(bManaImmune);
	HandleLooseGameplayTag(TAG_Gameplay_ManaImmunity, bManaImmune);
}

void AArcherPlayerCharacter::SetAbilityCameraMode(TSubclassOf<ULyraCameraMode> cameraMode, const FGameplayAbilitySpecHandle& gameplayAbilitySpecHandle)
{
	if (!cameraMode)
	{
		return;
	}

	AbilityCameraMode = cameraMode;
	AbilityCameraModeOwningSpecHandle = gameplayAbilitySpecHandle;
}

void AArcherPlayerCharacter::ClearAbilityCameraMode(const FGameplayAbilitySpecHandle& owningSpecHandle, const FGameplayAbilitySpecHandle& gameplayAbilitySpecHandle)
{
	// Only clear camera mode if giver is removing.
	if (AbilityCameraModeOwningSpecHandle != gameplayAbilitySpecHandle)
	{
		return;
	}

	AbilityCameraMode = nullptr;
	AbilityCameraModeOwningSpecHandle = FGameplayAbilitySpecHandle();
}

TSubclassOf<ULyraCameraMode> AArcherPlayerCharacter::DetermineCameraMode() const
{
	// Do we have a overriden camera mode from ability ? if so return that if not return our default camera mode.
	return AbilityCameraMode ? AbilityCameraMode : DefaultCameraMode;
}
