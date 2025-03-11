// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgetController/PlayerWidgetController.h"


void UPlayerWidgetController::SetWidgetControllerParams(FWidgetControllerParams& InParams)
{
	AbilitySystemComponent = InParams.AbilitySystemComponent;
	AttributeSet = InParams.AttributeSet;
	PlayerState = InParams.PlayerState;
	PlayerController = InParams.PlayerController;
}

void UPlayerWidgetController::BroadcastInitialValues()
{
}

void UPlayerWidgetController::BindCallBacksToDependencies()
{
}