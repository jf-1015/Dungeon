// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"//
#include "DungeonAttributeInfo.generated.h"

USTRUCT(BlueprintType)
struct FAttributeInfo
{
	GENERATED_BODY();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag AttributeTag = FGameplayTag();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText AttributeName = FText();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UTexture2D* AttributeImage = nullptr;

	UPROPERTY(BlueprintReadOnly)
	float AttributeValue = 0.f;

};

/**
 * 
 */
UCLASS()
class DUNGEON_API UDungeonAttributeInfo : public UDataAsset
{
	GENERATED_BODY()

public:

	FAttributeInfo FindAttributeInfoForTag(const FGameplayTag& AttributeTag, bool bLogNotFound = false)const;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FAttributeInfo>AttributeInfomation;

};