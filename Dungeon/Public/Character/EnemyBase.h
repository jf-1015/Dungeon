// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/CharacterBase.h"
#include "Interaction/EnemyInterface.h"//
#include "Data/DungeonItemInfo.h"//
#include "Data/ItemClass.h"//
#include "EnemyBase.generated.h"

class UBehaviorTree;
class AEnemyAIController;

class USphereComponent;

class APickupItems;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAwake);

/**
 * 
 */
UCLASS()
class DUNGEON_API AEnemyBase : public ACharacterBase, public IEnemyInterface
{
	GENERATED_BODY()

public:

	AEnemyBase();
	virtual void PossessedBy(AController* NewController)override;

	/*Save Interface*/
	virtual void LoadActor_Implementation()override;
	virtual bool ShouldLoadTransform_Implementation()override;
	/*Save Interface*/

	/*Enemy Interface*/
	virtual void SetEnemyHandheldItem_Implementation(const FGameItemInfo& InInfo, bool IsSecondary)override;

	virtual APickupItems* FindNearestWeapon_Implementation(float Radius)override;
	virtual APickupItems* FindNearestConsumable_Implementation(float Radius, int32 ItemIndex)override;

	virtual TMap<int32, UAnimMontage*>GetCountToMontageByHandheldType_Implementation()override;
	virtual UAnimMontage* GetShootMontage_Implementation(bool InBool)override;
	virtual UAnimMontage* GetReloadMontage_Implementation()override;
	virtual UAnimMontage* GetPickupMontage_Implementation()override;

	virtual void SetIsAwake_Implementation(bool InBool)override;
	virtual bool GetIsAwake_Implementation()override;
	virtual UAnimMontage* GetAwakeMontage_Implementation(int32 InIndex)override;

	virtual bool GetInactiveState_Implementation()override;

	virtual bool GetIsLoaded_Implementation()override;
	virtual void SetIsLoaded_Implementation(bool InBool)override;
	virtual bool CheckAmmunitionIsEnough_Implementation()override;
	virtual void SetAmountOfAmmunition_Implementation(int32 InNum)override;

	virtual void DicardCurrentWeapon_Implementation(bool bSecondary)override;

	virtual void SetWeaponInfo_Implementation(const FGameItemInfo& InInfo)override;
	virtual FGameItemInfo GetWeaponInfo_Implementation()override;

	virtual void SetSecondaryWeaponInfo_Implementation(const FGameItemInfo& InInfo)override;
	virtual FGameItemInfo GetSecondaryWeaponInfo_Implementation()override;
	/*Enemy Interface*/

	/*Character Interface*/
	virtual void SetTargetActor_Implementation(AActor* InActor)override;
	virtual AActor* GetTargetActor_Implementation()override;

	virtual int32 GetWeaponIndex_Implementation()override;
	virtual int32 GetSecondaryWeaponIndex_Implementation()override;

	virtual void SetCurrentWeaponType_Implementation(FGameplayTag InType)override;

	virtual FGameplayTag GetSecondaryWeaponHandheldType_Implementation()override;

	virtual void GiveWeaponAbilityByTag_Implementation(const FGameItemInfo& Info, FGameplayTag InputTag, bool bClear)override;

	virtual bool GetIsBlocking_Implementation()override;
	virtual void SetIsBlocking_Implementation(bool InBool)override;

	virtual bool GetIsInterrupt_Implementation()override;
	virtual void SetIsInterrupt_Implementation(bool InBool)override;

	virtual void Die_Implementation()override;

	virtual void SetDurability_Implementation(float InNum, bool bSecondary)override;
	virtual float GetDurability_Implementation(bool bSecondary)override;

	virtual void SaveEnemyVitalAttribute_Implementation(FSavedEnemyVitalAttribute& InAttributeInfo)override;
	virtual void LoadAttribute_Implementation(FSavedEnemyVitalAttribute InAttributeInfo)override;
	/*Character Interface*/

	UPROPERTY(BlueprintReadWrite, Category = "Combat")
	TObjectPtr<AActor>TargetActor;

	APickupItems* FilterItemByTag(TArray<FOverlapResult>& OverlapResults, FGameplayTag FilterTag_1, FGameplayTag FilterTag_2 = FGameplayTag());

	UPROPERTY(BlueprintAssignable)
	FOnAwake OnAwakeDelegate;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UItemClass>ItemClass;

	void InitializeHandheld();

protected:

	virtual void BeginPlay()override;

	virtual void InitAbilityActorInfo() override;

	virtual void HitReactTagChanged(const FGameplayTag CallbackTag, int32 NewCount)override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Default")
	int32 Level = 1;

	/*Handhled*/
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Character Default", SaveGame)
	FGameItemInfo WeaponInfo = FGameItemInfo();

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Character Default", SaveGame)
	FGameItemInfo SecondaryWeaponInfo = FGameItemInfo();

	UPROPERTY(BlueprintReadOnly)
	bool IsBlocking = false;

	UPROPERTY(BlueprintReadOnly)
	bool IsInterrupt = false;

	UPROPERTY(BlueprintReadOnly)
	bool IsGun = false;/*If is RangeWeapon*/
	/*Handhled*/

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Default", SaveGame)
	int32 AmountOfArrows = 0;

	/*Awake*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Default")
	bool IsInactive_Floor = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Default", SaveGame)
	bool IsAwake = false;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USphereComponent>AwakeRange;

	UFUNCTION()
	void AwakeRangeEvent(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	/*Awake*/

	/*AI*/
	UPROPERTY(EditAnywhere, Category = "AI")
	TObjectPtr<UBehaviorTree>BehaviorTree;

	UPROPERTY()
	TObjectPtr<AEnemyAIController>EnemyAIController;
	/*AI*/

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character Default")
	FEquipmentAbilitiesAndMontages EquipmentAbilitiesAndMontages;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character Default")
	TArray<UAnimMontage*> AwakeMontages_Floor;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character Default")
	UAnimMontage* AwakeMontage_Stand;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character Default")
	UAnimMontage* PickupMontage;

	bool bVitalIsSet = false;

	FSavedEnemyVitalAttribute SavedVitalAttribute;
};