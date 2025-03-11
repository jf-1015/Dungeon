// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Actor/DungeonProjectile.h"//
#include "ProjectileClass.generated.h"

USTRUCT(BlueprintType)
struct FIndexToProjectile
{
	GENERATED_BODY();

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	int32 ItemIndex = -1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<ADungeonProjectile>ProjectileClass;
};

/**
 * 
 */
UCLASS()
class DUNGEON_API UProjectileClass : public UDataAsset
{
	GENERATED_BODY()
	
public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FIndexToProjectile>ItemProjectiles;

	TSubclassOf<ADungeonProjectile> GetSubclassByItemIndex(int32 Index);
};