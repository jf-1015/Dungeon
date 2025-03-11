// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Ability/BaseDamageGameplayAbility.h"
#include "MeleeGameplayAbility.generated.h"

/**
 * 
 */
UCLASS()
class DUNGEON_API UMeleeGameplayAbility : public UBaseDamageGameplayAbility
{
	GENERATED_BODY()

protected:

	/*Weapon Trace Checking*/
	UFUNCTION(BlueprintCallable)
	void TraceCheckTimerFunction(
		TArray<AActor*>& Outs,
		UPARAM(ref)TArray<AActor*>& TraceLineActors,
		const TArray<AActor*>& ActorsToIgnore,
		float InRaidus,
		UPARAM(ref) UStaticMeshComponent* InWeaponComponent);

	UFUNCTION(BlueprintCallable)
	void PreTraceCheck(UPARAM(ref) UStaticMeshComponent* InWeaponComponent);

	UFUNCTION(BlueprintCallable)
	void PrepareForNext(UPARAM(ref) UStaticMeshComponent* InWeaponComponent);

	UFUNCTION(BlueprintCallable)
	void ApplyEffectInOneRound(AActor* HitActor);

	UFUNCTION(BlueprintCallable)
	bool CheckIsOnFrontAndBlocking(AActor* InActor);

	UPROPERTY()
	TArray<FName> WeaponSocketNames;

	UPROPERTY()
	TMap<FName, FVector> NameToLocation;

	UPROPERTY()
	TArray<AActor*>HitActors;

	UPROPERTY(BlueprintReadWrite)
	bool bBlocked = false;

	FTimerHandle TraceTimerHandle;
	FTimerDelegate TraceCheckingTimerDelegate;
	/*Weapon Trace Checking*/

	/*ThrowCurrentWeapon*/
	UFUNCTION(BlueprintCallable)
	virtual void ThrowCurrentWeapon_1H();
	/*ThrowCurrentWeapon*/

	UFUNCTION(BlueprintCallable)
	void SetCost(FGameplayTag HandheldType);

	UPROPERTY(EditDefaultsOnly, Category = "Cost")
	TSubclassOf<UGameplayEffect>CostEffectClass1H;

	UPROPERTY(EditDefaultsOnly, Category = "Cost")
	TSubclassOf<UGameplayEffect>CostEffectClass2H;

	UFUNCTION(BlueprintCallable)
	UAnimMontage* GetWeaponHeavyAttackMontage();
};