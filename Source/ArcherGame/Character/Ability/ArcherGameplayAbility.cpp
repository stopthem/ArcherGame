// Fill out your copyright notice in the Description page of Project Settings.


#include "ArcherGameplayAbility.h"

#include "AbilitySystemGlobals.h"
#include "ArcherGameplayEffectContext.h"
#include "ArcherGame/Character/Player/ArcherPlayerCharacter.h"
#include "GameFramework/GameplayMessageSubsystem.h"
#include "Messages/ArcherAbilityCanActivateMessage.h"
#include "Messages/ArcherAbilityIsActiveMessage.h"
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

void UArcherGameplayAbility::AddRemoveLooseGameplayTags(bool bAdd, FGameplayTag gameplayTag, int count)
{
	if (bAdd)
	{
		GetAbilitySystemComponentFromActorInfo()->AddLooseGameplayTag(gameplayTag, count);
	}
	else
	{
		GetAbilitySystemComponentFromActorInfo()->RemoveLooseGameplayTag(gameplayTag, count);
	}
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

	if (bCommitAbility)
	{
		if (MessageTagInfoHolder.CooldownMessageInfo.bShouldBroadcast)
		{
			BroadcastCooldown();
		}
		if (MessageTagInfoHolder.IsActiveMessageInfo.bShouldBroadcast)
		{
			BroadcastIsActive(actorInfo, true);
		}
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

bool UArcherGameplayAbility::DoesAbilitySatisfyTagRequirements(const UAbilitySystemComponent& AbilitySystemComponent, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags,
                                                               FGameplayTagContainer* OptionalRelevantTags) const
{
	// Specialized version to handle death exclusion and AbilityTags expansion via ASC

	bool bBlocked = false;
	bool bMissing = false;

	const UAbilitySystemGlobals& AbilitySystemGlobals = UAbilitySystemGlobals::Get();
	const FGameplayTag& BlockedTag = AbilitySystemGlobals.ActivateFailTagsBlockedTag;
	const FGameplayTag& MissingTag = AbilitySystemGlobals.ActivateFailTagsMissingTag;

	// Check if any of this ability's tags are currently blocked
	if (AbilitySystemComponent.AreAbilityTagsBlocked(AbilityTags))
	{
		bBlocked = true;
	}

	const UArcherAbilitySystemComponent* archerASC = Cast<UArcherAbilitySystemComponent>(&AbilitySystemComponent);
	static FGameplayTagContainer allRequiredTags;
	static FGameplayTagContainer allBlockedTags;

	allRequiredTags = ActivationRequiredTags;
	allBlockedTags = ActivationBlockedTags;

	// Expand our ability tags to add additional required/blocked tags
	if (archerASC)
	{
		archerASC->GetAdditionalActivationTagRequirements(AbilityTags, allRequiredTags, allBlockedTags);
	}

	// Check to see the required/blocked tags for this ability
	if (allBlockedTags.Num() || allRequiredTags.Num())
	{
		static FGameplayTagContainer AbilitySystemComponentTags;

		AbilitySystemComponentTags.Reset();
		AbilitySystemComponent.GetOwnedGameplayTags(AbilitySystemComponentTags);

		if (AbilitySystemComponentTags.HasAny(allBlockedTags))
		{
			bBlocked = true;
		}

		if (!AbilitySystemComponentTags.HasAll(allRequiredTags))
		{
			bMissing = true;
		}
	}

	if (SourceTags != nullptr)
	{
		if (SourceBlockedTags.Num() || SourceRequiredTags.Num())
		{
			if (SourceTags->HasAny(SourceBlockedTags))
			{
				bBlocked = true;
			}

			if (!SourceTags->HasAll(SourceRequiredTags))
			{
				bMissing = true;
			}
		}
	}

	if (TargetTags != nullptr)
	{
		if (TargetBlockedTags.Num() || TargetRequiredTags.Num())
		{
			if (TargetTags->HasAny(TargetBlockedTags))
			{
				bBlocked = true;
			}

			if (!TargetTags->HasAll(TargetRequiredTags))
			{
				bMissing = true;
			}
		}
	}

	if (bBlocked)
	{
		if (OptionalRelevantTags && BlockedTag.IsValid())
		{
			OptionalRelevantTags->AddTag(BlockedTag);
		}
		return false;
	}
	if (bMissing)
	{
		if (OptionalRelevantTags && MissingTag.IsValid())
		{
			OptionalRelevantTags->AddTag(MissingTag);
		}
		return false;
	}

	return true;
}

void UArcherGameplayAbility::EndAbility(const FGameplayAbilitySpecHandle handle, const FGameplayAbilityActorInfo* actorInfo, const FGameplayAbilityActivationInfo activationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	if (MessageTagInfoHolder.IsActiveMessageInfo.bShouldBroadcast)
	{
		BroadcastIsActive(actorInfo, false);
	}
	Super::EndAbility(handle, actorInfo, activationInfo, bReplicateEndAbility, bWasCancelled);
}

void UArcherGameplayAbility::BroadcastCanActivate(const FGameplayAbilityActorInfo* actorInfo, const bool bCanActivate) const
{
	FArcherAbilityCanActivateMessage AbilityCostMessage;
	AbilityCostMessage.bCanActivate = bCanActivate;
	UGameplayMessageSubsystem::Get(actorInfo->OwnerActor.Get()).BroadcastMessage(MessageTagInfoHolder.CanActivateMessageInfo.MessageTag, AbilityCostMessage);
}

void UArcherGameplayAbility::BroadcastIsActive(const FGameplayAbilityActorInfo* actorInfo, const bool bIsAbilityActive) const
{
	FArcherAbilityIsActiveMessage abilityIsActiveMessage;
	abilityIsActiveMessage.bIsActive = bIsAbilityActive;
	UGameplayMessageSubsystem::Get(actorInfo->OwnerActor.Get()).BroadcastMessage(MessageTagInfoHolder.IsActiveMessageInfo.MessageTag, abilityIsActiveMessage);
}

void UArcherGameplayAbility::BroadcastCooldown() const
{
	FArcherInteractionDurationMessage durationMessage;
	durationMessage.Instigator = GetAvatarActorFromActorInfo();
	durationMessage.Duration = GetCooldownTimeRemaining();
	UGameplayMessageSubsystem::Get(GetAvatarActorFromActorInfo()).BroadcastMessage(MessageTagInfoHolder.CooldownMessageInfo.MessageTag, durationMessage);
}
