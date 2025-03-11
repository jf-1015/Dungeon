// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "PlayerHUD.generated.h"

class UAbilitySystemComponent;
class UAttributeSet;

class UPlayerUserWidget;

class UOverlayWidgetController;
class UAttributeMenuWidgetController;
class UBackpackWidgetController;
class UEquippedWidgetController;
class UShopWidgetController;

struct FWidgetControllerParams;

/**
 * 
 */
UCLASS()
class DUNGEON_API APlayerHUD : public AHUD
{
	GENERATED_BODY()

public:

	UOverlayWidgetController* GetOverlayWidgetController(FWidgetControllerParams& InParams);
	UAttributeMenuWidgetController* GetAttributeMenuWidgetController(FWidgetControllerParams& InParams);
	UBackpackWidgetController* GetBackpackWidgetController(FWidgetControllerParams& InParams);
	UEquippedWidgetController* GetEquippedWidgetController(FWidgetControllerParams& InParams);
	UShopWidgetController* GetShopWidgetController(AActor* InActor, FWidgetControllerParams& InParams);

	void InitOverlay(UAbilitySystemComponent* InASC, UAttributeSet* InAS, APlayerState* InPS, APlayerController* InPC);

private:

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UPlayerUserWidget>OverlayWidgetClass;

	UPROPERTY()
	TObjectPtr<UPlayerUserWidget>OverlayWidget;



	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UOverlayWidgetController>OverlayWidgetControllerClass;

	UPROPERTY()
	TObjectPtr<UOverlayWidgetController>OverlayWidgetController;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UAttributeMenuWidgetController>AttributeMenuWidgetControllerClass;

	UPROPERTY()
	TObjectPtr<UAttributeMenuWidgetController>AttributeMenuWidgetController;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UBackpackWidgetController>BackpackWidgetControllerClass;

	UPROPERTY()
	TObjectPtr<UBackpackWidgetController>BackpackWidgetController;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UEquippedWidgetController>EquippedWidgetControllerClass;

	UPROPERTY()
	TObjectPtr<UEquippedWidgetController>EquippedWidgetController;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UShopWidgetController>ShopWidgetControllerClass;

	UPROPERTY()
	TObjectPtr<UShopWidgetController>ShopWidgetController;
};