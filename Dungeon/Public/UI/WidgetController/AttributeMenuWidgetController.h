// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetController/PlayerWidgetController.h"
#include "AttributeMenuWidgetController.generated.h"

class UDungeonAttributeInfo;
struct FDungeonAttributeInfo;
struct FGameplayTag;
struct FGameplayAttribute;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAttributeInfo, const FAttributeInfo&, Info);

/**
 * 
 */
UCLASS(Blueprintable,BlueprintType)
class DUNGEON_API UAttributeMenuWidgetController : public UPlayerWidgetController
{
	GENERATED_BODY()

public:

	virtual void BroadcastInitialValues()override;
	virtual void BindCallBacksToDependencies()override;

	UPROPERTY(BlueprintAssignable, Category = "GAS|Attribute")
	FOnAttributeInfo OnAttributeInfoDelegate;

protected:

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UDungeonAttributeInfo> AttributeInfo;

private:

	void BroadcastAttributeInfo(const FGameplayTag& Tag, const FGameplayAttribute& Attribute);

};