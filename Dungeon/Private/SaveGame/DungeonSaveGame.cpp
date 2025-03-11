// Fill out your copyright notice in the Description page of Project Settings.


#include "SaveGame/DungeonSaveGame.h"

FSavedMap UDungeonSaveGame::GetSavedMapByMapAssetName(const FString& InMapAssetName)
{
	for (FSavedMap& SavedMap : SavedMaps)
	{
		if (SavedMap.MapAssetName == InMapAssetName)
		{
			return SavedMap;
		}
	}
	return FSavedMap();
}

bool UDungeonSaveGame::HasMap(const FString& InMapAssetName)
{
	for (FSavedMap& SavedMap : SavedMaps)
	{
		if (SavedMap.MapAssetName == InMapAssetName)
		{
			return true;
		}
	}
	return false;
}