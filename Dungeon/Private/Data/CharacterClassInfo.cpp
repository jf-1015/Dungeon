// Fill out your copyright notice in the Description page of Project Settings.


#include "Data/CharacterClassInfo.h"

FSPCharacterInfo UCharacterClassInfo::FindSPInfoForCharacterClass(ECharacterClass InCharacterClass)
{
	return CharacterClassToSPInfo.FindChecked(InCharacterClass);
}