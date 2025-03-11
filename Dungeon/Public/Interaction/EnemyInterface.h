// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Data/DungeonItemInfo.h"//
#include "EnemyInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UEnemyInterface : public UInterface
{
	GENERATED_BODY()
};

class APickupItems;

USTRUCT(BlueprintType)
struct FEquipmentAbilitiesAndMontages
{
	GENERATED_BODY();

	/*Melee*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<int32, UAnimMontage*>CountToMontageNone;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<int32, UAnimMontage*>CountToMontage1H;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAnimMontage* HeavyAttack1H = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<int32, UAnimMontage*>CountToMontage2H;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAnimMontage* HeavyAttack2H = nullptr;

	/*Range*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAnimMontage* ShootMontage = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAnimMontage* ShootMontage_2H = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAnimMontage* ReloadMontage = nullptr;

	/*Magic*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAnimMontage* SpellcastMontage = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAnimMontage* Spellcast_LongMontage = nullptr;
};

/**
 * 
 */
class DUNGEON_API IEnemyInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	/*SetHandheldItem*/
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void SetEnemyHandheldItem(const FGameItemInfo& InInfo, bool IsSecondary);

	/*Find Item For Behavior Tree*/
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	APickupItems* FindNearestWeapon(float Radius);
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	APickupItems* FindNearestConsumable(float Radius, int32 ItemIndex);

	/*GetMontage*/
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	TMap<int32, UAnimMontage*>GetCountToMontageByHandheldType();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	UAnimMontage* GetShootMontage(bool InBool);
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	UAnimMontage* GetReloadMontage();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	UAnimMontage* GetPickupMontage();

	/*Range Weapon Aux*/
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	bool GetIsLoaded();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void SetIsLoaded(bool InBool);
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	bool CanLoad();

	/*Awake Aux*/
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void SetIsAwake(bool InBool);
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	bool GetIsAwake();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	UAnimMontage* GetAwakeMontage(int32 InIndex);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	bool GetInactiveState();

	/*Ammunition*/
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	bool CheckAmmunitionIsEnough();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void SetAmountOfAmmunition(int32 InNum);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void DicardCurrentWeapon(bool bSecondary);

	/*Handle HandheldInfo*/
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void SetWeaponInfo(const FGameItemInfo& InInfo);
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	FGameItemInfo GetWeaponInfo();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void SetSecondaryWeaponInfo(const FGameItemInfo& InInfo);
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	FGameItemInfo GetSecondaryWeaponInfo();
};