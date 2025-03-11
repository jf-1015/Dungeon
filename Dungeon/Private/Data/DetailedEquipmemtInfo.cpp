// Fill out your copyright notice in the Description page of Project Settings.


#include "Data/DetailedEquipmemtInfo.h"

FWeaponInfo UDetailedEquipmemtInfo::GetWeaponInfoForItemIndex(int32 InIndex)
{
	for (const FWeaponInfo& Info : WeaponInfomation)
	{
		if (Info.WeaponItemIndex == InIndex)
		{
			return Info;
		}

	}
	return FWeaponInfo();
}