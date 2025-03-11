// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"
#include "DetailedEquipmemtInfo.generated.h"

class UGameplayAbility;

USTRUCT(BlueprintType)
struct FWeaponInfo
{
	GENERATED_BODY();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 WeaponItemIndex = -1;

	/*Montage*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<int32, UAnimMontage*>CountToMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAnimMontage* HeavyAttack = nullptr;
	/*Montage*/

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float BaseDamage = 0.f;

	/*Coefficient*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float PowerCoefficient = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DexterityCoefficient = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float IntelligenceCoefficient = 0.f;
	/*Coefficient*/

	/*Combat*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ResilienceCut = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AvoidanceRate = 0.f;
	/*Combat*/

	/*Ability*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UGameplayAbility>WeaponAbility;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UGameplayAbility>SpecialSkill;
	/*Ability*/

};

/**
 * 
 */
UCLASS()
class DUNGEON_API UDetailedEquipmemtInfo : public UDataAsset
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable)
	FWeaponInfo GetWeaponInfoForItemIndex(int32 InIndex);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FWeaponInfo>WeaponInfomation;
	
};