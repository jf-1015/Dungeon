// Fill out your copyright notice in the Description page of Project Settings.


#include "MyAssetManager.h"
#include "DungeonGameplayTags.h"//
#include "AbilitySystemGlobals.h"//

UMyAssetManager& UMyAssetManager::Get()
{
	check(GEngine);
	UMyAssetManager* MyAssetManager = Cast<UMyAssetManager>(GEngine->AssetManager);
	return *MyAssetManager;
}

void UMyAssetManager::StartInitialLoading()
{
	Super::StartInitialLoading();

	FDungeonGameplayTags::InitializeNativeGameplayTags();
	UAbilitySystemGlobals::Get().InitGlobalData();
}