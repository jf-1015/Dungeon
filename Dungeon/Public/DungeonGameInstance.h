// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "DungeonGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class DUNGEON_API UDungeonGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:

	UPROPERTY()
	FString LoadSlotName = FString();

	UPROPERTY()
	int32 LoadSlotIndex = 0;

	UPROPERTY()
	FName PlayerStartTag = FName();
};