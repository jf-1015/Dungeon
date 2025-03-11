// Fill out your copyright notice in the Description page of Project Settings.


#include "ControlledPlayerState.h"
#include "AbilitySystem/CharacterAbilitySystemComponent.h"//
#include "AbilitySystem/CharacterAttributeSet.h"//
#include "DungeonGameplayTags.h"//
#include "AbilitySystem/DungeonAbilitySystemLibrary.h"//
#include "Interaction/CharacterInterface.h"//
#include "Interaction/PlayerInterface.h"//
#include "Data/DetailedEquipmemtInfo.h"//
#include "Data/ItemClass.h"//
#include "Kismet/GameplayStatics.h"//
#include "AbilitySystem/Ability/BaseGameplayAbility.h"//
#include "SaveGame/DungeonSaveGame.h"//

AControlledPlayerState::AControlledPlayerState()
{
	const FDungeonGameplayTags DungeonTags = FDungeonGameplayTags::Get();

	IndexToAttributeTag.Add(0, DungeonTags.Attribute_Base_Power);
	IndexToAttributeTag.Add(1, DungeonTags.Attribute_Base_Dexterity);
	IndexToAttributeTag.Add(2, DungeonTags.Attribute_Base_Intelligence);
	IndexToAttributeTag.Add(3, DungeonTags.Attribute_Base_Vitality);
	IndexToAttributeTag.Add(4, DungeonTags.Attribute_Base_Endurance);
	IndexToAttributeTag.Add(5, DungeonTags.Attribute_Base_Resilience);
	IndexToAttributeTag.Add(6, DungeonTags.Attribute_Base_Defence);

	PlayerASC = CreateDefaultSubobject<UCharacterAbilitySystemComponent>("CharacterASC");
	PlayerASC->SetIsReplicated(true);
	PlayerASC->SetReplicationMode(EGameplayEffectReplicationMode::Full);

	PlayerAS = CreateDefaultSubobject<UCharacterAttributeSet>("CharacterAS");
}

UAbilitySystemComponent* AControlledPlayerState::GetAbilitySystemComponent() const
{
	return PlayerASC;
}

void AControlledPlayerState::AddToBackpack(FGameItemInfo& InInfo)
{
	bool IsExist = false;
	for (FGameItemInfo& Info : StoredItemInfomation)
	{
		if (Info.ItemIndex == InInfo.ItemIndex)
		{
			IsExist = true;
			if (InInfo.CanStack)
			{
				Info.ItemNum += InInfo.ItemNum;
			}		
		}
	}

	CurseOfGoldCoins(InInfo, InInfo.ItemNum);

	if (!IsExist || !InInfo.CanStack)
	{
		StoredItemInfomation.Add(InInfo);
	}

	UpdateBackPackInfomation();

}

void AControlledPlayerState::Merge(TArray<FGameItemInfo>& TargetArray, int32 L, int32 M, int32 R, TArray<FGameItemInfo>& AuxArray)
{
	int32 Index = 0;
	int32 P1 = L;
	int32 P2 = M + 1;

	while (P1 <= M && P2 <= R)
	{
		/*Sort By Index*/
		if (TargetArray[P1].ItemIndex < TargetArray[P2].ItemIndex)
		{
			AuxArray[Index++] = TargetArray[P1++];
		}
		else if (TargetArray[P1].ItemIndex > TargetArray[P2].ItemIndex)
		{
			AuxArray[Index++] = TargetArray[P2++];
		}
		else if (TargetArray[P1].ItemIndex == TargetArray[P2].ItemIndex)
		{
			/*Which one is Equipped will get front position*/
			if (TargetArray[P1].IsEquiped && !TargetArray[P2].IsEquiped)
			{
				AuxArray[Index++] = TargetArray[P1++];
			}
			else if (TargetArray[P1].IsEquiped && !TargetArray[P2].IsEquiped)
			{
				AuxArray[Index++] = TargetArray[P2++];
			}

			/*If both are Equipped ot not Equipped, sort by Durability*/
			if (TargetArray[P1].Durability > TargetArray[P2].Durability)
			{
				AuxArray[Index++] = TargetArray[P1++];
			}
			else
			{
				AuxArray[Index++] = TargetArray[P2++];
			}
		}
	}

	while (P1 <= M)
	{
		AuxArray[Index++] = TargetArray[P1++];
	}
	while (P2 <= R)
	{
		AuxArray[Index++] = TargetArray[P2++];
	}

	for (int32 i = 0; i <= R - L; ++i)
	{
		TargetArray[L + i] = AuxArray[i];
		TargetArray[L + i].BackpackIndex = i + 1;
	}

}

void AControlledPlayerState::Discard(int32 InIndex, int32 Num)
{
	for (FGameItemInfo& Info : StoredItemInfomation)
	{
		if (Info.BackpackIndex == InIndex)
		{
			if (!Info.CanStack)
			{
				SpawnItemToWorld(Info);

				Info.ItemNum -= 1;
				if (Info.ItemNum == 0)
				{
					StoredItemInfomation.RemoveAt(Info.BackpackIndex - 1);
				}

				UpdateBackPackInfomation();

				return;
			}
			else
			{
				SpawnItemToWorld(Info, Num);

				Info.ItemNum -= Num;

				CurseOfGoldCoins(Info, -Num);

				if (Info.ItemNum == 0)
				{
					StoredItemInfomation.RemoveAt(Info.BackpackIndex - 1);
				}

				UpdateBackPackInfomation();

				return;
			}	
		}
	}
}


bool AControlledPlayerState::CheckCanAfford(int32 ItemIndex, int32 RequiredNum)
{
	for (FGameItemInfo& Info : StoredItemInfomation)
	{
		if (Info.ItemIndex == ItemIndex)
		{
			if (Info.ItemNum >= RequiredNum)
			{
				return true;
			}
			else
			{
				return false;
			}
		}
	}
	return false;
}

