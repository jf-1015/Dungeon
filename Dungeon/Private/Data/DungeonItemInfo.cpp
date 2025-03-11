// Fill out your copyright notice in the Description page of Project Settings.


#include "Data/DungeonItemInfo.h"

FGameItemInfo UDungeonItemInfo::GetItemInfoByItemIndex(int32 Index)
{
    for (FGameItemInfo& Info : GameItemInfomation)
    {
        if (Info.ItemIndex == Index)
        {
            return Info;
        }
    }
    return FGameItemInfo();
}