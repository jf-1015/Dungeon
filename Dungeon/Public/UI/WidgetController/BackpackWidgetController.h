// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetController/PlayerWidgetController.h"
#include "Data/DungeonItemInfo.h"//
#include "BackpackWidgetController.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FUpdateBackpackDelegate, const FGameItemInfo&, Info);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FUpdateWeaponBackpackDelegate, const FGameItemInfo&, WeaponInfo);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FUpdateConsumeBackpackDelegate, const FGameItemInfo&, ConsumeInfo);

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class DUNGEON_API UBackpackWidgetController : public UPlayerWidgetController
{
	GENERATED_BODY()

public:

	virtual void BroadcastInitialValues()override;
	virtual void BindCallBacksToDependencies()override;

	UFUNCTION(BlueprintCallable)
	void OpenBackpack();

	UFUNCTION(BlueprintCallable)
	void Discard(int32 InIndex, int32 Num = 1);

	UFUNCTION(BlueprintCallable)
	void Use(int32 InIndex);

	UFUNCTION(BlueprintPure)
	bool CheckIsEmptyCellByItemTypes(int32 InIndex, FGameplayTagContainer InContainer);

	/*Delegate*/
	UPROPERTY(BlueprintAssignable)
	FUpdateBackpackDelegate UpdateBackpackDelegate;
	/*Delegate*/

protected:

	UPROPERTY()
	FGameplayTagContainer SelectedTagContainer = FGameplayTagContainer();

	UFUNCTION(BlueprintCallable)
	void SetSelectedContainer(FGameplayTagContainer InContainer);

	UFUNCTION(BlueprintPure)
	FGameplayTagContainer GetSelectedContainer();
	
};