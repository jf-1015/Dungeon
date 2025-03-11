// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HUD/PlayerHUD.h"
#include "UI/Widget/PlayerUserWidget.h"//
#include "UI/WidgetController/OverlayWidgetController.h"//
#include "UI/WidgetController/AttributeMenuWidgetController.h"//
#include "UI/WidgetController/BackpackWidgetController.h"//
#include "UI/WidgetController/EquippedWidgetController.h"//
#include "UI/WidgetController/ShopWidgetController.h"//

UOverlayWidgetController* APlayerHUD::GetOverlayWidgetController(FWidgetControllerParams& InParams)
{
	if (OverlayWidgetController == nullptr)
	{
		OverlayWidgetController = NewObject<UOverlayWidgetController>(this, OverlayWidgetControllerClass);
		OverlayWidgetController->SetWidgetControllerParams(InParams);
		OverlayWidgetController->BindCallBacksToDependencies();
	}
	return OverlayWidgetController;
}

UAttributeMenuWidgetController* APlayerHUD::GetAttributeMenuWidgetController(FWidgetControllerParams& InParams)
{
	if (AttributeMenuWidgetController == nullptr)
	{
		AttributeMenuWidgetController = NewObject<UAttributeMenuWidgetController>(this, AttributeMenuWidgetControllerClass);
		AttributeMenuWidgetController->SetWidgetControllerParams(InParams);
		AttributeMenuWidgetController->BindCallBacksToDependencies();
	}
	return AttributeMenuWidgetController;
}

UBackpackWidgetController* APlayerHUD::GetBackpackWidgetController(FWidgetControllerParams& InParams)
{
	if (BackpackWidgetController == nullptr)
	{
		BackpackWidgetController = NewObject<UBackpackWidgetController>(this, BackpackWidgetControllerClass);
		BackpackWidgetController->SetWidgetControllerParams(InParams);
		BackpackWidgetController->BindCallBacksToDependencies();
	}
	return BackpackWidgetController;
}

UEquippedWidgetController* APlayerHUD::GetEquippedWidgetController(FWidgetControllerParams& InParams)
{
	if (EquippedWidgetController == nullptr)
	{
		EquippedWidgetController = NewObject<UEquippedWidgetController>(this, EquippedWidgetControllerClass);
		EquippedWidgetController->SetWidgetControllerParams(InParams);
		EquippedWidgetController->BindCallBacksToDependencies();
	}
	return EquippedWidgetController;
}

UShopWidgetController* APlayerHUD::GetShopWidgetController(AActor* InActor, FWidgetControllerParams& InParams)
{
	ShopWidgetController = NewObject<UShopWidgetController>(this, ShopWidgetControllerClass);
	ShopWidgetController->SetShoper(InActor);
	ShopWidgetController->SetWidgetControllerParams(InParams);
	ShopWidgetController->BindCallBacksToDependencies();
	return ShopWidgetController;
}

void APlayerHUD::InitOverlay(UAbilitySystemComponent* InASC, UAttributeSet* InAS, APlayerState* InPS, APlayerController* InPC)
{
	check(OverlayWidgetClass);
	check(OverlayWidgetControllerClass);

	UUserWidget* Widget = CreateWidget<UUserWidget>(GetWorld(), OverlayWidgetClass);

	FWidgetControllerParams WidgetControllerParams(InASC, InAS, InPS, InPC);
	UOverlayWidgetController* Controller = GetOverlayWidgetController(WidgetControllerParams);

	OverlayWidget = Cast<UPlayerUserWidget>(Widget);
	OverlayWidget->SetWidgetController(Controller);

	OverlayWidgetController->BroadcastInitialValues();

	OverlayWidget->AddToViewport();
}