void AControlledPlayerState::ExpendItem(int32 ItemIndex, int32 Num)
{
	for (FGameItemInfo& Info : StoredItemInfomation)
	{
		if (Info.ItemIndex == ItemIndex)
		{
			Info.ItemNum -= Num;

			CurseOfGoldCoins(Info, -Num);

			if (Info.ItemNum == 0)
			{
				if (Info.IsEquiped)
				{
					FGameplayTagContainer TagContainer;
					if (Info.HandheldType == FDungeonGameplayTags::Get().Handheld_1H || Info.HandheldType == FDungeonGameplayTags::Get().Handheld_2H)
					{
						TagContainer.AddTag(FDungeonGameplayTags::Get().Handheld_1H);
						TagContainer.AddTag(FDungeonGameplayTags::Get().Handheld_2H);
					}
					else
					{
						TagContainer.AddTag(Info.HandheldType);
					}
					ClearEquipmentSlot(Info.EquipSlotIndex, TagContainer);

				}

				StoredItemInfomation.RemoveAt(Info.BackpackIndex - 1);
			}

			UpdateBackPackInfomation();

			return;
		}
	}
}

void AControlledPlayerState::SpawnItemToWorld(FGameItemInfo& Info, int32 Num)
{
	UItemClass* ItemClass = UDungeonAbilitySystemLibrary::GetItemClass(GetPawn());
	if (!IsValid(ItemClass))return;

	for (int i = 0; i < Num; ++i)
	{
		FTransform Transform;
		FVector BaseLocation = GetPawn()->GetActorLocation() + GetPawn()->GetActorForwardVector() * 15;

		BaseLocation = BaseLocation.RotateAngleAxis(FMath::RandRange(-5.f, 5.f), FVector::UpVector);
		
		Transform.SetLocation(BaseLocation);
		Transform.SetRotation(GetPawn()->GetActorQuat());

		//Synchronized Item State
		APickupItems* Item = GetWorld()->SpawnActorDeferred<APickupItems>(ItemClass->GetSubclassByItemIndex(Info.ItemIndex), Transform);
		Item->ItemInfo.IsLoaded = Info.IsLoaded;
		Item->ItemInfo.Durability = Info.Durability;

		Item->FinishSpawning(Transform);
	}	
}

void AControlledPlayerState::SaveItemsToSaveGame(UDungeonSaveGame* SaveData)
{
	SaveData->BackpackItems = StoredItemInfomation;
}

void AControlledPlayerState::UseBackpackConsumable(int32 InIndex)
{
	for (FGameItemInfo& Info : StoredItemInfomation)
	{
		if (Info.BackpackIndex == InIndex)
		{
			HandleUse(Info);
			return;
		}
	}
}

void AControlledPlayerState::UseCurrentConsumable()
{
	for (FGameItemInfo& Info : StoredItemInfomation)
	{
		if (Info.EquipSlotIndex == GetCurrentEquippedConsumableSlot() && Info.ItemType == FDungeonGameplayTags::Get().ItemType_Consume)
		{
			HandleUse(Info);
			return;
		}
	}
}

bool AControlledPlayerState::IsCurrentConsumableEmpty()
{
	for (FGameItemInfo& Info : StoredItemInfomation)
	{
		if (Info.EquipSlotIndex == GetCurrentEquippedConsumableSlot() && Info.ItemType == FDungeonGameplayTags::Get().ItemType_Consume)
		{
			return true;
		}
	}
	return false;
}

void AControlledPlayerState::HandleUse(FGameItemInfo& InInfo)
{
	if (IsValid(InInfo.ItemEffect))
	{
		FGameplayEffectContextHandle ContextHandle = PlayerASC->MakeEffectContext();
		ContextHandle.AddSourceObject(PlayerASC->GetAvatarActor());
		FGameplayEffectSpecHandle SpecHandle = PlayerASC->MakeOutgoingSpec(InInfo.ItemEffect, 1.f, ContextHandle);
		PlayerASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
	}
	InInfo.ItemNum -= 1;
	if (InInfo.ItemNum == 0)
	{
		StoredItemInfomation.RemoveAt(InInfo.BackpackIndex - 1);
	}

	UpdateBackPackInfomation();
}

void AControlledPlayerState::UpdateBackPackInfomation()
{
	int32 ItemNum = StoredItemInfomation.Num();
	TArray<FGameItemInfo> AuxArray;
	AuxArray.SetNum(ItemNum);
	MergeSort(StoredItemInfomation, 0, ItemNum - 1, AuxArray);
	UpdateBackpack.Broadcast(StoredItemInfomation);
}

bool AControlledPlayerState::CheckIsEmptyCellByBackpackIndex(int32 InIndex)
{
	for (FGameItemInfo& Info : StoredItemInfomation)
	{
		if (Info.BackpackIndex == InIndex)
		{
			return true;
		}
	}
	return false;
}

bool AControlledPlayerState::CheckIsEmptyCellByConsumeIndex(int32 InIndex)
{
	for (FGameItemInfo& Info : StoredItemInfomation)
	{
		if (Info.ItemType == FDungeonGameplayTags::Get().ItemType_Consume)
		{
			if (Info.BackpackIndex_Type == InIndex)
			{
				return true;
			}
		}
	}
	return false;
}

bool AControlledPlayerState::CheckIsEmptyCellByWeaponIndex(int32 InIndex)
{
	for (FGameItemInfo& Info : StoredItemInfomation)
	{
		if (Info.ItemType == FDungeonGameplayTags::Get().ItemType_Weapon)
		{
			if (Info.BackpackIndex_Type == InIndex)
			{
				return true;
			}
		}
	}
	return false;
}

