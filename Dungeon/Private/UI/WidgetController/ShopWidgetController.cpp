// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgetController/ShopWidgetController.h"
#include "ControlledPlayerState.h"//
#include "Character/NPCBase.h"

void UShopWidgetController::SetShoper(AActor* InActor)
{
	Shoper = InActor;
}

void UShopWidgetController::BroadcastInitialValues()
{
	ANPCBase* NPC = Cast<ANPCBase>(Shoper);
	if (NPC != nullptr)
	{
		for (const FItemsToShop& Info : NPC->ItemsToSell)
		{
			UpdateWidgetShop.Broadcast(Info);
		}
	}
}

void UShopWidgetController::BindCallBacksToDependencies()
{
	ANPCBase* NPC = Cast<ANPCBase>(Shoper);
	if (NPC != nullptr)
	{
		NPC->UpdateShop.AddLambda(
			[this](TArray<FItemsToShop>& ItemsToSell)
			{
				for (const FItemsToShop& Info : ItemsToSell)
				{
					UpdateWidgetShop.Broadcast(Info);
				}
			}
		);
	}
}

void UShopWidgetController::SetItemNumBySlotIndex(int32 Slot, int32 Num)
{
	ANPCBase* NPC = Cast<ANPCBase>(Shoper);
	if (NPC != nullptr)
	{
		NPC->SetItemNumBySlotIndex(Slot, Num);
	}
}

int32 UShopWidgetController::GetItemNumBySlotIndex(int32 Index)
{
	ANPCBase* NPC = Cast<ANPCBase>(Shoper);
	if (NPC != nullptr)
	{
		return NPC->GetItemNumBySlotIndex(Index);
	}
	return -1;
}

bool UShopWidgetController::CheckCanAfford(int32 ItemIndex, int32 RequiredNum)
{
	return Cast<AControlledPlayerState>(PlayerState)->CheckCanAfford(ItemIndex, RequiredNum);	
}

void UShopWidgetController::Buy(int32 ItemIndex, int32 RequiredNum, int32 SlotIndex, int32 NumPurchased)
{
	AControlledPlayerState* PS = Cast<AControlledPlayerState>(PlayerState);
	ANPCBase* NPC = Cast<ANPCBase>(Shoper);

	PS->ExpendItem(ItemIndex, RequiredNum);
	NPC->SetItemNumBySlotIndex(SlotIndex, NumPurchased);
}