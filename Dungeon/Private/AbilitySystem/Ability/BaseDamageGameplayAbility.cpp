// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Ability/BaseDamageGameplayAbility.h"
#include "AbilitySystemBlueprintLibrary.h"//
#include "AbilitySystemComponent.h"//
#include "AbilitySystem/DungeonAbilitySystemLibrary.h"//

FDamageEffectParams UBaseDamageGameplayAbility::MakeDamageEffectParams(AActor* TargetActor)
{
    FDamageEffectParams Params;

    Params.WorldContext = GetAvatarActorFromActorInfo();
    Params.SourceAbilitySystemComponent = GetAbilitySystemComponentFromActorInfo();
    Params.TargetAbilitySystemComponent = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
    Params.DamageEffectClass = DamageEffectClass;

    return Params;

    GetCostGameplayEffect();
}