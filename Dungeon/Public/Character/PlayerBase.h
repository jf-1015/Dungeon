// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/CharacterBase.h"
#include "Interaction/PlayerInterface.h"//
#include "PlayerBase.generated.h"

struct FGameItemInfo;

class UCameraComponent;
class USpringArmComponent;

class USplineComponent;

/**
 * 
 */
UCLASS()
class DUNGEON_API APlayerBase : public ACharacterBase, public IPlayerInterface
{
	GENERATED_BODY()

public:

	APlayerBase();
	virtual void PossessedBy(AController* NewController) override;

	/*Save Interface*/
	virtual bool ShouldLoadTransform_Implementation()override;
	/*Save Interface*/

	/*Player Interface*/
	virtual FGameItemInfo GetEquipedWeaponInfo_Implementation()override;
	virtual FGameItemInfo GetEquipedSecondaryWeaponInfo_Implementation()override;
	virtual FGameItemInfo GetEquipedConsumableInfo_Implementation()override;

	virtual bool GetIsAttacking_Implementation()override;
	virtual void SetIsAttacking_Implementation(bool InBool)override;

	virtual bool GetIsLocking_Implementation()override;
	virtual void SetIsLocking_Implementation(bool InBool)override;

	virtual float GetDirection_Implementation()override;
	virtual void SetDirection_Implementation(float InDirection)override;

	virtual void AddToBackpack_Implementation(UPARAM(ref) FGameItemInfo& InItemInfo)override;

	virtual UCharacterMovementComponent* GetCharacterMovementComponent_Implementation()override;

	virtual bool CanEquip_Implementation(FGameplayTag HandheldType)override;
	virtual bool CheckBackpackIsFull_Implementation()override;

	virtual void SetUnusedSocket_Implementation(UStaticMesh* InMesh)override;

	virtual void ChangeCurrentEquippedWeaponSlot_Implementation(int32 Slot)override;
	virtual void ChangeCurrentEquippedSecondaryWeaponSlot_Implementation(int32 Slot)override;
	virtual void ChangeCurrentEquippedConsumableSlot_Implementation(int32 Slot)override;

	virtual int32 GetCurrentEquippedWeaponSlot_Implementation()override;
	virtual int32 GetCurrentEquippedSecondaryWeaponSlot_Implementation()override;
	virtual int32 GetCurrentEquippedConsumableSlot_Implementation()override;

	virtual void ExpendItem_Implementation(int32 ItemIndex, int32 Num)override;
	virtual bool CheckCanAfford_Implementation(int32 ItemIndex, int32 RequiredNum)override;

	virtual void SetIsTargetToItem_Implementation(bool InBool)override;
	virtual bool GetIsTargetToItem_Implementation()override;

	virtual void SetLineTrace_Implementation(bool InBool)override;

	virtual bool GetIsLoaded_Implementation()override;
	virtual void SetIsLoaded_Implementation(bool InBool)override;
	virtual bool CanLoad_Implementation()override;

	virtual bool GetIsAiming_Implementation()override;
	virtual void SetIsAiming_Implementation(bool InBool)override;

	virtual void SetUseControllerRotationYaw_Implementation(bool InBool)override;

	virtual AActor* GetInteractableActor_Implementation()override;

	virtual void UseCurrentConsumable_Implementation()override;
	virtual bool IsCurrentConsumableEmpty_Implementation()override;

	virtual void SetWidgetOpen_Implementation(bool InBool, FName WidgetName)override;
	virtual bool GetIsWidgetOpen_Implementation(FName WidgetName)override;

	virtual bool GetShowAuxSpline_Implementation()override;
	virtual void SetShowAuxSpline_Implementation(bool InBool)override;

	virtual FVector GetThrowVelocity_Implementation()override;
	virtual FTransform GetThrowPointTransform_Implementation()override;

	virtual void SaveProgress_Implementation()override;
	virtual void SaveAndQuit_Implementation()override;
	virtual void LoadToMap_Implementation()override;

	virtual void SetFoodConsumeRate_Implementation(float InFloat)override;
	virtual float GetFoodConsumeRate_Implementation()override;

	virtual bool IsFoodEmpty_Implementation()override;
	/*Player Interface*/

	/*Character Interface*/
	virtual void SetTargetActor_Implementation(AActor* InActor)override;
	virtual AActor* GetTargetActor_Implementation()override;

	virtual bool Use_Implementation(UPARAM(ref) FGameItemInfo& Info)override;

	virtual void SetHandheldStaticMeshByType_Implementation(UStaticMesh* InMesh, FGameplayTag HandheldType)override;

	virtual int32 GetWeaponIndex_Implementation()override;
	virtual int32 GetSecondaryWeaponIndex_Implementation()override;

	virtual FGameplayTag GetSecondaryWeaponHandheldType_Implementation()override;

	virtual void GiveWeaponAbilityByTag_Implementation(const FGameItemInfo& Info, FGameplayTag InputTag, bool bClear)override;

	virtual void Die_Implementation()override;

	virtual void SetDurability_Implementation(float InNum, bool bSecondary)override;
	virtual float GetDurability_Implementation(bool bSecondary)override;
	/*Character Interface*/

	UFUNCTION(BlueprintCallable)
	void HandleDie();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void Die_BlueprintImplement();

	UFUNCTION()
	void WeaponTypeChanged(FGameplayTag WeaponType);

	void LoadProgress();
	void InitializeItems(FGameItemInfo& ItemInfo, UDungeonSaveGame* SaveData);

protected:

	/*Throw*/
	UFUNCTION(BlueprintCallable)
	int32 TickForThrow(FVector StartPosition, FVector Velocity);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UStaticMeshComponent>ThrowPoint;

	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<USplineComponent>Spline;

	UPROPERTY(BlueprintReadOnly)
	bool bShowAuxSpline = false;

	UPROPERTY(BlueprintReadWrite)
	bool IsLowerAngle = false;
	/*Throw*/

	/*Camera*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UCameraComponent> Camera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<USpringArmComponent> CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<USpringArmComponent> AimingBoom;
	/*Camera*/

	/*Combat Status*/
	UPROPERTY(BlueprintReadOnly)
	bool IsAttacking = false;

	UPROPERTY(BlueprintReadOnly)
	bool IsLocking = false;

	bool IsAiming = false;
	/*Combat Status*/

	/*ActionConsume*/
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	float FoodConsumeRate = 0.05;
	/*ActionConsume*/

	UPROPERTY(BlueprintReadOnly)
	float Direction = 0.f;

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<AActor>LockTarget;

	virtual void InitAbilityActorInfo() override;
	virtual void InitializeDefaultAttributes() override;

	virtual void HitReactTagChanged(const FGameplayTag CallbackTag, int32 NewCount)override;
	virtual void RunTagChanged(const FGameplayTag CallbackTag, int32 NewCount)override;

	bool bFirstLoadIn = true;
};