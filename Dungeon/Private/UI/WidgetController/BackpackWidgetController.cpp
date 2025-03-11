// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgetController/BackpackWidgetController.h"
#include "ControlledPlayerState.h"//

void UBackpackWidgetController::BroadcastInitialValues()
{
}

void UBackpackWidgetController::BindCallBacksToDependencies()
{
	FDungeonGameplayTags Tags = FDungeonGameplayTags::Get();
	AControlledPlayerState* ControlledPS = Cast<AControlledPlayerState>(PlayerState);

	ControlledPS->UpdateBackpack.AddLambda
	([this, Tags](TArray<FGameItemInfo>& StoredItemInfomation)
		{
			int32 WeaponTypeCount = 1;
			int32 ConsumeTypeCount = 1;
			int32 AccessoryCount = 1;

			int32 ItemNum = StoredItemInfomation.Num();
		
			UpdateBackpackDelegate.Broadcast(FGameItemInfo());

			for (FGameItemInfo& Info : StoredItemInfomation)
			{
				for (FGameplayTag InTag : SelectedTagContainer)
				{
					if (Info.ItemType.MatchesTag(InTag))
					{					 
						if (Info.ItemType.MatchesTagExact(Tags.ItemType_Weapon))
						{
							Info.BackpackIndex_Type = WeaponTypeCount++;
						}
						else if (Info.ItemType.MatchesTagExact(Tags.ItemType_Consume))
						{
							Info.BackpackIndex_Type = ConsumeTypeCount++;
						}
						else if (Info.ItemType.MatchesTagExact(Tags.ItemType_Accessory))
						{
							Info.BackpackIndex_Type = AccessoryCount++;
						}
						UpdateBackpackDelegate.Broadcast(Info);
					}
				}
			}
		}
	);
}

void UBackpackWidgetController::OpenBackpack()
{
	AControlledPlayerState* ControlledPS = Cast<AControlledPlayerState>(PlayerState);
	ControlledPS->UpdateBackPackInfomation();
}

void UBackpackWidgetController::Discard(int32 InIndex, int32 Num)
{
	AControlledPlayerState* ControlledPS = Cast<AControlledPlayerState>(PlayerState);
	ControlledPS->Discard(InIndex, Num);
}

void UBackpackWidgetController::Use(int32 InIndex)
{
	AControlledPlayerState* ControlledPS = Cast<AControlledPlayerState>(PlayerState);
	ControlledPS->UseBackpackConsumable(InIndex);
}

bool UBackpackWidgetController::CheckIsEmptyCellByItemTypes(int32 InIndex, FGameplayTagContainer InContainer)
{
	AControlledPlayerState* ControlledPS = Cast<AControlledPlayerState>(PlayerState);
	return ControlledPS->CheckIsEmptyCellByItemTypes(InIndex, InContainer);
}

void UBackpackWidgetController::SetSelectedContainer(FGameplayTagContainer InContainer)
{
	SelectedTagContainer = InContainer;
}

FGameplayTagContainer UBackpackWidgetController::GetSelectedContainer()
{
	return SelectedTagContainer;
}