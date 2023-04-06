// Fill out your copyright notice in the Description page of Project Settings.


#include "ArcherGameplayAbility.h"

#include "ArcherGameplayEffectContext.h"
#include "ArcherGame/Character/Player/ArcherPlayerCharacter.h"
#include "GameFramework/GameplayMessageSubsystem.h"
#include "Kismet/KismetStringLibrary.h"
#include "Messages/ArcherAbilityCanActivateMessage.h"
#include "Messages/ArcherInteractionDurationMessage.h"

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

void UArcherGameplayAbility::TryActivateAbilityOnSpawn(const FGameplayAbilityActorInfo* actorInfo, const FGameplayAbilitySpec& spec) const
{
	// Try to activate if activation policy is on spawn.
	// lyra code
	if (actorInfo && !spec.IsActive() && (ActivationPolicy == EArcherAbilityActivationPolicy::OnSpawn))
	{
		UAbilitySystemComponent* ASC = actorInfo->AbilitySystemComponent.Get();
		const AActor* AvatarActor = actorInfo->AvatarActor.Get();

		// If avatar actor is torn off or about to die, don't try to activate until we get the new one.
		if (ASC && AvatarActor && !AvatarActor->GetTearOff() && (AvatarActor->GetLifeSpan() <= 0.0f))
		{
			const bool bIsLocalExecution = (NetExecutionPolicy == EGameplayAbilityNetExecutionPolicy::LocalPredicted) || (NetExecutionPolicy == EGameplayAbilityNetExecutionPolicy::LocalOnly);
			const bool bIsServerExecution = (NetExecutionPolicy == EGameplayAbilityNetExecutionPolicy::ServerOnly) || (NetExecutionPolicy == EGameplayAbilityNetExecutionPolicy::ServerInitiated);

			const bool bClientShouldActivate = actorInfo->IsLocallyControlled() && bIsLocalExecution;
			const bool bServerShouldActivate = actorInfo->IsNetAuthority() && bIsServerExecution;

			if (bClientShouldActivate || bServerShouldActivate)
			{
				ASC->TryActivateAbility(spec.Handle);
			}
		}
	}
}

bool UArcherGameplayAbility::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags,
                                                FGameplayTagContainer* OptionalRelevantTags) const
{
	const bool bCanActivateAbility = Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags);

	if (MessageTagInfoHolder.CanActivateMessageInfo.bShouldBroadcast)
	{
		BroadcastCanActivate(ActorInfo, bCanActivateAbility);
	}

	return bCanActivateAbility;
}

bool UArcherGameplayAbility::CommitAbility(const FGameplayAbilitySpecHandle handle, const FGameplayAbilityActorInfo* actorInfo, const FGameplayAbilityActivationInfo activationInfo, FGameplayTagContainer* optionalRelevantTags)
{
	const bool bCommitAbility = Super::CommitAbility(handle, actorInfo, activationInfo, optionalRelevantTags);

	if (bCommitAbility && MessageTagInfoHolder.CooldownMessageInfo.bShouldBroadcast)
	{
		BroadcastCooldown();
	}

	return bCommitAbility;
}

FGameplayEffectContextHandle UArcherGameplayAbility::MakeEffectContext(const FGameplayAbilitySpecHandle handle, const FGameplayAbilityActorInfo* actorInfo) const
{
	FGameplayEffectContextHandle ContextHandle = Super::MakeEffectContext(handle, actorInfo);

	FArcherGameplayEffectContext* EffectContext = FArcherGameplayEffectContext::ExtractEffectContext(ContextHandle);
	check(EffectContext);

	check(actorInfo);

	const UObject* sourceObject = GetSourceObject(handle, actorInfo);

	AActor* instigator = actorInfo ? actorInfo->OwnerActor.Get() : nullptr;

	EffectContext->AddInstigator(instigator, instigator);
	EffectContext->AddSourceObject(sourceObject);

	return ContextHandle;
}

void UArcherGameplayAbility::BroadcastCanActivate(const FGameplayAbilityActorInfo* actorInfo, const bool bCanActivate) const
{
	FArcherAbilityCanActivateMessage AbilityCostMessage;
	AbilityCostMessage.bCanActivate = bCanActivate;
	UGameplayMessageSubsystem::Get(actorInfo->OwnerActor.Get()).BroadcastMessage(MessageTagInfoHolder.CanActivateMessageInfo.MessageTag, AbilityCostMessage);
}

void UArcherGameplayAbility::BroadcastCooldown() const
{
	FArcherInteractionDurationMessage durationMessage;
	durationMessage.Instigator = GetAvatarActorFromActorInfo();
	durationMessage.Duration = GetCooldownTimeRemaining();
	UGameplayMessageSubsystem::Get(GetAvatarActorFromActorInfo()).BroadcastMessage(MessageTagInfoHolder.CooldownMessageInfo.MessageTag, durationMessage);
}
