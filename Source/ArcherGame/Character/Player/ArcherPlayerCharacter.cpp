// Fill out your copyright notice in the Description page of Project Settings.


#include "ArcherPlayerCharacter.h"

#include "Pooler.h"
#include "ArcherGame/ArcherGameplayTags.h"
#include "ArcherGame/Character/Ability/Attribute/ArcherHealthSet.h"
#include "ArcherGame/Character/Ability/Attribute/ArcherManaComponent.h"
#include "ArcherGame/Character/Ability/Attribute/ArcherManaSet.h"
#include "ArcherGame/Input/ArcherEnhancedInputComponent.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"

// Sets default values
AArcherPlayerCharacter::AArcherPlayerCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	// PrimaryActorTick.bCanEverTick = true;

	ManaComponent = CreateDefaultSubobject<UArcherManaComponent>("ArcherPlayerCharacterManaComponent");

	TeamId = 0;
}

void AArcherPlayerCharacter::InitializeAbilitySystem()
{
	Super::InitializeAbilitySystem();

	ManaComponent->InitializeWithAbilitySystem(AbilitySystemComponent);
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

		// add vertical input
		AddMovementInput(FVector::ForwardVector, InputVector.X);

		// add horizontal input
		AddMovementInput(FVector::RightVector, InputVector.Y);

		// return if there is no input so characters rotation doesnt reset to 0 0 0
		if (InputVector.Size() == 0)
		{
			return;
		}

		// if aiming, control rotation is handled by various aiming abilities. (ex. GA_Archer_Aim, GA_Archer_Ultimate)
		if (GetIsAiming())
		{
			return;
		}

		// set the character movement rotation target to input vector's rotation 
		Controller->SetControlRotation(InputVector.Rotation());
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
