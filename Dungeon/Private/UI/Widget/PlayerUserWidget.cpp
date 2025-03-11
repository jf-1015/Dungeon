// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widget/PlayerUserWidget.h"

void UPlayerUserWidget::SetWidgetController(UObject* InWidgetController)
{
	WidgetController = InWidgetController;
	WidgetControllerSet();
}

int32 UPlayerUserWidget::GetFullIconNums(float InValue)
{
	return InValue / FullIconValue;
}

float UPlayerUserWidget::GetLastIconPercent(float InValue)
{
	return (InValue - FullIconValue * GetFullIconNums(InValue)) / FullIconValue;
}