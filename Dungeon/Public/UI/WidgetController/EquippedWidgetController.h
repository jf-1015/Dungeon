// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetController/PlayerWidgetController.h"
#include "GameplayTagContainer.h"//
#include "EquippedWidgetController.generated.h"

struct FGameItemInfo;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FEquippedItemInfo, const FGameItemInfo&, ItemInfo);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FWaitingSelected, const FGameItemInfo&, ItemInfo, int32, Index);

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class DUNGEON_API UEquippedWidgetController : public UPlayerWidgetController
{
	GENERATED_BODY()

public:

	virtual void BroadcastInitialValues()override;
	virtual void BindCallBacksToDependencies()override;

	UPROPERTY(BlueprintAssignable)
	FEquippedItemInfo EquippedItemInfo;

	UPROPERTY(BlueprintAssignable)
	FWaitingSelected WaitSelected;
	
	UFUNCTION(BlueprintCallable)
	void OpenEquipmentPanel();

	UFUNCTION(BlueprintCallable)
	bool Equip(int32 EquipSlotIndex, int32 BackpackIndex, FGameplayTagContainer HandheldTypes);

	UFUNCTION(BlueprintCallable)
	FGameItemInfo GetEquipedWeaponInfo();
	UFUNCTION(BlueprintCallable)
	FGameItemInfo GetEquipedSecondaryWeaponInfo();
	UFUNCTION(BlueprintCallable)
	FGameItemInfo GetEquipedConsumableInfo();

	UFUNCTION(BlueprintCallable)
	bool CheckIsEmptyEquipmentSlot(int32 EquipSlotIndex, FGameplayTagContainer HandheldTypes);

	UFUNCTION(BlueprintPure)
	int32 GetNumByType(FGameplayTagContainer InContainer);

	UFUNCTION(BlueprintCallable)
	bool GetPointerVisibility(int32 SlotIndex, FGameplayTagContainer InContainer);


protected:

	UPROPERTY()
	FGameplayTagContainer SelectedTagContainer = FGameplayTagContainer();

	UFUNCTION(BlueprintCallable)
	void SetSelectedContainer(FGameplayTagContainer InContainer);
	
};