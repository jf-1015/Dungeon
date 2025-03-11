// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "GlobalGameModeBase.generated.h"

class UItemClass;
class UProjectileClass;
class UDetailedEquipmemtInfo;
class UEnemyEquipmentInfo;
class UCharacterClassInfo;

class UMvvm_LoadSlot;
class USaveGame;
class UDungeonSaveGame;

/**
 * 
 */
UCLASS()
class DUNGEON_API AGlobalGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:

	/*Infomation*/
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UDetailedEquipmemtInfo>EquipmentInfomation;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UEnemyEquipmentInfo>EnemyEquipmentInfomation;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UItemClass>ItemClass;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UProjectileClass>ProjectileClass;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UCharacterClassInfo>CharacterClassInfomation;
	/*Infomation*/

	/*SaveGame*/
	UDungeonSaveGame* GetLoadSlotSaveData(const FString& InSlotName, int32 InSlotIndex);
	void SaveLoadSlotData(UMvvm_LoadSlot* InLoadSlot,int32 InSlotIndex);

	static void DeleteSlot(const FString& InSlotName, int32 InSlotIndex);

	UDungeonSaveGame* RetrieveInGameSaveData();
	void SaveInGameProgressData(UDungeonSaveGame* SaveObject);

	void SaveWorldState(UWorld* World, const FString& DestinationMapAssetName = FString(""));
	void LoadWorldState(UWorld* World);

	void TravelToMap(UMvvm_LoadSlot* InLoadSlot);

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<USaveGame>SaveGameClass;

	UPROPERTY(EditDefaultsOnly)
	FString DefaultMapName;

	UPROPERTY(EditDefaultsOnly)
	TSoftObjectPtr<UWorld>DefaultMap;

	UPROPERTY(EditDefaultsOnly)
	TMap<FString, TSoftObjectPtr<UWorld>>NameToMap;

	FString GetMapNameFormMapAssetName(const FString& InMapAssetName);
	/*SaveGame*/

	UPROPERTY(EditDefaultsOnly)
	TSoftObjectPtr<UWorld>StartupMap;

protected:

	virtual void BeginPlay()override;
};