bool AControlledPlayerState::CheckIsEmptyCellByAccessoryIndex(int32 InIndex)
{
	for (FGameItemInfo& Info : StoredItemInfomation)
	{
		if (Info.ItemType == FDungeonGameplayTags::Get().ItemType_Accessory)
		{
			if (Info.BackpackIndex_Type == InIndex)
			{
				return true;
			}
		}
	}
	return false;
}

bool AControlledPlayerState::CheckBackpackIsFull()
{
	if (StoredItemInfomation.Num() == 20)
	{
		return true;
	}
	return false;
}

bool AControlledPlayerState::CheckIsEmptyCellByItemTypes(int32 InIndex, FGameplayTagContainer InContainer)
{
	const FDungeonGameplayTags DungeonTags = FDungeonGameplayTags::Get();

	if (InContainer.Num() > 1)
	{
		return CheckIsEmptyCellByBackpackIndex(InIndex);
	}

	if (InContainer.HasTag(DungeonTags.ItemType_Weapon))
	{
		return CheckIsEmptyCellByWeaponIndex(InIndex);
	}
	else if (InContainer.HasTag(DungeonTags.ItemType_Consume))
	{
		return CheckIsEmptyCellByConsumeIndex(InIndex);
	}
	else if (InContainer.HasTag(DungeonTags.ItemType_Accessory))
	{
		return CheckIsEmptyCellByAccessoryIndex(InIndex);
	}

	return false;
}

bool AControlledPlayerState::Equip(int32 EquipSlotIndex, int32 BackpackIndex, FGameplayTagContainer HandheldTypes)
{
	const FDungeonGameplayTags DungeonTags = FDungeonGameplayTags::Get();

	if (IsEquippedInThisSlotAlready(EquipSlotIndex, BackpackIndex, HandheldTypes))
	{
		ClearEquipmentSlot(EquipSlotIndex, HandheldTypes);

		if (!HandheldTypes.HasTag(DungeonTags.Handheld_None)|| !HandheldTypes.HasTag(DungeonTags.Handheld_Body))
		{
			ICharacterInterface::Execute_SetCurrentWeaponType(GetPawn(), DungeonTags.Weapon_Melee);
		}
		return true;
	}
	else
	{
		ClearEquipmentSlot(EquipSlotIndex, HandheldTypes);

		for (FGameItemInfo& Info : StoredItemInfomation)
		{		
			if (HandheldTypes.HasTag(Info.HandheldType) && Info.BackpackIndex == BackpackIndex)
			{
				//If is Equipped already
				FGameplayTagContainer WeaponContainer;
				WeaponContainer.AddTag(DungeonTags.Handheld_1H);
				WeaponContainer.AddTag(DungeonTags.Handheld_2H);
				if (Info.EquipSlotIndex == GetCurrentEquippedWeaponSlot() && WeaponContainer.HasTag(Info.HandheldType))
				{
					ICharacterInterface::Execute_SetHandheldStaticMeshByType(
						GetPawn(),
						nullptr,
						DungeonTags.Handheld_1H);

					ICharacterInterface::Execute_SetCurrentWeaponType(GetPawn(), DungeonTags.Weapon_Melee);
					ICharacterInterface::Execute_SetEHandheldType(GetPawn(), EHandheldType::None);

					ICharacterInterface::Execute_GiveWeaponAbilityByTag(GetPawn(), FGameItemInfo(), DungeonTags.Input_LM, true);
					ICharacterInterface::Execute_GiveWeaponAbilityByTag(GetPawn(), FGameItemInfo(), DungeonTags.Input_LM, false);

				}
				if (Info.EquipSlotIndex == GetCurrentEquippedSecondaryWeaponSlot() && Info.HandheldType.MatchesTag(DungeonTags.Handheld_Secondary))
				{
					ICharacterInterface::Execute_SetHandheldStaticMeshByType(
						GetPawn(),
						nullptr,
						DungeonTags.Handheld_Secondary);

					IPlayerInterface::Execute_SetUnusedSocket(
						GetPawn(),
						nullptr);

					Cast<UCharacterAbilitySystemComponent>(PlayerASC)->RmoveAbilityByInputTag(DungeonTags.Input_RM);
				}
				if (Info.EquipSlotIndex == GetCurrentEquippedConsumableSlot() && Info.HandheldType.MatchesTag(DungeonTags.Handheld_None))
				{
					ICharacterInterface::Execute_GiveWeaponAbilityByTag(GetPawn(), FGameItemInfo(), DungeonTags.Input_R, true);
				}
				//

				Info.EquipSlotIndex = EquipSlotIndex;
				Info.IsEquiped = true;

				if (Info.ItemType == DungeonTags.ItemType_Weapon)
				{
					if (Info.EquipSlotIndex == GetCurrentEquippedWeaponSlot())
					{
						if (Info.HandheldType == DungeonTags.Handheld_2H)
						{
							ICharacterInterface::Execute_SetWeaponIsEmpty(GetPawn(), false);

							ICharacterInterface::Execute_SetWeaponAttachment(GetPawn(), Info);
							ICharacterInterface::Execute_SetCurrentWeaponType(GetPawn(), Info.WeaponType);
							ICharacterInterface::Execute_SetHandheldStaticMeshByType(
								GetPawn(),
								Info.ItemMesh,
								DungeonTags.Handheld_2H);

							ICharacterInterface::Execute_SetEHandheldType(GetPawn(), EHandheldType::TwoHands);

							ICharacterInterface::Execute_GiveWeaponAbilityByTag(GetPawn(), FGameItemInfo(), DungeonTags.Input_LM, true);
							ICharacterInterface::Execute_GiveWeaponAbilityByTag(GetPawn(), Info, DungeonTags.Input_LM, false);

							if (CheckIsEmptyEquipmentSlot(GetCurrentEquippedSecondaryWeaponSlot(), DungeonTags.Handheld_Secondary))
							{
								ICharacterInterface::Execute_SetHandheldStaticMeshByType(
									GetPawn(),
									nullptr,
									DungeonTags.Handheld_Secondary);

								IPlayerInterface::Execute_SetUnusedSocket(
									GetPawn(),
									GetCurrentSecondaryWeaponInfo().ItemMeshOnUnusedSocket);

								Cast<UCharacterAbilitySystemComponent>(PlayerASC)->RmoveAbilityByInputTag(DungeonTags.Input_RM);
							}
							return true;
						}

						if (Info.HandheldType == DungeonTags.Handheld_1H)
						{
							ICharacterInterface::Execute_SetWeaponIsEmpty(GetPawn(), false);

							ICharacterInterface::Execute_SetWeaponAttachment(GetPawn(), Info);
							ICharacterInterface::Execute_SetCurrentWeaponType(GetPawn(), Info.WeaponType);
							ICharacterInterface::Execute_SetHandheldStaticMeshByType(
								GetPawn(),
								Info.ItemMesh,
								DungeonTags.Handheld_1H);

							ICharacterInterface::Execute_SetEHandheldType(GetPawn(), EHandheldType::OneHand);

							ICharacterInterface::Execute_GiveWeaponAbilityByTag(GetPawn(), FGameItemInfo(), DungeonTags.Input_LM, true);
							ICharacterInterface::Execute_GiveWeaponAbilityByTag(GetPawn(), Info, DungeonTags.Input_LM, false);

							return true;
						}
					}
					if (Info.EquipSlotIndex == GetCurrentEquippedSecondaryWeaponSlot())
					{
						if (Info.HandheldType.MatchesTag(DungeonTags.Handheld_Secondary))
						{
							ICharacterInterface::Execute_SetSecondaryWeaponIsEmpty(GetPawn(), false);

							bool Is1HEquipped = CheckIsEmptyEquipmentSlot(GetCurrentEquippedWeaponSlot(), DungeonTags.Handheld_1H);
							bool Is2HEquipped = CheckIsEmptyEquipmentSlot(GetCurrentEquippedWeaponSlot(), DungeonTags.Handheld_2H);

							if (Is1HEquipped)
							{
								ICharacterInterface::Execute_SetHandheldStaticMeshByType(
									GetPawn(),
									Info.ItemMesh,
									DungeonTags.Handheld_Secondary);

								Cast<UCharacterAbilitySystemComponent>(PlayerASC)->RmoveAbilityByInputTag(DungeonTags.Input_RM);
								ICharacterInterface::Execute_GiveWeaponAbilityByTag(GetPawn(), Info, DungeonTags.Input_RM, false);

							}
							else if (Is2HEquipped)
							{
								ICharacterInterface::Execute_SetHandheldStaticMeshByType(
									GetPawn(),
									nullptr,
									DungeonTags.Handheld_Secondary);

								IPlayerInterface::Execute_SetUnusedSocket(
									GetPawn(),
									Info.ItemMeshOnUnusedSocket);
							}
							else
							{
								ICharacterInterface::Execute_SetHandheldStaticMeshByType(
									GetPawn(),
									Info.ItemMesh,
									DungeonTags.Handheld_Secondary);
								ICharacterInterface::Execute_SetEHandheldType(GetPawn(), EHandheldType::OneHand);

								Cast<UCharacterAbilitySystemComponent>(PlayerASC)->RmoveAbilityByInputTag(DungeonTags.Input_RM);
								ICharacterInterface::Execute_GiveWeaponAbilityByTag(GetPawn(), Info, DungeonTags.Input_RM, false);
							}
							return true;
						}
					}
				}
				else if(Info.ItemType==DungeonTags.ItemType_Consume)
				{
					if (Info.EquipSlotIndex == GetCurrentEquippedConsumableSlot())
					{
						Cast<UCharacterAbilitySystemComponent>(PlayerASC)->RmoveAbilityByInputTag(DungeonTags.Input_R);
						GiveItemAbility(Info, DungeonTags.Input_R);
					}
				}
				else if (Info.ItemType == DungeonTags.ItemType_Accessory)
				{
					if (Info.HandheldType == DungeonTags.Handheld_Body_Helmet)
					{
						ICharacterInterface::Execute_SetHandheldStaticMeshByType(
							GetPawn(),
							Info.ItemMesh,
							DungeonTags.Handheld_Body_Helmet);
					}
					EquipAccessory(Info);
				}
			}
		}
	}
	return false;
}

