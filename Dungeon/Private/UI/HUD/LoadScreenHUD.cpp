// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HUD/LoadScreenHUD.h"
#include "Blueprint/UserWidget.h"//
#include "UI/Widget/LoadScreenWidget.h"//
#include "UI/ViewModel/Mvvm_LoadScreen.h"//

void ALoadScreenHUD::BeginPlay()
{
	Super::BeginPlay();

	LoadScreen = NewObject<UMvvm_LoadScreen>(this, LoadScreenClass);
	LoadScreen->InitializeLoadSlots();

	LoadScreenWidget = CreateWidget<ULoadScreenWidget>(GetWorld(), LoadScreenWidgetClass);
	LoadScreenWidget->AddToViewport();
	LoadScreenWidget->BlueprintInitializeWidget();

	LoadScreen->LoadData();
}