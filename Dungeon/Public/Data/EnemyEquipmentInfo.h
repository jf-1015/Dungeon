// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "EnemyEquipmentInfo.generated.h"

class UGameplayAbility;

USTRUCT(BlueprintType)
struct FEnemyWeaponInfo
{
	GENERATED_BODY();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 WeaponItemIndex = -1;

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
class DUNGEON_API UEnemyEquipmentInfo : public UDataAsset
{
	GENERATED_BODY()
	
public:

	UFUNCTION(BlueprintCallable)
	FEnemyWeaponInfo GetEnemyWeaponInfoForItemIndex(int32 InIndex);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FEnemyWeaponInfo>EnemyWeaponInfomation;

};