void AControlledPlayerState::ChangeCurrentEquippedWeaponSlot(int32 InSlot)
{
	const FDungeonGameplayTags DungeonTags = FDungeonGameplayTags::Get();

	CurrentEquippedWeaponSlot = InSlot;

	FGameItemInfo WeaponInfo = GetCurrentWeaponInfo();

	ICharacterInterface::Execute_GiveWeaponAbilityByTag(GetPawn(), FGameItemInfo(), DungeonTags.Input_LM, true);
	ICharacterInterface::Execute_GiveWeaponAbilityByTag(GetPawn(), WeaponInfo, DungeonTags.Input_LM, false);

	if (WeaponInfo.HandheldType == DungeonTags.Handheld_2H)
	{
		ICharacterInterface::Execute_SetWeaponAttachment(GetPawn(), WeaponInfo);
		ICharacterInterface::Execute_SetCurrentWeaponType(GetPawn(), WeaponInfo.WeaponType);
		ICharacterInterface::Execute_SetHandheldStaticMeshByType(
			GetPawn(),
			WeaponInfo.ItemMesh,
			DungeonTags.Handheld_2H);

		ICharacterInterface::Execute_SetWeaponIsEmpty(GetPawn(), false);

		ICharacterInterface::Execute_SetEHandheldType(GetPawn(), EHandheldType::TwoHands);

		if (CheckIsEmptyEquipmentSlot(GetCurrentEquippedSecondaryWeaponSlot(), DungeonTags.Handheld_Secondary))
		{
			ICharacterInterface::Execute_SetHandheldStaticMeshByType(
				GetPawn(),
				nullptr,
				DungeonTags.Handheld_Secondary);

			IPlayerInterface::Execute_SetUnusedSocket(
				GetPawn(),
				GetCurrentSecondaryWeaponInfo().ItemMeshOnUnusedSocket);

			Cast<UCharacterAbilitySystemComponent>(PlayerASC)->RmoveAbilityByInputTag(DungeonTags.Input_RM);
		}
	}

	if (WeaponInfo.HandheldType == DungeonTags.Handheld_1H)
	{
		ICharacterInterface::Execute_SetWeaponAttachment(GetPawn(), WeaponInfo);
		ICharacterInterface::Execute_SetCurrentWeaponType(GetPawn(), WeaponInfo.WeaponType);
		ICharacterInterface::Execute_SetHandheldStaticMeshByType(
			GetPawn(),
			WeaponInfo.ItemMesh,
			DungeonTags.Handheld_1H);

		ICharacterInterface::Execute_SetWeaponIsEmpty(GetPawn(), false);

		ICharacterInterface::Execute_SetEHandheldType(GetPawn(), EHandheldType::OneHand);

		if (CheckIsEmptyEquipmentSlot(GetCurrentEquippedSecondaryWeaponSlot(), DungeonTags.Handheld_Secondary))
		{
			ICharacterInterface::Execute_SetHandheldStaticMeshByType(
				GetPawn(),
				GetCurrentSecondaryWeaponInfo().ItemMesh,
				DungeonTags.Handheld_Secondary);

			IPlayerInterface::Execute_SetUnusedSocket(
				GetPawn(), 
				nullptr);

			Cast<UCharacterAbilitySystemComponent>(PlayerASC)->RmoveAbilityByInputTag(DungeonTags.Input_RM);
			ICharacterInterface::Execute_GiveWeaponAbilityByTag(GetPawn(), GetCurrentSecondaryWeaponInfo(), DungeonTags.Input_RM, false);
		}
	}

	if (WeaponInfo.HandheldType == DungeonTags.Handheld_None)
	{
		ICharacterInterface::Execute_SetWeaponAttachment(GetPawn(), WeaponInfo);
		ICharacterInterface::Execute_SetCurrentWeaponType(GetPawn(), WeaponInfo.WeaponType);
		ICharacterInterface::Execute_SetHandheldStaticMeshByType(
			GetPawn(),
			nullptr,
			DungeonTags.Handheld_1H);

		ICharacterInterface::Execute_SetWeaponIsEmpty(GetPawn(), true);

		if (CheckIsEmptyEquipmentSlot(GetCurrentEquippedSecondaryWeaponSlot(), DungeonTags.Handheld_Secondary))
		{
			ICharacterInterface::Execute_SetHandheldStaticMeshByType(
				GetPawn(),
				GetCurrentSecondaryWeaponInfo().ItemMesh,
				DungeonTags.Handheld_Secondary);

			IPlayerInterface::Execute_SetUnusedSocket(
				GetPawn(), 
				nullptr);

			Cast<UCharacterAbilitySystemComponent>(PlayerASC)->RmoveAbilityByInputTag(DungeonTags.Input_RM);
			ICharacterInterface::Execute_GiveWeaponAbilityByTag(GetPawn(), GetCurrentSecondaryWeaponInfo(), DungeonTags.Input_RM, false);

			ICharacterInterface::Execute_SetEHandheldType(GetPawn(), EHandheldType::OneHand);
		}
		else
		{
			ICharacterInterface::Execute_SetEHandheldType(GetPawn(), EHandheldType::None);
		}
	}
}

