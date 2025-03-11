// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Data/DungeonItemInfo.h"//
#include "PlayerInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UPlayerInterface : public UInterface
{
	GENERATED_BODY()
};

struct FGameItemInfo;
class AControlledPlayerState;

/**
 * 
 */
class DUNGEON_API IPlayerInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	/*GetEquipmentInfo*/
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	FGameItemInfo GetEquipedWeaponInfo();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	FGameItemInfo GetEquipedSecondaryWeaponInfo();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	FGameItemInfo GetEquipedConsumableInfo();

	/*GetUnusedWeaponSocket*/
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void SetUnusedSocket(UStaticMesh* InMesh);

	/*Check EquipCondition*/
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	bool CanEquip(FGameplayTag HandheldType);
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	bool CheckBackpackIsFull();

	/*IsAttack*/
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	bool GetIsAttacking();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void SetIsAttacking(bool InBool);

	/*IsLock*/
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	bool GetIsLocking();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void SetIsLocking(bool InBool);

	/*Handle Dodge Direction*/
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	float GetDirection();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void SetDirection(float InDirection);

	/*Add To Backpack*/
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void AddToBackpack(UPARAM(ref) FGameItemInfo& InItemInfo);

	/*GetCMC*/
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	UCharacterMovementComponent* GetCharacterMovementComponent();

	/*Handle Locking Spring Arm*/
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SetSpringArmOffset(float X, float Y, float Z);

	/*AllowRootMotion*/
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void AllowRootMotion(bool InBool);

	/*ChangeCurrentSlot*/
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void ChangeCurrentEquippedWeaponSlot(int32 Slot);
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void ChangeCurrentEquippedSecondaryWeaponSlot(int32 Slot);
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void ChangeCurrentEquippedConsumableSlot(int32 Slot);

	/*GetCurrentSlot*/
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	int32 GetCurrentEquippedWeaponSlot();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	int32 GetCurrentEquippedSecondaryWeaponSlot();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	int32 GetCurrentEquippedConsumableSlot();

	/*Expend Backpack Item*/
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void ExpendItem(int32 ItemIndex, int32 Num);
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	bool CheckCanAfford(int32 ItemIndex, int32 RequiredNum);

	/*IsTargetToItem(LineTrace)*/
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void SetIsTargetToItem(bool InBool);
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	bool GetIsTargetToItem();

	/*LineTrace*/
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void SetLineTrace(bool InBool);

	/*IsLoaded*/
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	bool GetIsLoaded();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void SetIsLoaded(bool InBool);
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	bool CanLoad();

	/*IsAiming*/
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	bool GetIsAiming();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void SetIsAiming(bool InBool);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void SetUseControllerRotationYaw(bool InBool);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	AActor* GetInteractableActor();

	/*Current Consumable*/
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void UseCurrentConsumable();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	bool IsCurrentConsumableEmpty();

	/*Handle InputMode With Widget*/
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void SetWidgetOpen(bool InBool, FName WidgetName);
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	bool GetIsWidgetOpen(FName WidgetName);

	/*Handle Throw Weapon*/
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	bool GetShowAuxSpline();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void SetShowAuxSpline(bool InBool);
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	FVector GetThrowVelocity();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	FTransform GetThrowPointTransform();
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void EndShowAuxSpline();
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SwitchThrowAngle();

	/*Save*/
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void SaveProgress();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void SaveAndQuit();

	/*Load To Another Map*/
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void LoadToMap();

	/*ConsumeFood*/
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void SetFoodConsumeRate(float InFloat);
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	float GetFoodConsumeRate();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	bool IsFoodEmpty();
};