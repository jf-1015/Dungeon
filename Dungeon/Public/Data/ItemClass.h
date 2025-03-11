// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Actor/PickupItems.h"//
#include "ItemClass.generated.h"

USTRUCT(BlueprintType)
struct FIndexToItem
{
	GENERATED_BODY();

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	int32 ItemIndex = -1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<APickupItems>ItemClass;
};

/**
 * 
 */
UCLASS()
class DUNGEON_API UItemClass : public UDataAsset
{
	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	TArray<FIndexToItem>Items;

	TSubclassOf<APickupItems> GetSubclassByItemIndex(int32 Index);
};