void AControlledPlayerState::ChangeCurrentEquippedSecondaryWeaponSlot(int32 InSlot)
{
	const FDungeonGameplayTags DungeonTags = FDungeonGameplayTags::Get();

	CurrentEquippedSecondaryWeaponSlot = InSlot;

	FGameItemInfo SecondaryWeaponInfo = GetCurrentSecondaryWeaponInfo();

	bool Is1HEquipped = CheckIsEmptyEquipmentSlot(GetCurrentEquippedWeaponSlot(), DungeonTags.Handheld_1H);
	bool Is2HEquipped = CheckIsEmptyEquipmentSlot(GetCurrentEquippedWeaponSlot(), DungeonTags.Handheld_2H);

	if (SecondaryWeaponInfo.HandheldType != DungeonTags.Handheld_None)
	{
		ICharacterInterface::Execute_SetSecondaryWeaponIsEmpty(GetPawn(), false);

		if (Is1HEquipped)
		{
			Cast<UCharacterAbilitySystemComponent>(PlayerASC)->RmoveAbilityByInputTag(DungeonTags.Input_RM);
			ICharacterInterface::Execute_GiveWeaponAbilityByTag(GetPawn(), SecondaryWeaponInfo, DungeonTags.Input_RM, false);

			IPlayerInterface::Execute_SetUnusedSocket(
				GetPawn(),
				nullptr);

			ICharacterInterface::Execute_SetHandheldStaticMeshByType(
				GetPawn(),
				SecondaryWeaponInfo.ItemMesh,
				DungeonTags.Handheld_Secondary);
		}
		else if (Is2HEquipped)
		{
			ICharacterInterface::Execute_SetHandheldStaticMeshByType(
				GetPawn(),
				nullptr,
				DungeonTags.Handheld_Secondary);

			IPlayerInterface::Execute_SetUnusedSocket(
				GetPawn(),
				SecondaryWeaponInfo.ItemMeshOnUnusedSocket);

			Cast<UCharacterAbilitySystemComponent>(PlayerASC)->RmoveAbilityByInputTag(DungeonTags.Input_RM);
		}
		else
		{
			Cast<UCharacterAbilitySystemComponent>(PlayerASC)->RmoveAbilityByInputTag(DungeonTags.Input_RM);
			ICharacterInterface::Execute_GiveWeaponAbilityByTag(GetPawn(), SecondaryWeaponInfo, DungeonTags.Input_RM, false);

			IPlayerInterface::Execute_SetUnusedSocket(
				GetPawn(),
				nullptr);

			ICharacterInterface::Execute_SetHandheldStaticMeshByType(
				GetPawn(),
				SecondaryWeaponInfo.ItemMesh,
				DungeonTags.Handheld_Secondary);

			ICharacterInterface::Execute_SetEHandheldType(GetPawn(), EHandheldType::OneHand);
		}
	}
	else
	{
		ICharacterInterface::Execute_SetHandheldStaticMeshByType(
			GetPawn(),
			nullptr,
			DungeonTags.Handheld_Secondary);

		IPlayerInterface::Execute_SetUnusedSocket(
			GetPawn(),
			nullptr);

		if (!Is1HEquipped && !Is2HEquipped)
		{
			ICharacterInterface::Execute_SetEHandheldType(GetPawn(), EHandheldType::None);
		}

		Cast<UCharacterAbilitySystemComponent>(PlayerASC)->RmoveAbilityByInputTag(DungeonTags.Input_RM);
		ICharacterInterface::Execute_SetSecondaryWeaponIsEmpty(GetPawn(), true);
	}
}

