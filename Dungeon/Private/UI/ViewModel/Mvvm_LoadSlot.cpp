// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ViewModel/Mvvm_LoadSlot.h"

void UMvvm_LoadSlot::InitializeSlot()
{
	int32 SwitcherIndex = SlotStatus.GetValue();
	SetSwitcherIndex.Broadcast(SwitcherIndex);
}

void UMvvm_LoadSlot::SetLoadSlotName(FString InSlotName)
{
	UE_MVVM_SET_PROPERTY_VALUE(LoadSlotName, InSlotName);
}

void UMvvm_LoadSlot::SetMapName(FString InMapName)
{
	UE_MVVM_SET_PROPERTY_VALUE(MapName, InMapName);
}

void UMvvm_LoadSlot::SetPlayerName(FString InPlayerName)
{
	UE_MVVM_SET_PROPERTY_VALUE(PlayerName, InPlayerName);
}