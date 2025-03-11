// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/CharacterAbilitySystemComponent.h"
#include "AbilitySystem/Ability/BaseGameplayAbility.h"//
#include "DungeonGameplayTags.h"//
#include "AbilitySystemBlueprintLibrary.h"//

void UCharacterAbilitySystemComponent::AddCharacterAbilities(const TArray<TSubclassOf<UGameplayAbility>>& Abilities)
{
	for (const TSubclassOf<UGameplayAbility> Ability : Abilities)
	{
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(Ability, 1);
		if (const UBaseGameplayAbility* GrantedAbility = Cast<UBaseGameplayAbility>(AbilitySpec.Ability))
		{
			AbilitySpec.DynamicAbilityTags.AddTag(GrantedAbility->StartupInputTag);
			GiveAbility(AbilitySpec);
		}
	}
}

void UCharacterAbilitySystemComponent::AddCharacterPassiveAbilities(const TArray<TSubclassOf<UGameplayAbility>>& PassiveAbilities)
{
	for (const TSubclassOf<UGameplayAbility> Ability : PassiveAbilities)
	{
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(Ability, 1);
		GiveAbilityAndActivateOnce(AbilitySpec);
	}
}

void UCharacterAbilitySystemComponent::AbilityInputTagPressed(const FGameplayTag& InputTag)
{
	if (!InputTag.IsValid())return;

	FScopedAbilityListLock ActiveScopLock(*this);
	for (auto& AbilitySpec : GetActivatableAbilities())
	{
		if (InputTag.MatchesTag(FGameplayTag::RequestGameplayTag("Input.Async")))
		{
			if(AbilitySpec.DynamicAbilityTags.HasTag(FGameplayTag::RequestGameplayTag("Input.Async")))
			{
				Cast<UBaseGameplayAbility>(AbilitySpec.Ability)->AsyncInputTag = InputTag;
				if (!AbilitySpec.IsActive())
				{
					TryActivateAbility(AbilitySpec.Handle);
					return;
				}
			}
		}

		/*InteractAction do not need held triger*/
		if (InputTag == FDungeonGameplayTags::Get().Input_E && AbilitySpec.DynamicAbilityTags.HasTag(FDungeonGameplayTags::Get().Input_E))
		{
			if (!AbilitySpec.IsActive())
			{
				TryActivateAbility(AbilitySpec.Handle);
				return;
			}
		}

		if (AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag))
		{
			AbilitySpecInputPressed(AbilitySpec);
			if (AbilitySpec.IsActive())
			{
				InvokeReplicatedEvent(EAbilityGenericReplicatedEvent::InputPressed, AbilitySpec.Handle, AbilitySpec.ActivationInfo.GetActivationPredictionKey());
			}
		}
	}
}

void UCharacterAbilitySystemComponent::AbilityInputTagHeld(const FGameplayTag& InputTag)
{
	if (!InputTag.IsValid())return;

	FScopedAbilityListLock ActiveScopLock(*this);

	if (InputTag == FDungeonGameplayTags::Get().Input_Space)
	{
		HeldTime += GetWorld()->GetDeltaSeconds();
	}

	if (InputTag == FDungeonGameplayTags::Get().Input_E)return;

	for (auto& AbilitySpec : GetActivatableAbilities())
	{
		FGameplayTag Tag = InputTag;
		if (InputTag == FDungeonGameplayTags::Get().Input_Space)
		{
			Tag = FDungeonGameplayTags::Get().Ability_Run;
		}

		if (AbilitySpec.DynamicAbilityTags.HasTagExact(Tag))
		{
			AbilitySpecInputPressed(AbilitySpec);
			if (!AbilitySpec.IsActive())
			{
				TryActivateAbility(AbilitySpec.Handle);
			}
		}
	}
}

void UCharacterAbilitySystemComponent::AbilityInputTagReleased(const FGameplayTag& InputTag)
{
	if (!InputTag.IsValid())return;

	FScopedAbilityListLock ActiveScopLock(*this);
	for (auto& AbilitySpec : GetActivatableAbilities())
	{
		FGameplayTag Tag = InputTag;
		if (InputTag == FDungeonGameplayTags::Get().Input_Space)
		{
			Tag = FDungeonGameplayTags::Get().Ability_Run;
		}

		if (AbilitySpec.DynamicAbilityTags.HasTagExact(Tag) && AbilitySpec.IsActive())
		{			
			AbilitySpecInputReleased(AbilitySpec);
			InvokeReplicatedEvent(EAbilityGenericReplicatedEvent::InputReleased, AbilitySpec.Handle, AbilitySpec.ActivationInfo.GetActivationPredictionKey());
		}

		if (InputTag == FDungeonGameplayTags::Get().Input_Space && AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag) && HeldTime < 0.5f)
		{
			if(!AbilitySpec.IsActive())
			{
				TryActivateAbility(AbilitySpec.Handle);
			}
		}
	}
	HeldTime = 0.f;
}

void UCharacterAbilitySystemComponent::RmoveAbilityByInputTag(const FGameplayTag& InputTag)
{
	if (!InputTag.IsValid())return;

	FScopedAbilityListLock ActiveScopLock(*this);
	{
		for (auto& AbilitySpec : GetActivatableAbilities())
		{
			if (AbilitySpec.DynamicAbilityTags.HasTag(InputTag))
			{
				ClearAbility(AbilitySpec.Handle);
				return;
			}
		}
	}
}

void UCharacterAbilitySystemComponent::UpgradeAttributeByTag(const FGameplayTag& AttributeTag, bool InBool)
{
	FGameplayEventData Payload;
	Payload.EventTag = AttributeTag;
	if (InBool)
	{
		Payload.EventMagnitude = 1.f;
	}
	else
	{
		Payload.EventMagnitude = -1.f;
	}
	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(GetAvatarActor(), AttributeTag, Payload);
}

FGameplayTag UCharacterAbilitySystemComponent::GetInputTagByAbilityClass(TSubclassOf<UGameplayAbility> InClass)
{
	UBaseGameplayAbility* Ability = Cast<UBaseGameplayAbility>(InClass->GetDefaultObject());
	
	if (Ability->StartupInputTag.IsValid())
	{
		return Ability->StartupInputTag;
	}
	
	return FGameplayTag();
}

void UCharacterAbilitySystemComponent::BroadcastDeactivateAccessoryAbilityDelegate(const FGameplayTag& InTag)
{
	DeactivateAccessoryAbility.Broadcast(InTag);
}