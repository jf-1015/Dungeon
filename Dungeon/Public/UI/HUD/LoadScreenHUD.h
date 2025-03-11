// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "LoadScreenHUD.generated.h"

class ULoadScreenWidget;
class UMvvm_LoadScreen;

/**
 * 
 */
UCLASS()
class DUNGEON_API ALoadScreenHUD : public AHUD
{
	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget>LoadScreenWidgetClass;

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<ULoadScreenWidget>LoadScreenWidget;


	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UMvvm_LoadScreen>LoadScreenClass;

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UMvvm_LoadScreen>LoadScreen;

protected:

	virtual void BeginPlay()override;

};