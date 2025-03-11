// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Ability/BaseGameplayAbility.h"
#include "AbilitySystemComponent.h"//
#include "AbilitySystemBlueprintLibrary.h"//
#include "DungeonGameplayTags.h"//
#include "Interaction/CharacterInterface.h"//
#include "Interaction/PlayerInterface.h"//
#include "GameFramework/CharacterMovementComponent.h"//

void UBaseGameplayAbility::SetFoodTimer()
{
	GetWorld()->GetTimerManager().SetTimer(FoodTimerHandle, this, &UBaseGameplayAbility::SustainLife, 0.1f, true);
}

void UBaseGameplayAbility::SustainLife()
{
	FDungeonGameplayTags Tags = FDungeonGameplayTags::Get();
	UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo();

	FGameplayEffectContextHandle ContextHandle = ASC->MakeEffectContext();
	FGameplayEffectSpecHandle SpecHandle = ASC->MakeOutgoingSpec(SustainLifeEffect, 1.f, ContextHandle);

	float FoodConsumeRate = IPlayerInterface::Execute_GetFoodConsumeRate(GetAvatarActorFromActorInfo());
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, Tags.Attribute_Vital_Food, -FoodConsumeRate);
	ASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data);
}

void UBaseGameplayAbility::SetRunTimer()
{
	GetWorld()->GetTimerManager().SetTimer(RunTimerHandle, this, &UBaseGameplayAbility::ConsumeEnergy, 0.1f, true);
}

void UBaseGameplayAbility::ConsumeEnergy()
{
	FDungeonGameplayTags Tags = FDungeonGameplayTags::Get();
	UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo();

	bool IsFalling = ICharacterInterface::Execute_GetIsFalling(GetAvatarActorFromActorInfo());
	if (!ASC->HasMatchingGameplayTag(FDungeonGameplayTags::Get().Ability_Run) || IsFalling)
	{
		return;
	}

	FGameplayEffectContextHandle ContextHandle = ASC->MakeEffectContext();
	FGameplayEffectSpecHandle SpecHandle = ASC->MakeOutgoingSpec(EnergyEffect, 1.f, ContextHandle);

	float Speed = IPlayerInterface::Execute_GetCharacterMovementComponent(GetAvatarActorFromActorInfo())->Velocity.Length();

	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, Tags.Attribute_Vital_Energy, -Speed * 0.01);
	ASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data);
}

void UBaseGameplayAbility::SetEnergyRecoverTimer()
{
	GetWorld()->GetTimerManager().SetTimer(EnergyRecoverTimer, this, &UBaseGameplayAbility::RecoverEnergy, 0.1f, true);
}

void UBaseGameplayAbility::RecoverEnergy()
{
	FDungeonGameplayTags Tags = FDungeonGameplayTags::Get();
	UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo();

	FGameplayTagContainer TagContainer;
	TagContainer.AddTag(Tags.Ability_Execution_Melee);
	TagContainer.AddTag(Tags.Ability_Execution_Range);
	TagContainer.AddTag(Tags.Ability_Dodge);
	TagContainer.AddTag(Tags.Ability_Jump);
	TagContainer.AddTag(Tags.Ability_Run);
	TagContainer.AddTag(Tags.Ability_HitReact);
	TagContainer.AddTag(Tags.Ability_Interrupt);

	bool IsFalling = ICharacterInterface::Execute_GetIsFalling(GetAvatarActorFromActorInfo());
	bool IsFoodEmpty = IPlayerInterface::Execute_IsFoodEmpty(GetAvatarActorFromActorInfo());

	float EnergyRecoverRate = 3.f;
	float HealthConsumeRate = 0.f;

	if (IsFoodEmpty)
	{
		EnergyRecoverRate = 1.f;
		HealthConsumeRate = 0.5f;
	}

	if (ASC->HasAnyMatchingGameplayTags(TagContainer) || IsFalling)
	{
		EnergyRecoverRate = 0.f;
	}

	FGameplayEffectContextHandle ContextHandle = ASC->MakeEffectContext();
	FGameplayEffectSpecHandle SpecHandle = ASC->MakeOutgoingSpec(EnergyEffect, 1.f, ContextHandle);

	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, Tags.Attribute_Vital_Energy, EnergyRecoverRate);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, Tags.Attribute_Vital_Health, HealthConsumeRate);
	ASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data);
}

void UBaseGameplayAbility::SetResilienceTimer()
{
	GetWorld()->GetTimerManager().SetTimer(ResilienceRecoverHandle, this, &UBaseGameplayAbility::RecoverResilience, 0.1f, true);
}

void UBaseGameplayAbility::RecoverResilience()
{
	FDungeonGameplayTags Tags = FDungeonGameplayTags::Get();
	UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo();

	if (ASC->HasMatchingGameplayTag(Tags.Hurt))
	{
		return;
	}

	FGameplayEffectContextHandle ContextHandle = ASC->MakeEffectContext();
	FGameplayEffectSpecHandle SpecHandle = ASC->MakeOutgoingSpec(ResilienceEffect, 1.f, ContextHandle);
	ASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data);
}