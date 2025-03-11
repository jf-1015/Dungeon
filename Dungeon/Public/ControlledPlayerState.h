// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "AbilitySystemInterface.h"//
#include "Data/DungeonItemInfo.h"//
#include "GameplayTagContainer.h"//
#include "GameplayEffectTypes.h"//
#include "ControlledPlayerState.generated.h"

class UAttributeSet;
class UAbilitySystemComponent;
struct FGameplayTag;

class USplineComponent;

class UDungeonSaveGame;

DECLARE_MULTICAST_DELEGATE_OneParam(FUpdateBackpack, TArray<FGameItemInfo>&);

/**
 * 
 */
UCLASS()
class DUNGEON_API AControlledPlayerState : public APlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()
	
public:

	AControlledPlayerState();

	virtual UAbilitySystemComponent* GetAbilitySystemComponent()const override;
	UAttributeSet* GetAttributeSet()const { return PlayerAS; }

	/*Backpack Aux*/
	UFUNCTION()
	void AddToBackpack(FGameItemInfo& InInfo);
	
	void Merge(TArray<FGameItemInfo>& TargetArray, int32 L, int32 M, int32 R, TArray<FGameItemInfo>& AuxArray);
	void MergeSort(TArray<FGameItemInfo>& TargetArray, int32 L, int32 R, TArray<FGameItemInfo>& AuxArray)
	{
		if (L < R)
		{
			int32 M = L + (R - L) / 2;
			MergeSort(TargetArray, L, M, AuxArray);
			MergeSort(TargetArray, M + 1, R, AuxArray);
			Merge(TargetArray, L, M, R, AuxArray);
		}
		if (!TargetArray.IsEmpty())
		{
			TargetArray[0].BackpackIndex = 1;
		}
	}

	FUpdateBackpack UpdateBackpack;

	void Discard(int32 InIndex, int32 Num = 1);
	void UseBackpackConsumable(int32 InIndex);//BackpackIndex
	void UseCurrentConsumable();//CurrentConsumableSlot
	bool IsCurrentConsumableEmpty();
	void HandleUse(FGameItemInfo& InInfo);

	void UpdateBackPackInfomation();

	bool CheckIsEmptyCellByBackpackIndex(int32 InIndex);
	bool CheckIsEmptyCellByConsumeIndex(int32 InIndex);
	bool CheckIsEmptyCellByWeaponIndex(int32 InIndex);
	bool CheckIsEmptyCellByAccessoryIndex(int32 InIndex);

	bool CheckBackpackIsFull();

	bool CheckIsEmptyCellByItemTypes(int32 InIndex, FGameplayTagContainer InContainer);
	/*Backpack Aux*/

	/*Equip Aux*/
	bool Equip(int32 EquipSlotIndex, int32 BackpackIndex, FGameplayTagContainer HandheldTypes);
	void ClearEquipmentSlot(int32 EquipSlotIndex, FGameplayTagContainer HandheldTypes);

	bool IsEquippedInThisSlotAlready(int32 EquipSlotIndex, int32 BackpackIndex, FGameplayTagContainer HandheldTypes);
	bool CheckIsEmptyEquipmentSlot(int32 EquipSlotIndex, const FGameplayTag& HandheldType);

	FGameItemInfo GetCurrentWeaponInfo();
	FGameItemInfo GetCurrentSecondaryWeaponInfo();
	FGameItemInfo GetCurrentConsumableInfo();

	int32 GetCurrentEquippedWeaponSlot();
	int32 GetCurrentEquippedSecondaryWeaponSlot();
	int32 GetCurrentEquippedConsumableSlot();
	void SetCurrentEquippedWeaponSlot(int32 InSlot);
	void SetCurrentEquippedSecondaryWeaponSlot(int32 InSlot);
	void SetCurrentEquippedConsumableSlot(int32 InSlot);

	int32 GetEmptyAccessorySlot();
	bool CheckHelmet();
	void SetAccessorySlotStatus(int32 InSlot, bool InBool);

	int32 GetNumByType(FGameplayTagContainer InContainer);

	void ChangeCurrentEquippedWeaponSlot(int32 InSlot);
	void ChangeCurrentEquippedSecondaryWeaponSlot(int32 InSlot);
	void ChangeCurrentEquippedConsumableSlot(int32 InSlot);

	void GiveItemAbility(const FGameItemInfo& InInfo, const FGameplayTag& InputTag);

	void EquipAccessory(const FGameItemInfo& InInfo);
	void RemoveAccessory(int32 InSlot, const FGameplayTag& InTag);
	void RemoveAccessoryEffect();

	void SetDurability(float InNum, bool bSecondary);
	float GetDurability(bool bSecondary);
	/*Equip Aux*/

	/*Range Aux*/
	bool CanLoad();
	bool GetIsLoaded();
	void SetIsLoaded(bool InBool);
	/*Range Aux*/

	/*Coin Aux*/
	void CurseOfGoldCoins(FGameItemInfo& InInfo, int32 NumToChange = 0);
	/*Coin Aux*/

	/*Expend Aux*/
	bool CheckCanAfford(int32 ItemIndex, int32 RequiredNum);
	void ExpendItem(int32 ItemIndex, int32 Num);
	/*Expend Aux*/

	/*Save*/
	void SaveItemsToSaveGame(UDungeonSaveGame* SaveData);
	/*Save*/

	void SpawnItemToWorld(FGameItemInfo& Info, int32 Num = 1);

protected:

	TMap<int32, FActiveGameplayEffectHandle>SlotToAccessoryEffects;

	TMap<int32, FGameplayTag>IndexToAttributeTag;

	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent>PlayerASC;

	UPROPERTY()
	TObjectPtr<UAttributeSet>PlayerAS;

	UPROPERTY(BlueprintReadOnly)
	TArray<FGameItemInfo> StoredItemInfomation;

	UPROPERTY(BlueprintReadOnly)
	int32 CurrentEquippedWeaponSlot = 1;

	UPROPERTY(BlueprintReadOnly)
	int32 CurrentEquippedSecondaryWeaponSlot = 1;

	UPROPERTY(BlueprintReadOnly)
	int32 CurrentEquippedConsumableSlot = 1;

	int32 NumOfCoins = 0;

	bool IsAccessorySlotEmpty_1 = true;
	bool IsAccessorySlotEmpty_2 = true;
	bool IsAccessorySlotEmpty_3 = true;
};