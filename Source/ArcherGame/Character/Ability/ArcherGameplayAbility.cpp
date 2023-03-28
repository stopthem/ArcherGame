// Fill out your copyright notice in the Description page of Project Settings.


#include "ArcherGameplayAbility.h"

#include "ArcherGame/Character/Player/ArcherPlayerCharacter.h"

UArcherGameplayAbility::UArcherGameplayAbility()
{
}

AArcherPlayerCharacter* UArcherGameplayAbility::GetArcherPlayerCharacterFromActorInfo() const
{
	return CurrentActorInfo ? Cast<AArcherPlayerCharacter>(CurrentActorInfo->AvatarActor.Get()) : nullptr;
}

AArcherCharacter* UArcherGameplayAbility::GetArcherCharacterFromActorInfo() const
{
	return CurrentActorInfo ? Cast<AArcherCharacter>(CurrentActorInfo->AvatarActor.Get()) : nullptr;
}

void UArcherGameplayAbility::TryActivateAbilityOnSpawn(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) const
{
	// Try to activate if activation policy is on spawn.
	// lyra code
	if (ActorInfo && !Spec.IsActive() && (ActivationPolicy == EArcherAbilityActivationPolicy::OnSpawn))
	{
		UAbilitySystemComponent* ASC = ActorInfo->AbilitySystemComponent.Get();
		const AActor* AvatarActor = ActorInfo->AvatarActor.Get();

		// If avatar actor is torn off or about to die, don't try to activate until we get the new one.
		if (ASC && AvatarActor && !AvatarActor->GetTearOff() && (AvatarActor->GetLifeSpan() <= 0.0f))
		{
			const bool bIsLocalExecution = (NetExecutionPolicy == EGameplayAbilityNetExecutionPolicy::LocalPredicted) || (NetExecutionPolicy == EGameplayAbilityNetExecutionPolicy::LocalOnly);
			const bool bIsServerExecution = (NetExecutionPolicy == EGameplayAbilityNetExecutionPolicy::ServerOnly) || (NetExecutionPolicy == EGameplayAbilityNetExecutionPolicy::ServerInitiated);

			const bool bClientShouldActivate = ActorInfo->IsLocallyControlled() && bIsLocalExecution;
			const bool bServerShouldActivate = ActorInfo->IsNetAuthority() && bIsServerExecution;

			if (bClientShouldActivate || bServerShouldActivate)
			{
				ASC->TryActivateAbility(Spec.Handle);
			}
		}
	}
}
