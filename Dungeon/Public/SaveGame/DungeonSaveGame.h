// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "Data/DungeonItemInfo.h"//
#include "AbilitySystem/DungeonAbilityTypes.h"//
#include "Interaction/CharacterInterface.h"//
#include "DungeonSaveGame.generated.h"

struct FGameplayTag;
class ADungeonProjectile;

UENUM()
enum ELoadSlotStatus
{
	Vacant,
	EnterName,
	Taken,
};

USTRUCT()
struct FSavedActor
{
	GENERATED_BODY();

	UPROPERTY()
	FName ActorName = FName();

	UPROPERTY()
	FTransform ActorTransform = FTransform();

	//Serialize variables from the actor - only those marked with SvaeGame Specifier
	UPROPERTY()
	TArray<uint8>Bytes;
};

inline bool operator == (const FSavedActor& Left, const FSavedActor& Right)
{
	return Left.ActorName == Right.ActorName;
}

USTRUCT()
struct FSavedEnemyInfo
{
	GENERATED_BODY();

	UPROPERTY()
	FName ActorName = FName();

	UPROPERTY()
	FGameItemInfo WeaponInfo = FGameItemInfo();

	UPROPERTY()
	FGameItemInfo SecondaryWeaponInfo = FGameItemInfo();

	UPROPERTY()
	FSavedEnemyVitalAttribute SavedAttributeInfo = FSavedEnemyVitalAttribute();
};

inline bool operator == (const FSavedEnemyInfo& Left, const FSavedEnemyInfo& Right)
{
	return Left.ActorName == Right.ActorName;
}

USTRUCT()
struct FPickupItemInWorld
{
	GENERATED_BODY();

	UPROPERTY()
	FGameItemInfo ItemInfo = FGameItemInfo();

	UPROPERTY()
	FTransform Transform = FTransform();

	UPROPERTY()
	TObjectPtr<UStaticMesh> MeshToShow = nullptr;
};

USTRUCT()
struct FProjectileInWorld
{
	GENERATED_BODY();

	UPROPERTY()
	TSubclassOf<ADungeonProjectile>ProjectielClass;

	UPROPERTY()
	FVector Location = FVector();

	UPROPERTY()
	FVector Velocity = FVector();

	UPROPERTY()
	FName OwnerName = FName();

	UPROPERTY()
	TSubclassOf<UGameplayEffect> DamageEffect;

	UPROPERTY()
	float Durability = 1.f;
};

USTRUCT()
struct FSavedMap
{
	GENERATED_BODY();

	UPROPERTY()
	FString MapAssetName = FString();

	UPROPERTY()
	TArray<FSavedActor>SavedActors;

	/*PickupActorInWorld*/
	UPROPERTY()
	TArray<FPickupItemInWorld>PickupItems;
	/*PickupActorInWorld*/

	/*ProjectileInWorld*/
	UPROPERTY()
	TArray<FProjectileInWorld>Projectiles;
	/*ProjectileInWorld*/

	UPROPERTY()
	TArray<FSavedEnemyInfo>SavedEnemyInfomation;
};

/**
 * 
 */
UCLASS()
class DUNGEON_API UDungeonSaveGame : public USaveGame
{
	GENERATED_BODY()
	
public:

	UPROPERTY()
	FString MapName = FString("Default Map Name");

	UPROPERTY()
	FString PlayerName = FString("Default PlayerName");

	UPROPERTY()
	TEnumAsByte<ELoadSlotStatus>SlotStatus = ELoadSlotStatus::Vacant;

	UPROPERTY()
	FString MapAssetName = FString("Default MapAssetName");

	UPROPERTY()
	bool bFirstLoadIn = true;

	UPROPERTY()
	TArray<FSavedMap>SavedMaps;

	/*Player*/
	UPROPERTY()
	TArray<FGameItemInfo>BackpackItems;

	UPROPERTY()
	int32 WeaponSlot;

	UPROPERTY()
	int32 SecondaryWeaponSlot;

	UPROPERTY()
	int32 ConsumableSlot;

	UPROPERTY()
	FVector PlayerLocation = FVector();

	UPROPERTY()
	FRotator PlayerRotation = FRotator();
	/*Player*/

	/*Attribute*/
	UPROPERTY()
	float Vitality;

	UPROPERTY()
	float Power;

	UPROPERTY()
	float Dexterity;

	UPROPERTY()
	float Intelligence;

	UPROPERTY()
	float Endurance;

	UPROPERTY()
	float Resilience;

	UPROPERTY()
	float Defence;

	UPROPERTY()
	float Health;

	UPROPERTY()
	float Mana;

	UPROPERTY()
	float Energy;

	UPROPERTY()
	float CurrentResilience;

	UPROPERTY()
	float Food;
	/*Attribute*/

	FSavedMap GetSavedMapByMapAssetName(const FString& InMapAssetName);
	bool HasMap(const FString& InMapAssetName);
};