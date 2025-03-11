// Fill out your copyright notice in the Description page of Project Settings.


#include "Data/EnemyEquipmentInfo.h"

FEnemyWeaponInfo UEnemyEquipmentInfo::GetEnemyWeaponInfoForItemIndex(int32 InIndex)
{
	for (const FEnemyWeaponInfo& Info : EnemyWeaponInfomation)
	{
		if (Info.WeaponItemIndex == InIndex)
		{
			return Info;
		}
	}

	return FEnemyWeaponInfo();
}