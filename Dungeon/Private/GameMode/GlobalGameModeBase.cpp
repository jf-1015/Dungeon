// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/GlobalGameModeBase.h"
#include "Kismet/GameplayStatics.h"//
#include "UI/ViewModel/Mvvm_LoadSlot.h"//
#include "SaveGame/DungeonSaveGame.h"//
#include "DungeonGameInstance.h"//
#include "EngineUtils.h"//
#include "Interaction/CharacterInterface.h"//
#include "Interaction/EnemyInterface.h"//
#include "Interaction/SaveInterface.h"//
#include "Interaction/PickupInterface.h"//
#include "Interaction/ProjectileInterface.h"//
#include "Serialization/ObjectAndNameAsStringProxyArchive.h"//
#include "Data/ItemClass.h"//
#include "Data/ProjectileClass.h"//
#include "Actor/PickupItems.h"//
#include "Actor/DungeonProjectile.h"//
#include "GameFramework/ProjectileMovementComponent.h"//
#include "Character/CharacterBase.h"//
#include "AbilitySystemBlueprintLibrary.h"//
#include "Components/BoxComponent.h"//

UDungeonSaveGame* AGlobalGameModeBase::GetLoadSlotSaveData(const FString& InSlotName, int32 InSlotIndex)
{
	USaveGame* SaveGameObject = nullptr;

	if (UGameplayStatics::DoesSaveGameExist(InSlotName, InSlotIndex))
	{
		SaveGameObject = UGameplayStatics::LoadGameFromSlot(InSlotName, InSlotIndex);
	}
	else
	{
		SaveGameObject = UGameplayStatics::CreateSaveGameObject(SaveGameClass);
	}
	UDungeonSaveGame* DungeonSaveGame = Cast<UDungeonSaveGame>(SaveGameObject);
	return DungeonSaveGame;
}

void AGlobalGameModeBase::SaveLoadSlotData(UMvvm_LoadSlot* InLoadSlot, int32 InSlotIndex)
{
	DeleteSlot(InLoadSlot->GetLoadSlotName(), InSlotIndex);

	USaveGame* SaveGameObject = UGameplayStatics::CreateSaveGameObject(SaveGameClass);
	UDungeonSaveGame* DungeonSaveGame = Cast<UDungeonSaveGame>(SaveGameObject);

	DungeonSaveGame->MapName = InLoadSlot->GetMapName();
	DungeonSaveGame->PlayerName = InLoadSlot->GetPlayerName();
	DungeonSaveGame->SlotStatus = InLoadSlot->SlotStatus;
	DungeonSaveGame->MapAssetName = InLoadSlot->MapAssetName;

	UGameplayStatics::SaveGameToSlot(DungeonSaveGame, InLoadSlot->GetLoadSlotName(), InSlotIndex);
}

void AGlobalGameModeBase::DeleteSlot(const FString& InSlotName, int32 InSlotIndex)
{
	if (UGameplayStatics::DoesSaveGameExist(InSlotName, InSlotIndex))
	{
		UGameplayStatics::DeleteGameInSlot(InSlotName, InSlotIndex);
	}
}

UDungeonSaveGame* AGlobalGameModeBase::RetrieveInGameSaveData()
{
	UDungeonGameInstance* DungeonGI = Cast<UDungeonGameInstance>(GetGameInstance());

	FString InGameLoadSlotName = DungeonGI->LoadSlotName;
	int32 InGameLoadSlotIndex = DungeonGI->LoadSlotIndex;
	
	return GetLoadSlotSaveData(InGameLoadSlotName, InGameLoadSlotIndex);
}

void AGlobalGameModeBase::SaveInGameProgressData(UDungeonSaveGame* SaveObject)
{
	UDungeonGameInstance* DungeonGI = Cast<UDungeonGameInstance>(GetGameInstance());

	FString InGameLoadSlotName = DungeonGI->LoadSlotName;
	int32 InGameLoadSlotIndex = DungeonGI->LoadSlotIndex;

	UGameplayStatics::SaveGameToSlot(SaveObject, InGameLoadSlotName, InGameLoadSlotIndex);
}

