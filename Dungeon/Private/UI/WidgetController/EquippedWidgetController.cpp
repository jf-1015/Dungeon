// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgetController/EquippedWidgetController.h"
#include "Data/DungeonItemInfo.h"//
#include "ControlledPlayerState.h"//

void UEquippedWidgetController::BroadcastInitialValues()
{
}

void UEquippedWidgetController::BindCallBacksToDependencies()
{
	FDungeonGameplayTags Tags = FDungeonGameplayTags::Get();
	AControlledPlayerState* ControlledPS = Cast<AControlledPlayerState>(PlayerState);

	ControlledPS->UpdateBackpack.AddLambda
	([this, Tags](TArray<FGameItemInfo>& StoredItemInfomation)
		{
			int32 Count = 1;
			bool IsSomethingEquipped = false;

			for (FGameItemInfo& Info : StoredItemInfomation)
			{
				if (Info.IsEquiped)
				{
					EquippedItemInfo.Broadcast(Info);
					IsSomethingEquipped = true;
				}

				for (FGameplayTag InTag : SelectedTagContainer)
				{
					if (Info.HandheldType.MatchesTag(InTag))
					{
						WaitSelected.Broadcast(Info, Count++);
					}
				}

			}	

			if (Count == 1)
			{
				WaitSelected.Broadcast(FGameItemInfo(), 0);
			}

			if (!IsSomethingEquipped)
			{
				EquippedItemInfo.Broadcast(FGameItemInfo());
			}
		}
	);
}

void UEquippedWidgetController::OpenEquipmentPanel()
{
	AControlledPlayerState* ControlledPS = Cast<AControlledPlayerState>(PlayerState);
	ControlledPS->UpdateBackPackInfomation();
}

bool UEquippedWidgetController::Equip(int32 EquipSlotIndex,int32 BackpackIndex, FGameplayTagContainer HandheldTypes)
{
	AControlledPlayerState* ControlledPS = Cast<AControlledPlayerState>(PlayerState);
	return ControlledPS->Equip(EquipSlotIndex, BackpackIndex, HandheldTypes);
}

FGameItemInfo UEquippedWidgetController::GetEquipedWeaponInfo()
{
	AControlledPlayerState* ControlledPS = Cast<AControlledPlayerState>(PlayerState);
	return ControlledPS->GetCurrentWeaponInfo();
}

FGameItemInfo UEquippedWidgetController::GetEquipedSecondaryWeaponInfo()
{
	AControlledPlayerState* ControlledPS = Cast<AControlledPlayerState>(PlayerState);
	return ControlledPS->GetCurrentSecondaryWeaponInfo();
}

FGameItemInfo UEquippedWidgetController::GetEquipedConsumableInfo()
{
	AControlledPlayerState* ControlledPS = Cast<AControlledPlayerState>(PlayerState);
	return ControlledPS->GetCurrentConsumableInfo();
}

bool UEquippedWidgetController::CheckIsEmptyEquipmentSlot(int32 EquipSlotIndex, FGameplayTagContainer HandheldTypes)
{
	AControlledPlayerState* ControlledPS = Cast<AControlledPlayerState>(PlayerState);

	for (FGameplayTag Tag : HandheldTypes)
	{
		if (ControlledPS->CheckIsEmptyEquipmentSlot(EquipSlotIndex, Tag))
		{
			return true;
		}
	}
	return false;
}

int32 UEquippedWidgetController::GetNumByType(FGameplayTagContainer InContainer)
{
	AControlledPlayerState* ControlledPS = Cast<AControlledPlayerState>(PlayerState);
	return ControlledPS->GetNumByType(InContainer);
}

bool UEquippedWidgetController::GetPointerVisibility(int32 SlotIndex, FGameplayTagContainer InContainer)
{
	FDungeonGameplayTags Tags = FDungeonGameplayTags::Get();
	AControlledPlayerState* ControlledPS = Cast<AControlledPlayerState>(PlayerState);

	if (InContainer.HasTag(Tags.Handheld_1H) || InContainer.HasTag(Tags.Handheld_2H))
	{
		if (SlotIndex == ControlledPS->GetCurrentEquippedWeaponSlot())
		{
			return true;
		}
	}
	else if (InContainer.HasTag(Tags.Handheld_Secondary))
	{
		if (SlotIndex == ControlledPS->GetCurrentEquippedSecondaryWeaponSlot())
		{
			return true;
		}
	}
	else if (InContainer.HasTag(Tags.Handheld_None))
	{
		if (SlotIndex == ControlledPS->GetCurrentEquippedConsumableSlot())
		{
			return true;
		}
	}
	return false;
}

void UEquippedWidgetController::SetSelectedContainer(FGameplayTagContainer InContainer)
{
	SelectedTagContainer = InContainer;
}