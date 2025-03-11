// Fill out your copyright notice in the Description page of Project Settings.


#include "Data/ProjectileClass.h"

TSubclassOf<ADungeonProjectile> UProjectileClass::GetSubclassByItemIndex(int32 Index)
{
	for (FIndexToProjectile& Info : ItemProjectiles)
	{
		if (Info.ItemIndex == Index)
		{
			return Info.ProjectileClass;
		}
	}
	return TSubclassOf<ADungeonProjectile>();
}