void AGlobalGameModeBase::SaveWorldState(UWorld* World, const FString& DestinationMapAssetName)
{
	FString CurrentMapAssetName = World->GetMapName();
	CurrentMapAssetName.RemoveFromStart(World->StreamingLevelsPrefix);

	UDungeonGameInstance* DungeonGI = Cast<UDungeonGameInstance>(GetGameInstance());
	check(DungeonGI);

	if (UDungeonSaveGame* SaveData = GetLoadSlotSaveData(DungeonGI->LoadSlotName, DungeonGI->LoadSlotIndex))
	{
		if (DestinationMapAssetName != FString(""))
		{
			SaveData->MapAssetName = DestinationMapAssetName;
			SaveData->MapName = GetMapNameFormMapAssetName(DestinationMapAssetName);
		}

		//if CurrentMap not in SavedMaps , add it to SavedMaps in SaveData
		if (!SaveData->HasMap(CurrentMapAssetName))
		{
			FSavedMap NewMap;
			NewMap.MapAssetName = CurrentMapAssetName;

			SaveData->SavedMaps.Add(NewMap);
		}

		FSavedMap SavedMap = SaveData->GetSavedMapByMapAssetName(CurrentMapAssetName);
		SavedMap.SavedActors.Empty();
		SavedMap.PickupItems.Empty();
		SavedMap.Projectiles.Empty();
		SavedMap.SavedEnemyInfomation.Empty();

		for (FActorIterator It(World); It; ++It)
		{
			AActor* Actor = *It;

			if (IsValid(Actor) && Actor->Implements<UPickupInterface>())
			{
				FPickupItemInWorld Info;
				Info.ItemInfo = Cast<APickupItems>(Actor)->ItemInfo;
				Info.Transform = Actor->GetActorTransform();
				Info.MeshToShow = Cast<APickupItems>(Actor)->GetMesh();

				SavedMap.PickupItems.Add(Info);
				continue;
			}
			else
			{
				if (IsValid(Actor) && Actor->Implements<UProjectileInterface>())
				{
					ADungeonProjectile* Projectile = Cast<ADungeonProjectile>(Actor);

					FProjectileInWorld Info;
					Info.ProjectielClass = Projectile->GetClass();
					Info.Location = Actor->GetActorLocation();
					Info.Velocity = Projectile->GetComponentVelocity();
					Info.OwnerName = Projectile->OwnerName;
					Info.DamageEffect = Projectile->DamageEffectParams.DamageEffectClass;
					Info.Durability = Projectile->Durability;

					SavedMap.Projectiles.Add(Info);
					continue;
				}
			}

			if (!IsValid(Actor) || !Actor->Implements<USaveInterface>())continue;

			if (Actor->Implements<UEnemyInterface>())
			{
				FSavedEnemyInfo SavedInfo;
				SavedInfo.ActorName = Actor->GetFName();;
				SavedInfo.WeaponInfo = IEnemyInterface::Execute_GetWeaponInfo(Actor);
				SavedInfo.SecondaryWeaponInfo = IEnemyInterface::Execute_GetSecondaryWeaponInfo(Actor);
				
				ICharacterInterface::Execute_SaveEnemyVitalAttribute(Actor, SavedInfo.SavedAttributeInfo);

				SavedMap.SavedEnemyInfomation.AddUnique(SavedInfo);
			}

			FSavedActor SavedActor;
			SavedActor.ActorName = Actor->GetFName();
			SavedActor.ActorTransform = Actor->GetTransform();

			FMemoryWriter MemoryWriter(SavedActor.Bytes);
			FObjectAndNameAsStringProxyArchive Archive(MemoryWriter, true);
			Archive.ArIsSaveGame = true;
			Actor->Serialize(Archive);

			SavedMap.SavedActors.AddUnique(SavedActor);
		}

		for (FSavedMap& MapToReplace : SaveData->SavedMaps)
		{
			if (MapToReplace.MapAssetName == CurrentMapAssetName)
			{
				MapToReplace = SavedMap;
			}
		}

		UGameplayStatics::SaveGameToSlot(SaveData, DungeonGI->LoadSlotName, DungeonGI->LoadSlotIndex);

	}
}

