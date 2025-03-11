// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "CharacterAbilitySystemComponent.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FDeactivateAccessoryAbility, const FGameplayTag&/*InpuTag*/);

/**
 * 
 */
UCLASS()
class DUNGEON_API UCharacterAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:

	/*Delegate*/
	FDeactivateAccessoryAbility DeactivateAccessoryAbility;
	/*Delegate*/

	void AddCharacterAbilities(const TArray<TSubclassOf<UGameplayAbility>>& Abilities);
	void AddCharacterPassiveAbilities(const TArray<TSubclassOf<UGameplayAbility>>& PassiveAbilities);

	void AbilityInputTagPressed(const FGameplayTag& InputTag);
	void AbilityInputTagHeld(const FGameplayTag& InputTag);
	void AbilityInputTagReleased(const FGameplayTag& InputTag);

	void RmoveAbilityByInputTag(const FGameplayTag& InputTag);

	void UpgradeAttributeByTag(const FGameplayTag& AttributeTag, bool InBool);

	static FGameplayTag GetInputTagByAbilityClass(TSubclassOf<UGameplayAbility> InClass);
	
	void BroadcastDeactivateAccessoryAbilityDelegate(const FGameplayTag& InTag);

	float HeldTime = 0.f;
};