// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "BaseGameplayAbility.generated.h"

class UGameplayEffect;

/**
 * 
 */
UCLASS()
class DUNGEON_API UBaseGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	FGameplayTag StartupInputTag;

	UPROPERTY(BlueprintReadOnly, Category = "Input")
	FGameplayTag AsyncInputTag;
	
protected:

	/*Food*/
	FTimerHandle FoodTimerHandle;

	UFUNCTION(BlueprintCallable)
	void SetFoodTimer();

	UPROPERTY(EditDefaultsOnly, Category = "Food")
	TSubclassOf<UGameplayEffect>SustainLifeEffect;

	void SustainLife();
	/*Food*/

	/*Energy*/
	FTimerHandle RunTimerHandle;
	FTimerHandle EnergyRecoverTimer;

	UFUNCTION(BlueprintCallable)
	void SetRunTimer();

	UFUNCTION(BlueprintCallable)
	void SetEnergyRecoverTimer();

	UPROPERTY(EditDefaultsOnly, Category = "Run")
	TSubclassOf<UGameplayEffect>EnergyEffect;

	void ConsumeEnergy();

	void RecoverEnergy();
	/*Energy*/

	/*Resilience*/
	FTimerHandle ResilienceRecoverHandle;

	UFUNCTION(BlueprintCallable)
	void SetResilienceTimer();

	UPROPERTY(EditDefaultsOnly, Category = "Resilience")
	TSubclassOf<UGameplayEffect>ResilienceEffect;

	void RecoverResilience();
	/*Resilience*/
};