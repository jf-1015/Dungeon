// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/DungeonAbilitySystemGlobals.h"
#include "AbilitySystem/DungeonAbilityTypes.h"//

FGameplayEffectContext* UDungeonAbilitySystemGlobals::AllocGameplayEffectContext() const
{
	return new FDungeonGameplayEffectContext();
}