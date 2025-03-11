// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ViewModel/Mvvm_LoadScreen.h"
#include "UI/ViewModel/Mvvm_LoadSlot.h"//
#include "GameMode/GlobalGameModeBase.h"//
#include "Kismet/GameplayStatics.h"//
#include "DungeonGameInstance.h"//

void UMvvm_LoadScreen::InitializeLoadSlots()
{
	LoadSlot_0 = NewObject<UMvvm_LoadSlot>(this, LoadSlotClass);
	LoadSlot_0->SetLoadSlotName(FString("LoadSlot_0"));
	LoadSlot_0->SlotIndex = 0;
	LoadSlots.Add(0, LoadSlot_0);

	LoadSlot_1 = NewObject<UMvvm_LoadSlot>(this, LoadSlotClass);
	LoadSlot_1->SetLoadSlotName(FString("LoadSlot_1"));
	LoadSlot_1->SlotIndex = 1;
	LoadSlots.Add(1, LoadSlot_1);

	LoadSlot_2 = NewObject<UMvvm_LoadSlot>(this, LoadSlotClass);
	LoadSlot_2->SetLoadSlotName(FString("LoadSlot_2"));
	LoadSlot_2->SlotIndex = 2;
	LoadSlots.Add(2, LoadSlot_2);

	SetLoadSlotNum(LoadSlots.Num());
}

UMvvm_LoadSlot* UMvvm_LoadScreen::GetLoadSoltByIndex(int32 InIndex)
{
	return LoadSlots.FindChecked(InIndex);
}

void UMvvm_LoadScreen::NewGameButtonPressed(int32 InIndex)
{
	LoadSlots[InIndex]->SetSwitcherIndex.Broadcast(1);
}

void UMvvm_LoadScreen::NewSlotButtonPressed(int32 InIndex, const FString EnteredName)
{
	AGlobalGameModeBase* GM = Cast<AGlobalGameModeBase>(UGameplayStatics::GetGameMode(this));
	if (!IsValid(GM))return;

	LoadSlots[InIndex]->SetMapName(GM->DefaultMapName);
	LoadSlots[InIndex]->SetPlayerName(EnteredName);
	LoadSlots[InIndex]->SlotStatus = ELoadSlotStatus::Taken;
	LoadSlots[InIndex]->MapAssetName = GM->DefaultMap.ToSoftObjectPath().GetAssetName();

	LoadSlots[InIndex]->InitializeSlot();

	GM->SaveLoadSlotData(LoadSlots[InIndex], InIndex);

	UDungeonGameInstance* GI = Cast<UDungeonGameInstance>(GM->GetGameInstance());

	GI->LoadSlotName = LoadSlots[InIndex]->GetLoadSlotName();
	GI->LoadSlotIndex = InIndex;
}

void UMvvm_LoadScreen::SelectSlotButtonPressed(int32 InIndex)
{
	OnSlotSelected.Broadcast();

	for (TTuple<int32, UMvvm_LoadSlot*>Pair : LoadSlots)
	{
		if (Pair.Key == InIndex)
		{
			Pair.Value->EnableSelectButton.Broadcast(false);
		}
		else
		{
			Pair.Value->EnableSelectButton.Broadcast(true);
		}
	}
	SelectedSlot = LoadSlots[InIndex];
}

void UMvvm_LoadScreen::DeleteButtonPressed()
{
	if (IsValid(SelectedSlot))
	{
		AGlobalGameModeBase::DeleteSlot(SelectedSlot->GetLoadSlotName(), SelectedSlot->SlotIndex);

		SelectedSlot->SlotStatus = ELoadSlotStatus::Vacant;
		SelectedSlot->InitializeSlot();

		SelectedSlot->EnableSelectButton.Broadcast(true);
	}
}

void UMvvm_LoadScreen::PlayButtonPressed()
{
	AGlobalGameModeBase* GM = Cast<AGlobalGameModeBase>(UGameplayStatics::GetGameMode(this));
	UDungeonGameInstance* GI = Cast<UDungeonGameInstance>(GM->GetGameInstance());

	GI->LoadSlotName = SelectedSlot->GetLoadSlotName();
	GI->LoadSlotIndex = SelectedSlot->SlotIndex;

	if(IsValid(SelectedSlot))
	{
		GM->TravelToMap(SelectedSlot);
	}
}

void UMvvm_LoadScreen::LoadData()
{
	AGlobalGameModeBase* GM = Cast<AGlobalGameModeBase>(UGameplayStatics::GetGameMode(this));
	if (!IsValid(GM))return;

	for (TTuple<int32, UMvvm_LoadSlot*>Pair : LoadSlots)
	{
		UDungeonSaveGame* SaveGame = GM->GetLoadSlotSaveData(Pair.Value->GetLoadSlotName(), Pair.Key);

		Pair.Value->SetMapName(SaveGame->MapName);
		Pair.Value->SetPlayerName(SaveGame->PlayerName);
		Pair.Value->SlotStatus = SaveGame->SlotStatus;
		Pair.Value->MapAssetName = SaveGame->MapAssetName;

		Pair.Value->InitializeSlot();
	}
}

void UMvvm_LoadScreen::SetLoadSlotNum(int32 InNum)
{
	UE_MVVM_SET_PROPERTY_VALUE(LoadSlotNum, InNum);
}