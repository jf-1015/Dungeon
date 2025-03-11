// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"//
#include "InputConfig.generated.h"

class UInputAction;

USTRUCT(BlueprintType)
struct FPlayerInputAction
{
	GENERATED_BODY();

	UPROPERTY(EditDefaultsOnly)
	UInputAction* InputAction = nullptr;

	UPROPERTY(EditDefaultsOnly)
	FGameplayTag InputTag = FGameplayTag();

};

/**
 * 
 */
UCLASS()
class DUNGEON_API UInputConfig : public UDataAsset
{
	GENERATED_BODY()
	
public:

	UInputAction* FindAbilityInputActionForTag(const FGameplayTag& InputTag, bool bLogNotFound = false);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FPlayerInputAction>PlayerInputActions;

};