void AControlledPlayerState::ChangeCurrentEquippedConsumableSlot(int32 InSlot)
{
	const FDungeonGameplayTags DungeonTags = FDungeonGameplayTags::Get();

	CurrentEquippedConsumableSlot = InSlot;

	FGameItemInfo ConsumableInfo = GetCurrentConsumableInfo();

	Cast<UCharacterAbilitySystemComponent>(PlayerASC)->RmoveAbilityByInputTag(DungeonTags.Input_R);
	GiveItemAbility(ConsumableInfo, DungeonTags.Input_R);
}

void AControlledPlayerState::ClearEquipmentSlot(int32 EquipSlotIndex, FGameplayTagContainer HandheldTypes)
{
	const FDungeonGameplayTags DungeonTags = FDungeonGameplayTags::Get();

	for (FGameItemInfo& Info : StoredItemInfomation)
	{
		if (Info.EquipSlotIndex == EquipSlotIndex && HandheldTypes.HasTag(Info.HandheldType))
		{
			Info.IsEquiped = false;

			if (Info.ItemType == DungeonTags.ItemType_Weapon)
			{
				if (Info.EquipSlotIndex == GetCurrentEquippedWeaponSlot())
				{
					if (Info.HandheldType == DungeonTags.Handheld_2H)
					{
						ICharacterInterface::Execute_SetCurrentWeaponType(GetPawn(), DungeonTags.Weapon_Melee);
						ICharacterInterface::Execute_SetHandheldStaticMeshByType(GetPawn(), nullptr, DungeonTags.Handheld_2H);
						ICharacterInterface::Execute_SetWeaponIsEmpty(GetPawn(), true);

						ICharacterInterface::Execute_GiveWeaponAbilityByTag(GetPawn(), FGameItemInfo(), DungeonTags.Input_LM, true);
						ICharacterInterface::Execute_GiveWeaponAbilityByTag(GetPawn(), FGameItemInfo(), DungeonTags.Input_LM, false);

						if (CheckIsEmptyEquipmentSlot(GetCurrentEquippedSecondaryWeaponSlot(), DungeonTags.Handheld_Secondary))
						{
							ICharacterInterface::Execute_SetEHandheldType(GetPawn(), EHandheldType::OneHand);

							IPlayerInterface::Execute_SetUnusedSocket(
								GetPawn(), 
								nullptr);

							ICharacterInterface::Execute_SetHandheldStaticMeshByType(
								GetPawn(),
								GetCurrentSecondaryWeaponInfo().ItemMesh,
								DungeonTags.Handheld_Secondary);

							ICharacterInterface::Execute_SetEHandheldType(GetPawn(), EHandheldType::OneHand);

							Cast<UCharacterAbilitySystemComponent>(PlayerASC)->RmoveAbilityByInputTag(DungeonTags.Input_RM);
							ICharacterInterface::Execute_GiveWeaponAbilityByTag(GetPawn(), GetCurrentSecondaryWeaponInfo(), DungeonTags.Input_RM, true);
						}
						else
						{
							ICharacterInterface::Execute_SetEHandheldType(GetPawn(), EHandheldType::None);
						}

					}

					if (Info.HandheldType == DungeonTags.Handheld_1H)
					{
						ICharacterInterface::Execute_SetCurrentWeaponType(GetPawn(), DungeonTags.Weapon_Melee);
						ICharacterInterface::Execute_SetHandheldStaticMeshByType(GetPawn(), nullptr, DungeonTags.Handheld_1H);
						ICharacterInterface::Execute_SetWeaponIsEmpty(GetPawn(), true);

						ICharacterInterface::Execute_GiveWeaponAbilityByTag(GetPawn(), FGameItemInfo(), DungeonTags.Input_LM, true);
						ICharacterInterface::Execute_GiveWeaponAbilityByTag(GetPawn(), FGameItemInfo(), DungeonTags.Input_LM, false);

						if (CheckIsEmptyEquipmentSlot(GetCurrentEquippedSecondaryWeaponSlot(), DungeonTags.Handheld_Secondary))
						{
							ICharacterInterface::Execute_SetEHandheldType(GetPawn(), EHandheldType::OneHand);
						}
						else
						{
							ICharacterInterface::Execute_SetEHandheldType(GetPawn(), EHandheldType::None);
						}
					}
				}
				if (Info.EquipSlotIndex == GetCurrentEquippedSecondaryWeaponSlot())
				{
					if (Info.HandheldType.MatchesTag(DungeonTags.Handheld_Secondary))
					{
						ICharacterInterface::Execute_SetHandheldStaticMeshByType(GetPawn(), nullptr, DungeonTags.Handheld_Secondary);

						IPlayerInterface::Execute_SetUnusedSocket(
							GetPawn(), 
							nullptr);

						ICharacterInterface::Execute_SetSecondaryWeaponIsEmpty(GetPawn(), true);

						bool Is1HEquipped = CheckIsEmptyEquipmentSlot(GetCurrentEquippedWeaponSlot(), DungeonTags.Handheld_1H);
						bool Is2HEquipped = CheckIsEmptyEquipmentSlot(GetCurrentEquippedWeaponSlot(), DungeonTags.Handheld_2H);			

						if (!Is1HEquipped && !Is2HEquipped)
						{
							ICharacterInterface::Execute_SetEHandheldType(GetPawn(), EHandheldType::None);
						}

						Cast<UCharacterAbilitySystemComponent>(PlayerASC)->RmoveAbilityByInputTag(DungeonTags.Input_RM);

					}
				}
			}
			else if (Info.ItemType == DungeonTags.ItemType_Consume)
			{
				if (Info.EquipSlotIndex == GetCurrentEquippedConsumableSlot())
				{
					Cast<UCharacterAbilitySystemComponent>(PlayerASC)->RmoveAbilityByInputTag(DungeonTags.Input_R);
				}
			}
			else if (Info.ItemType == DungeonTags.ItemType_Accessory)
			{
				if (Info.HandheldType == DungeonTags.Handheld_Body_Helmet)
				{
					ICharacterInterface::Execute_SetHandheldStaticMeshByType(GetPawn(), nullptr, DungeonTags.Handheld_Body_Helmet);
				}

				SetAccessorySlotStatus(Info.EquipSlotIndex, true);
				RemoveAccessory(Info.EquipSlotIndex, UCharacterAbilitySystemComponent::GetInputTagByAbilityClass(Info.ItemAbility));

			}

			Info.EquipSlotIndex = 0;

			return;
		}
	}
}

