// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Ability/BaseDamageGameplayAbility.h"
#include "RangeGameplayAbility.generated.h"

class ADungeonProjectile;
class UGameplayEffect;
struct FGameplayTag;

/**
 * 
 */
UCLASS()
class DUNGEON_API URangeGameplayAbility : public UBaseDamageGameplayAbility
{
	GENERATED_BODY()

protected:

	UFUNCTION(BlueprintCallable, Category = "Projectile")
	virtual void SpawnProjectile(bool IsAiming = false, bool IsLockTarget = false, bool bOverridePitch = false, float PitchOverride = 0.f, bool InIsArrow = false);

	UFUNCTION(BlueprintCallable, Category = "Projectile")
	virtual void SpawnActor_Throw();

	bool HandleTransform(FTransform& InTransform, bool IsAiming, bool IsLockTarget, bool bOverridePitch, float PitchOverride, bool InIsArrow);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<ADungeonProjectile>ProjectileClass;
};