// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "SaveInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class USaveInterface : public UInterface
{
	GENERATED_BODY()
};

struct FGameItemInfo;

/**
 * 
 */
class DUNGEON_API ISaveInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	bool ShouldLoadTransform();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void LoadActor();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void LoadEnemy(const FGameItemInfo& WeaponInfo, const FGameItemInfo& SecondaryWeaponInfo);

	/*LoadActor call this*/
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void InitializeStatus();
};