bool AControlledPlayerState::IsEquippedInThisSlotAlready(int32 EquipSlotIndex, int32 BackpackIndex, FGameplayTagContainer HandheldTypes)
{
	for (FGameItemInfo& Info : StoredItemInfomation)
	{
		if (HandheldTypes.HasTag(Info.HandheldType))
		{	
			if (Info.BackpackIndex == BackpackIndex)
			{
				if (Info.EquipSlotIndex == EquipSlotIndex)
				{
					return true;
				}
			}
		}	
	}
	return false;
}

bool AControlledPlayerState::CheckIsEmptyEquipmentSlot(int32 EquipSlotIndex, const FGameplayTag& HandheldType)
{
	for (FGameItemInfo& Info : StoredItemInfomation)
	{
		if (Info.HandheldType.MatchesTag(HandheldType))
		{
			if (Info.EquipSlotIndex == EquipSlotIndex)
			{
				return true;
			}
		}
	}
	return false;
}

FGameItemInfo AControlledPlayerState::GetCurrentWeaponInfo()
{
	for (FGameItemInfo& Info : StoredItemInfomation)
	{
		if (Info.EquipSlotIndex == GetCurrentEquippedWeaponSlot())
		{
			if(Info.HandheldType == FDungeonGameplayTags::Get().Handheld_1H || Info.HandheldType == FDungeonGameplayTags::Get().Handheld_2H)
			{
				return Info;
			}
		}
	}
	return FGameItemInfo();
}

FGameItemInfo AControlledPlayerState::GetCurrentSecondaryWeaponInfo()
{
	for (FGameItemInfo& Info : StoredItemInfomation)
	{
		if (Info.EquipSlotIndex == GetCurrentEquippedSecondaryWeaponSlot() && Info.HandheldType.MatchesTag(FDungeonGameplayTags::Get().Handheld_Secondary))
		{
			return Info;
		}
	}
	return FGameItemInfo();
}

FGameItemInfo AControlledPlayerState::GetCurrentConsumableInfo()
{
	for (FGameItemInfo& Info : StoredItemInfomation)
	{
		if (Info.EquipSlotIndex == GetCurrentEquippedConsumableSlot() && Info.HandheldType == FDungeonGameplayTags::Get().Handheld_None)
		{
			return Info;
		}
	}
	return FGameItemInfo();
}

int32 AControlledPlayerState::GetCurrentEquippedWeaponSlot()
{
	return CurrentEquippedWeaponSlot;
}

int32 AControlledPlayerState::GetCurrentEquippedSecondaryWeaponSlot()
{
	return CurrentEquippedSecondaryWeaponSlot;
}

int32 AControlledPlayerState::GetCurrentEquippedConsumableSlot()
{
	return CurrentEquippedConsumableSlot;
}

void AControlledPlayerState::SetCurrentEquippedWeaponSlot(int32 InSlot)
{
	CurrentEquippedWeaponSlot = InSlot;
}

void AControlledPlayerState::SetCurrentEquippedSecondaryWeaponSlot(int32 InSlot)
{
	CurrentEquippedSecondaryWeaponSlot = InSlot;
}

void AControlledPlayerState::SetCurrentEquippedConsumableSlot(int32 InSlot)
{
	CurrentEquippedConsumableSlot = InSlot;
}

int32 AControlledPlayerState::GetEmptyAccessorySlot()
{
	if (IsAccessorySlotEmpty_1)
	{
		return 1;
	}
	else if (IsAccessorySlotEmpty_2)
	{
		return 2;
	}
	else if (IsAccessorySlotEmpty_3)
	{
		return 3;
	}

	return -1;
}

bool AControlledPlayerState::CheckHelmet()
{
	for (FGameItemInfo& Info : StoredItemInfomation)
	{
		if (Info.IsEquiped && Info.HandheldType == FDungeonGameplayTags::Get().Handheld_Body_Helmet)
		{
			return true;
		}
	}
	return false;
}

