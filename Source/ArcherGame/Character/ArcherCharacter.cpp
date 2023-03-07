// Fill out your copyright notice in the Description page of Project Settings.


#include "ArcherCharacter.h"
#include "ArcherGame/ArcherGameplayTags.h"
#include "ArcherGame/Input/ArcherEnhancedInputComponent.h"

// Sets default values
AArcherCharacter::AArcherCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AArcherCharacter::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AArcherCharacter::Tick(float deltaTime)
{
	Super::Tick(deltaTime);
}

// Called to bind functionality to input
void AArcherCharacter::SetupPlayerInputComponent(UInputComponent* playerInputComponent)
{
	Super::SetupPlayerInputComponent(playerInputComponent);

	UArcherEnhancedInputComponent* archerEnhancedInputComponent = Cast<UArcherEnhancedInputComponent>(playerInputComponent);

	check(archerEnhancedInputComponent);

	const FArcherGameplayTags& gameplayTags = FArcherGameplayTags::Get();

	archerEnhancedInputComponent->BindActionByTag(InputConfig, gameplayTags.InputTag_Move, ETriggerEvent::Triggered, this, &AArcherCharacter::Input_Move);
	archerEnhancedInputComponent->BindActionByTag(InputConfig, gameplayTags.InputTag_Jump, ETriggerEvent::Triggered, this, &AArcherCharacter::Input_Jump);
	archerEnhancedInputComponent->BindActionByTag(InputConfig, gameplayTags.InputTag_Dash, ETriggerEvent::Triggered, this, &AArcherCharacter::Input_Dash);
	archerEnhancedInputComponent->BindActionByTag(InputConfig, gameplayTags.InputTag_LookNFire, ETriggerEvent::Started, this, &AArcherCharacter::Input_FireHold);
	archerEnhancedInputComponent->BindActionByTag(InputConfig, gameplayTags.InputTag_LookNFire, ETriggerEvent::Completed, this, &AArcherCharacter::Input_FireRelease);
}

void AArcherCharacter::Input_Move(const FInputActionValue& inputActionValue)
{
	if (Controller != nullptr)
	{
		const FVector2D MoveValue = inputActionValue.Get<FVector2D>();
		const FRotator MovementRotation(0.0f, Controller->GetControlRotation().Yaw, 0.0f);

		if (MoveValue.X != 0.0f)
		{
			const FVector MovementDirection = MovementRotation.RotateVector(FVector::RightVector);
			AddMovementInput(MovementDirection, MoveValue.X);
		}

		if (MoveValue.Y != 0.0f)
		{
			const FVector MovementDirection = MovementRotation.RotateVector(FVector::ForwardVector);
			AddMovementInput(MovementDirection, MoveValue.Y);
		}
	}
}

void AArcherCharacter::Input_Jump(const FInputActionValue& inputActionValue)
{
	UE_LOG(LogTemp, Warning, TEXT("lel"));

	Jump();
}

void AArcherCharacter::Input_Dash(const FInputActionValue& inputActionValue)
{
}

void AArcherCharacter::Input_FireHold(const FInputActionValue& inputActionValue)
{
}

void AArcherCharacter::Input_FireRelease(const FInputActionValue& inputActionValue)
{
}
