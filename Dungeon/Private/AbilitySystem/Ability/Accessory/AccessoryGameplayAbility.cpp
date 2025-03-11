// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Ability/Accessory/AccessoryGameplayAbility.h"
#include "AbilitySystemBlueprintLibrary.h"//
#include "AbilitySystem/CharacterAbilitySystemComponent.h"//

void UAccessoryGameplayAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (UCharacterAbilitySystemComponent* ASC = Cast<UCharacterAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetAvatarActorFromActorInfo())))
	{
		ASC->DeactivateAccessoryAbility.AddUObject(this, &UAccessoryGameplayAbility::ReceiveDeactivate);
	}
}

void UAccessoryGameplayAbility::ReceiveDeactivate(const FGameplayTag& InputTag)
{
	if (StartupInputTag.MatchesTag(InputTag))
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
	}
}