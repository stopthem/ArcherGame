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

void AArcherCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	AnimInstance = GetMesh()->GetAnimInstance();
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

	// make sure of archerEnhancedInputComponent
	check(archerEnhancedInputComponent);

	// bind input's to functions
	archerEnhancedInputComponent->BindActionByTag(InputConfig, TAG_INPUT_MOVE, ETriggerEvent::Triggered, this, &AArcherCharacter::Input_Move);
	archerEnhancedInputComponent->BindActionByTag(InputConfig, TAG_INPUT_DASH, ETriggerEvent::Triggered, this, &AArcherCharacter::Input_Dash);
	archerEnhancedInputComponent->BindActionByTag(InputConfig, TAG_INPUT_LOOKNFIRE, ETriggerEvent::Started, this, &AArcherCharacter::Input_FireHold);
	archerEnhancedInputComponent->BindActionByTag(InputConfig, TAG_INPUT_LOOKNFIRE, ETriggerEvent::Completed, this, &AArcherCharacter::Input_FireRelease);
}

void AArcherCharacter::Input_Move(const FInputActionValue& inputActionValue)
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
			// if character is running(determined by if current walking speed / max walking speed is more than StopMontagePlayConditionPercent) play the stopping montage
			if (GetNormalizedWalkSpeed() >= StopMontagePlayConditionPercent && StoppingMontage->IsValidToPlay())
			{
				PlayAnimMontage(StoppingMontage);
			}
			return;
		}

		if (AnimInstance->Montage_IsPlaying(StoppingMontage))
		{
			AnimInstance->Montage_Stop(StoppingMontage->BlendOut.GetBlendTime(), StoppingMontage);
		}

		// set the character movement rotation target to input vector's rotation 
		Controller->SetControlRotation(InputVector.Rotation());
	}
}

void AArcherCharacter::Input_Dash()
{
}

void AArcherCharacter::Input_FireHold()
{
}

void AArcherCharacter::Input_FireRelease()
{
}
