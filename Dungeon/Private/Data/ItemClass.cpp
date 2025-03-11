// Fill out your copyright notice in the Description page of Project Settings.


#include "Data/ItemClass.h"

TSubclassOf<APickupItems> UItemClass::GetSubclassByItemIndex(int32 Index)
{
	for (FIndexToItem& Info : Items)
	{
		if (Info.ItemIndex == Index)
		{
			return Info.ItemClass;
		}
	}
	return TSubclassOf<APickupItems>();
}