// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetController/PlayerWidgetController.h"
#include "Character/NPCBase.h"//
#include "ShopWidgetController.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FUpdateWidgetShop,const FItemsToShop&, Info);

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class DUNGEON_API UShopWidgetController : public UPlayerWidgetController
{
	GENERATED_BODY()
	
public:

	UFUNCTION(BlueprintCallable)
	void SetShoper(AActor* InActor);

	virtual void BroadcastInitialValues()override;
	virtual void BindCallBacksToDependencies()override;

	UPROPERTY(BlueprintAssignable)
	FUpdateWidgetShop UpdateWidgetShop;

	UFUNCTION(BlueprintCallable)
	void SetItemNumBySlotIndex(int32 Slot, int32 Num);
	UFUNCTION(BlueprintCallable)
	int32 GetItemNumBySlotIndex(int32 Index);

	UFUNCTION(BlueprintCallable)
	bool CheckCanAfford(int32 ItemIndex, int32 RequiredNum);

	UFUNCTION(BlueprintCallable)
	void Buy(int32 ItemIndex, int32 RequiredNum, int32 SlotIndex, int32 NumPurchased = 1);

protected:

	UPROPERTY()
	TObjectPtr<AActor>Shoper;

};