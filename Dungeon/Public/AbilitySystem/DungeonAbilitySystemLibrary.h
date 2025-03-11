// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Data/CharacterClassInfo.h"//
#include "DungeonAbilitySystemLibrary.generated.h"

class UAbilitySystemComponent;
class UGameplayEffect;

class UAttributeMenuWidgetController;
class UBackpackWidgetController;
class UEquippedWidgetController;
class UShopWidgetController;

class UItemClass;
class UProjectileClass;
class UDetailedEquipmemtInfo;
class UEnemyEquipmentInfo;
class UCharacterClassInfo;

struct FDamageEffectParams;

/**
 * 
 */
UCLASS()
class DUNGEON_API UDungeonAbilitySystemLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	/*Widget Controller*/
	UFUNCTION(BlueprintPure, Category = "DungeonAbilitySystemLibrary|WidgetController", meta = (DefaultToSelf = "WorldContextObject"))
	static UAttributeMenuWidgetController* GetAttributeMenuWidgetController(const UObject* WorldContextObject);

	UFUNCTION(BlueprintPure, Category = "DungeonAbilitySystemLibrary|WidgetController", meta = (DefaultToSelf = "WorldContextObject"))
	static UBackpackWidgetController* GetBackpackWidgetController(const UObject* WorldContextObject);

	UFUNCTION(BlueprintPure, Category = "DungeonAbilitySystemLibrary|WidgetController", meta = (DefaultToSelf = "WorldContextObject"))
	static UEquippedWidgetController* GetEquippedWidgetController(const UObject* WorldContextObject);

	UFUNCTION(BlueprintPure, Category = "DungeonAbilitySystemLibrary|WidgetController", meta = (DefaultToSelf = "WorldContextObject"))
	static UShopWidgetController* GetShopWidgetController(const UObject* WorldContextObject, AActor* InActor);
	/*Widget Controller*/

	/*Effect Context Getters*/
	UFUNCTION(Blueprintpure, Category = "DungeonAbilitySystemLibrary|GameplayEffects")
	static bool IsBlocked(const FGameplayEffectContextHandle& EffectContextHandle);

	UFUNCTION(Blueprintpure, Category = "DungeonAbilitySystemLibrary|GameplayEffects")
	static bool ShoudlReactBlock(const FGameplayEffectContextHandle& EffectContextHandle);

	UFUNCTION(Blueprintpure, Category = "DungeonAbilitySystemLibrary|GameplayEffects")
	static bool IsThrowable(const FGameplayEffectContextHandle& EffectContextHandle);

	UFUNCTION(Blueprintpure, Category = "DungeonAbilitySystemLibrary|GameplayEffects")
	static float GetResilienceCut(const FGameplayEffectContextHandle& EffectContextHandle);

	UFUNCTION(Blueprintpure, Category = "DungeonAbilitySystemLibrary|GameplayEffects")
	static float GetEnduranceCut(const FGameplayEffectContextHandle& EffectContextHandle);
	/*Effect Context Getters*/

	/*Effect Context Setters*/
	UFUNCTION(BlueprintCallable, Category = "DungeonAbilitySystemLibrary|GameplayEffects")
	static void SetIsBlocked(FGameplayEffectContextHandle& EffectContextHandle, bool InBool);

	UFUNCTION(BlueprintCallable, Category = "DungeonAbilitySystemLibrary|GameplayEffects")
	static void SetReactBlock(FGameplayEffectContextHandle& EffectContextHandle, bool InBool);

	UFUNCTION(BlueprintCallable, Category = "DungeonAbilitySystemLibrary|GameplayEffects")
	static void SetIsThrowable(FGameplayEffectContextHandle& EffectContextHandle, bool InBool);

	UFUNCTION(BlueprintCallable, Category = "DungeonAbilitySystemLibrary|GameplayEffects")
	static void SetResilienceCut(FGameplayEffectContextHandle& EffectContextHandle, float InFloat);

	UFUNCTION(BlueprintCallable, Category = "DungeonAbilitySystemLibrary|GameplayEffects")
	static void SetEnduranceCut(FGameplayEffectContextHandle& EffectContextHandle, float InFloat);
	/*Effect Context Setters*/

	/*Infomation*/
	UFUNCTION(BlueprintCallable, Category = "DungeonAbilitySystemLibrary|Infomation", meta = (DefaultToSelf = "WorldContextObject"))
	static UDetailedEquipmemtInfo* GetEquipmentInfomation(const UObject* WorldContextObject);

	UFUNCTION(BlueprintCallable, Category = "DungeonAbilitySystemLibrary|Infomation", meta = (DefaultToSelf = "WorldContextObject"))
	static UEnemyEquipmentInfo* GetEnemyEquipmentInfomation(const UObject* WorldContextObject);

	UFUNCTION(BlueprintCallable, Category = "DungeonAbilitySystemLibrary|Infomation", meta = (DefaultToSelf = "WorldContextObject"))
	static UCharacterClassInfo* GetCharacterClassInfomation(const UObject* WorldContextObject);

	UFUNCTION(BlueprintCallable, Category = "DungeonAbilitySystemLibrary|Infomation", meta = (DefaultToSelf = "WorldContextObject"))
	static UItemClass* GetItemClass(const UObject* WorldContextObject);

	UFUNCTION(BlueprintCallable, Category = "DungeonAbilitySystemLibrary|Infomation", meta = (DefaultToSelf = "WorldContextObject"))
	static UProjectileClass* GetProjectileClass(const UObject* WorldContextObject);
	/*Infomation*/

	/*Character*/
	UFUNCTION(BlueprintCallable, Category = "DungeonAbilitySystemLibrary|Character", meta = (DefaultToSelf = "WorldContextObject"))
	static void InitialCharacterAttribute(const UObject* WorldContextObject, 
		ECharacterClass InClass, float Level, UAbilitySystemComponent* InASC, 
		bool bVitalIsSet, FSavedEnemyVitalAttribute InSavedAttribute);

	UFUNCTION(BlueprintCallable, Category = "DungeonAbilitySystemLibrary|Character", meta = (DefaultToSelf = "WorldContextObject"))
	static void GiveDefaultAbilities(const UObject* WorldContextObject, ECharacterClass InClass, UAbilitySystemComponent* InASC);
	/*Character*/

	/*Combat*/
	UFUNCTION(BlueprintCallable, Category = "DungeonAbilitySystemLibrary|Combat", meta = (DefaultToSelf = "WorldContextObject"))
	static void GetLiveCharactersInRadiusByTag(const UObject* WorldContextObject,
		TArray<AActor*>& OutActors,
		const TArray<AActor*>& ActorsToIgnore,
		float Radius,
		const FVector SphereOrigin,
		const FGameplayTag TargetTag);

	UFUNCTION(BlueprintCallable, Category = "DungeonAbilitySystemLibrary|Combat", meta = (DefaultToSelf = "WorldContextObject"))
	static void GetClosestCharacterInSightByTag(const UObject* WorldContextObject,
		AActor*& OutActor,
		const TArray<AActor*>& ActorsToIgnore,
		float Sight,
		const FVector Origin,
		const FGameplayTag TargetTag);

	UFUNCTION(BlueprintCallable, Category = "DungeonAbilitySystemLibrary|Combat")
	static FGameplayEffectContextHandle ApplyDamageEffect(const FDamageEffectParams& Params);
	/*Combat*/
};