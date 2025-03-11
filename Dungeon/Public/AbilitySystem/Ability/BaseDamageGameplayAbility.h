// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Ability/BaseGameplayAbility.h"
#include "AbilitySystem/DungeonAbilityTypes.h"//
#include "BaseDamageGameplayAbility.generated.h"

/**
 * 
 */
UCLASS()
class DUNGEON_API UBaseDamageGameplayAbility : public UBaseGameplayAbility
{
	GENERATED_BODY()

public:

protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<UGameplayEffect>DamageEffectClass;

	UFUNCTION(BlueprintPure)
	FDamageEffectParams MakeDamageEffectParams(AActor* TargetActor = nullptr);
};