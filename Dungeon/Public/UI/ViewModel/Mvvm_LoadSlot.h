// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MVVMViewModelBase.h"
#include "SaveGame/DungeonSaveGame.h"//
#include "Mvvm_LoadSlot.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSetSwitcherIndex, int32, InIndex);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FEnableSelectButton, bool, bEnable);

/**
 * 
 */
UCLASS()
class DUNGEON_API UMvvm_LoadSlot : public UMVVMViewModelBase
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintAssignable)
	FSetSwitcherIndex SetSwitcherIndex;

	UPROPERTY(BlueprintAssignable)
	FEnableSelectButton EnableSelectButton;

	void InitializeSlot();

	/*Field Notifies*/
	void SetLoadSlotName(FString InSlotName);
	void SetMapName(FString InMapName);
	void SetPlayerName(FString InPlayerName);

	FString GetLoadSlotName()const { return LoadSlotName; }
	FString GetMapName()const { return MapName; }
	FString GetPlayerName()const { return PlayerName; }
	/*Field Notifies*/

	UPROPERTY()
	int32 SlotIndex;

	UPROPERTY()
	TEnumAsByte<ELoadSlotStatus>SlotStatus;

	UPROPERTY()
	FString MapAssetName;

	UPROPERTY()
	FName PlayerStartTag;

private:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, FieldNotify, Setter, Getter, meta = (AllowPrivateAccess = "true"))
	FString LoadSlotName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, FieldNotify, Setter, Getter, meta = (AllowPrivateAccess = "true"))
	FString MapName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, FieldNotify, Setter, Getter, meta = (AllowPrivateAccess = "true"))
	FString PlayerName;
};