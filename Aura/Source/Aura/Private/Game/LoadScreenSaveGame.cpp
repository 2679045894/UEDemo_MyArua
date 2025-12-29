// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/LoadScreenSaveGame.h"

FSaveMap ULoadScreenSaveGame::GetSaveMapWithMapName(const FString& InMapName)
{
	for (FSaveMap SavedMap:SavedMaps)
	{
		if (SavedMap.MapAssetName==InMapName)
		{
			return SavedMap;
		}
	}
	return FSaveMap();
}

bool ULoadScreenSaveGame::HasMap(const FString& InMapName)
{
	for (FSaveMap SavedMap:SavedMaps)
	{
		if (SavedMap.MapAssetName==InMapName)
		{
			return true;
		}
	}
	return false;
}
