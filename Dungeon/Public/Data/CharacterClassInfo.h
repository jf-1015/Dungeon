// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "ScalableFloat.h"//
#include "CharacterClassInfo.generated.h"

class UGameplayEffect;
class UGameplayAbility;

UENUM(BlueprintType)
enum class ECharacterClass :uint8
{
	Warrior,
	Ranger,
	Mage,
	Necromancer,
	Golem
};

USTRUCT(BlueprintType)
struct FSPCharacterInfo
{
	GENERATED_BODY();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UGameplayEffect>BaseAttribute;

};

/**
 * 
 */
UCLASS()
class DUNGEON_API UCharacterClassInfo : public UDataAsset
{
	GENERATED_BODY()

public:


	UPROPERTY(EditDefaultsOnly, Category = "Character Class SP")
	TMap<ECharacterClass, FSPCharacterInfo> CharacterClassToSPInfo;

	UPROPERTY(EditDefaultsOnly, Category = "Character Class Default")
	TSubclassOf<UGameplayEffect>BaseAttribute_SetByCaller;

	UPROPERTY(EditDefaultsOnly, Category = "Character Class Default")
	TSubclassOf<UGameplayEffect>BaseDerivedAttribute;

	UPROPERTY(EditDefaultsOnly, Category = "Character Class Default")
	TSubclassOf<UGameplayEffect>VitalAttribute;

	UPROPERTY(EditDefaultsOnly, Category = "Character Class Default")
	TSubclassOf<UGameplayEffect>VitalAttribute_SetByCaller;

	UPROPERTY(EditDefaultsOnly, Category = "Character Class Default")
	TArray<TSubclassOf<UGameplayAbility>>CommonAbilities;

	FSPCharacterInfo FindSPInfoForCharacterClass(ECharacterClass InCharacterClass);
	
};