void AGlobalGameModeBase::LoadWorldState(UWorld* World)
{
	FString CurrentMapAssetName = World->GetMapName();
	CurrentMapAssetName.RemoveFromStart(World->StreamingLevelsPrefix);

	UDungeonGameInstance* DungeonGI = Cast<UDungeonGameInstance>(GetGameInstance());
	check(DungeonGI);

	if (UGameplayStatics::DoesSaveGameExist(DungeonGI->LoadSlotName, DungeonGI->LoadSlotIndex))
	{
		UDungeonSaveGame* SaveData = GetLoadSlotSaveData(DungeonGI->LoadSlotName, DungeonGI->LoadSlotIndex);
		FSavedMap SavedMap = SaveData->GetSavedMapByMapAssetName(CurrentMapAssetName);

		if (SavedMap.MapAssetName == FString())return;

		for (FActorIterator It(World); It; ++It)
		{
			AActor* Actor = *It;

			/*Clear Items In World*/
			if (!SaveData->bFirstLoadIn)
			{
				if (IsValid(Actor) && Actor->Implements<UPickupInterface>())
				{
					Actor->Destroy();
					continue;
				}
			}

			if (!IsValid(Actor) || !Actor->Implements<USaveInterface>())continue;

			for (FSavedActor SavedActor : SavedMap.SavedActors)
			{
				if (Actor->GetFName() == SavedActor.ActorName)
				{
					if (ISaveInterface::Execute_ShouldLoadTransform(Actor))
					{
						Actor->SetActorTransform(SavedActor.ActorTransform);
					}

					if (Actor->Implements<UEnemyInterface>())
					{
						for (const FSavedEnemyInfo& Info : SavedMap.SavedEnemyInfomation)
						{
							if (Actor->GetFName() == Info.ActorName)
							{
								IEnemyInterface::Execute_SetWeaponInfo(Actor, Info.WeaponInfo);
								IEnemyInterface::Execute_SetSecondaryWeaponInfo(Actor, Info.SecondaryWeaponInfo);
								ICharacterInterface::Execute_LoadAttribute(Actor, Info.SavedAttributeInfo);
								break;
							}
						}
					}
					
					FMemoryReader MemoryReader(SavedActor.Bytes);
					FObjectAndNameAsStringProxyArchive Archive(MemoryReader, true);
					Archive.ArIsSaveGame = true;
					Actor->Serialize(Archive);// Converts binary bytes back into variables

					ISaveInterface::Execute_LoadActor(Actor);

					/*Load Character Related*/
					if (Actor->Implements<UCharacterInterface>() && !SaveData->bFirstLoadIn)
					{
						for (auto& Info : SavedMap.Projectiles)
						{
							if (Info.OwnerName == Actor->GetFName())
							{
								FTransform Transform;
								Transform.SetLocation(Info.Location);

								ADungeonProjectile* Projectile = GetWorld()->SpawnActorDeferred<ADungeonProjectile>(Info.ProjectielClass, Transform);

								Projectile->SetOwner(Actor);

								Projectile->ProjectileMovement->Velocity = Info.Velocity;

								FDamageEffectParams Params;
								Params.WorldContext = Actor;
								Params.DamageEffectClass = Info.DamageEffect;
								Params.SourceAbilitySystemComponent = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Actor);
								Projectile->DamageEffectParams = Params;

								Projectile->Durability = Info.Durability;

								Projectile->FinishSpawning(Transform);
							}
						}
					}
					/*Load Character Related*/
				}
			}
		}
		/*Load Items In World*/
		for (auto& Info : SavedMap.PickupItems)
		{
			APickupItems* Item = GetWorld()->SpawnActorDeferred<APickupItems>(ItemClass->GetSubclassByItemIndex(Info.ItemInfo.ItemIndex), Info.Transform);
			Item->ItemInfo = Info.ItemInfo;
			Item->SetMesh(Info.MeshToShow);

			Item->FinishSpawning(Info.Transform);
		}
		/*Load Items In World*/
	}
}

void AGlobalGameModeBase::TravelToMap(UMvvm_LoadSlot* InLoadSlot)
{
	const FString SlotName = InLoadSlot->GetLoadSlotName();
	const int32 SlotIndex = InLoadSlot->SlotIndex;

	UGameplayStatics::OpenLevelBySoftObjectPtr(InLoadSlot, NameToMap.FindChecked(InLoadSlot->GetMapName()));
}

FString AGlobalGameModeBase::GetMapNameFormMapAssetName(const FString& InMapAssetName)
{
	for (auto& Map : NameToMap)
	{
		if (Map.Value.ToSoftObjectPath().GetAssetName() == InMapAssetName)
		{
			return Map.Key;
		}
	}
	return FString();
}

void AGlobalGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	NameToMap.Add(DefaultMapName, DefaultMap);
}