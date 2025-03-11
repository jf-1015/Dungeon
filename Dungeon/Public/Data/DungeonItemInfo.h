// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "DungeonGameplayTags.h"//
#include "DungeonItemInfo.generated.h"

class UGameplayEffect;
class UGameplayAbility;

USTRUCT(BlueprintType)
struct FGameItemInfo
{
	GENERATED_BODY();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 ItemIndex = -1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayTag ItemType = FGameplayTag();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayTag HandheldType = FDungeonGameplayTags::Get().Handheld_None;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayTag WeaponType = FDungeonGameplayTags::Get().Weapon_Melee;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 ItemNum = 0;

	UPROPERTY(BlueprintReadOnly)
	int32 BackpackIndex = 0;

	UPROPERTY(BlueprintReadOnly)
	int32 BackpackIndex_Type = 0;

	UPROPERTY(BlueprintReadOnly)
	int32 EquipSlotIndex = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* ItemImage = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UGameplayEffect>ItemEffect;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UGameplayAbility>ItemAbility;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UStaticMesh>ItemMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UStaticMesh>ItemMeshOnUnusedSocket;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bUsedDirectly = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool CanStack = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool IsEquiped = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool IsLoaded = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxDurability = 1.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Durability = 1.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (MultiLine = "true"))
	FText Description = FText();
};

inline bool operator==(const FGameItemInfo& A, const FGameItemInfo& B)
{
	if (A.BackpackIndex == B.BackpackIndex)
	{
		return true;
	}
	return false;
}

/**
 * 
 */
UCLASS()
class DUNGEON_API UDungeonItemInfo : public UDataAsset
{
	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FGameItemInfo>GameItemInfomation;

	FGameItemInfo GetItemInfoByItemIndex(int32 Index);
};