void AControlledPlayerState::SetAccessorySlotStatus(int32 InSlot, bool InBool)
{
	switch (InSlot) {
	case 1:IsAccessorySlotEmpty_1 = InBool;
		break;
	case 2:IsAccessorySlotEmpty_2 = InBool;
		break;
	case 3: IsAccessorySlotEmpty_3 = InBool;
		break;
	}
}

int32 AControlledPlayerState::GetNumByType(FGameplayTagContainer InContainer)
{
	const FDungeonGameplayTags DungeonTags = FDungeonGameplayTags::Get();
	int32 Num = 0;
	for (FGameItemInfo& Info : StoredItemInfomation)
	{
		if (InContainer.HasTag(Info.HandheldType))
		{
			++Num;
		}
	}
	return Num;
}

void AControlledPlayerState::GiveItemAbility(const FGameItemInfo& InInfo, const FGameplayTag& InputTag)
{
	if (IsValid(InInfo.ItemAbility))
	{
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(InInfo.ItemAbility);
		AbilitySpec.DynamicAbilityTags.AddTag(InputTag);
		PlayerASC->GiveAbility(AbilitySpec);
	}
}

void AControlledPlayerState::EquipAccessory(const FGameItemInfo& InInfo)
{
	if (IsValid(InInfo.ItemEffect))
	{
		FGameplayEffectContextHandle ContextHandle = PlayerASC->MakeEffectContext();
		ContextHandle.AddSourceObject(PlayerASC->GetAvatarActor());
		FGameplayEffectSpecHandle SpecHandle = PlayerASC->MakeOutgoingSpec(InInfo.ItemEffect, 1.f, ContextHandle);
		FActiveGameplayEffectHandle ActiveEffectHandle = PlayerASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());

		SlotToAccessoryEffects.Add(InInfo.EquipSlotIndex, ActiveEffectHandle);
	}

	if (IsValid(InInfo.ItemAbility))
	{	
		Cast<UCharacterAbilitySystemComponent>(PlayerASC)->AbilityInputTagHeld(UCharacterAbilitySystemComponent::GetInputTagByAbilityClass(InInfo.ItemAbility));
	}
}

void AControlledPlayerState::RemoveAccessory(int32 InSlot, const FGameplayTag& InTag)
{
	const FDungeonGameplayTags DungeonTags = FDungeonGameplayTags::Get();

	if(SlotToAccessoryEffects[InSlot].IsValid())
	{
		PlayerASC->RemoveActiveGameplayEffect(SlotToAccessoryEffects[InSlot]);
		SlotToAccessoryEffects.FindAndRemoveChecked(InSlot);
	}

	Cast<UCharacterAbilitySystemComponent>(PlayerASC)->BroadcastDeactivateAccessoryAbilityDelegate(InTag);
}

void AControlledPlayerState::RemoveAccessoryEffect()
{
	if (SlotToAccessoryEffects.IsEmpty())return;

	for (auto& Pair : SlotToAccessoryEffects)
	{
		PlayerASC->RemoveActiveGameplayEffect(Pair.Value);
	}
}

void AControlledPlayerState::SetDurability(float InNum, bool bSecondary)
{
	for (FGameItemInfo& Info : StoredItemInfomation)
	{
		if (bSecondary)
		{
			if (Info.EquipSlotIndex == GetCurrentEquippedSecondaryWeaponSlot() && Info.HandheldType.MatchesTag(FDungeonGameplayTags::Get().Handheld_Secondary))
			{
				Info.Durability += InNum;
				UpdateBackPackInfomation();
				return;
			}
		}
		else
		{
			if (Info.EquipSlotIndex == GetCurrentEquippedWeaponSlot())
			{
				if (Info.HandheldType == FDungeonGameplayTags::Get().Handheld_1H || Info.HandheldType == FDungeonGameplayTags::Get().Handheld_2H)
				{
					Info.Durability += InNum;
					UpdateBackPackInfomation();
					return;
				}
			}
		}
	}
}

float AControlledPlayerState::GetDurability(bool bSecondary)
{
	if (bSecondary)
	{
		return GetCurrentSecondaryWeaponInfo().Durability;
	}
	else
	{
		return GetCurrentWeaponInfo().Durability;
	}
}

bool AControlledPlayerState::CanLoad()
{
	for (FGameItemInfo& Info : StoredItemInfomation)
	{
		if (Info.ItemIndex == 101)
		{
			return true;
		}
	}
	return false;
}

bool AControlledPlayerState::GetIsLoaded()
{
	return GetCurrentWeaponInfo().IsLoaded;
}

void AControlledPlayerState::SetIsLoaded(bool InBool)
{
	for (FGameItemInfo& Info : StoredItemInfomation)
	{
		if (Info.EquipSlotIndex == GetCurrentEquippedWeaponSlot())
		{
			Info.IsLoaded = InBool;
			break;
		}
	}

	if (InBool == true)
	{
		for (FGameItemInfo& Info : StoredItemInfomation)
		{
			if (Info.ItemIndex == 101)
			{
				Info.ItemNum -= 1;
				if (Info.ItemNum == 0)
				{
					StoredItemInfomation.RemoveAt(Info.BackpackIndex - 1);
				}
				UpdateBackPackInfomation();
				return;
			}
		}
	}
}

void AControlledPlayerState::CurseOfGoldCoins(FGameItemInfo& InInfo, int32 NumToChange)
{
	if (InInfo.ItemIndex == 999)
	{
		NumOfCoins += NumToChange;

		for (int32 i = 0; i < (NumToChange >= 0 ? NumToChange : -NumToChange); ++i)
		{		
			FGameplayTag AttributeTag = *IndexToAttributeTag.Find(FMath::RandRange(0, 6));

			Cast<UCharacterAbilitySystemComponent>(PlayerASC)->UpgradeAttributeByTag(AttributeTag, NumToChange >= 0);
		}
	}
}