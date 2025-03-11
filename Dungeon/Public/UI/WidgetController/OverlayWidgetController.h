// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetController/PlayerWidgetController.h"
#include "GameplayTagContainer.h"//
#include "OverlayWidgetController.generated.h"

struct FGameplayTag;
struct FGameItemInfo;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnAttributeChanged, float, NewValue, FGameplayTag, AttributeTag);

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class DUNGEON_API UOverlayWidgetController : public UPlayerWidgetController
{
	GENERATED_BODY()

public:

	virtual void BroadcastInitialValues()override;
	virtual void BindCallBacksToDependencies()override;

	UPROPERTY(BlueprintReadOnly)
	FGameplayTagContainer AttributesToShow;

	UPROPERTY(BlueprintAssignable, Category = "GAS|Attribute")
	FOnAttributeChanged OnAttributeChangedDelegate;
};