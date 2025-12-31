// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Data/LootTiers.h"

TArray<FLootItem> ULootTiers::GetLoopItem()
{
	TArray<FLootItem> ReturnItems;
	for (auto& LootItem : LootItems)
	{
		for (int32 i=0;i<LootItem.MaxNumberToSpawn;++i)
		{
			if (FMath::RandRange(0.f,100.f)<LootItem.ChanceToSpawn)
			{
				FLootItem NewLootItem;
				NewLootItem.LootClass=LootItem.LootClass;
				NewLootItem.bLootLevelOverride=LootItem.bLootLevelOverride;
				ReturnItems.Add(NewLootItem);
			}
		}
	}
	return ReturnItems;
}
