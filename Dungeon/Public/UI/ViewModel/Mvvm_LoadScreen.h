// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MVVMViewModelBase.h"
#include "Mvvm_LoadScreen.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnSlotSelected);

class UMvvm_LoadSlot;

/**
 * 
 */
UCLASS()
class DUNGEON_API UMvvm_LoadScreen : public UMVVMViewModelBase
{
	GENERATED_BODY()

public:

	void InitializeLoadSlots();

	UPROPERTY(BlueprintAssignable)
	FOnSlotSelected OnSlotSelected;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UMvvm_LoadSlot>LoadSlotClass;

	UFUNCTION(BlueprintPure)
	UMvvm_LoadSlot* GetLoadSoltByIndex(int32 InIndex);

	/*Button*/
	UFUNCTION(BlueprintCallable)
	void NewGameButtonPressed(int32 InIndex);

	UFUNCTION(BlueprintCallable)
	void NewSlotButtonPressed(int32 InIndex, const FString EnteredName);

	UFUNCTION(BlueprintCallable)
	void SelectSlotButtonPressed(int32 InIndex);

	UFUNCTION(BlueprintCallable)
	void DeleteButtonPressed();

	UFUNCTION(BlueprintCallable)
	void PlayButtonPressed();
	/*Button*/

	void LoadData();

	void SetLoadSlotNum(int32 InNum);
	int32 GetLoadSlotNum()const { return LoadSlotNum; }

private:

	UPROPERTY()
	TMap<int32, UMvvm_LoadSlot*>LoadSlots;

	UPROPERTY()
	TObjectPtr<UMvvm_LoadSlot>LoadSlot_0;

	UPROPERTY()
	TObjectPtr<UMvvm_LoadSlot>LoadSlot_1;

	UPROPERTY()
	TObjectPtr<UMvvm_LoadSlot>LoadSlot_2;

	UPROPERTY()
	TObjectPtr<UMvvm_LoadSlot>SelectedSlot;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, FieldNotify, Setter, Getter, meta = (AllowPrivateAccess = "true"))
	int32 LoadSlotNum;
};