// Fill out your copyright notice in the Description page of Project Settings.


#include "Data/DungeonAttributeInfo.h"

FAttributeInfo UDungeonAttributeInfo::FindAttributeInfoForTag(const FGameplayTag& AttributeTag, bool bLogNotFound) const
{
	for (FAttributeInfo Info : AttributeInfomation)
	{
		if (Info.AttributeTag == AttributeTag)
		{
			return Info;
		}
	}

	if (bLogNotFound)
	{
		UE_LOG(LogTemp, Error, TEXT("Can't find AttributeInfo for AttributeTag [%s], on AttributeInfomation [%s]"), *AttributeTag.ToString(), *GetNameSafe(this));
	}

	return FAttributeInfo();
}