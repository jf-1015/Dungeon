// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"//
#include "DungeonGameplayTags.h"//
#include "Interaction/CharacterInterface.h"//
#include "Interaction/SaveInterface.h"//
#include "Data/CharacterClassInfo.h"//
#include "CharacterBase.generated.h"

class UAttributeSet;
class UAbilitySystemComponent;
class UBoxComponent;
class UGameplayEffect;
class UGameplayAbility;
struct FGameplayTag;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnWeaponTypeChange, FGameplayTag, WeaponType);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FWhenFallingEnd, float, FallingTime);

UCLASS()
class DUNGEON_API ACharacterBase : public ACharacter, public IAbilitySystemInterface, public ICharacterInterface, public ISaveInterface
{
	GENERATED_BODY()

public:
	
	ACharacterBase();

	UFUNCTION(BlueprintCallable)
	virtual UAbilitySystemComponent* GetAbilitySystemComponent()const override;
	UAttributeSet* GetAttributeSet()const { return CharacterAS; }

	UPROPERTY(EditDefaultsOnly, Category = "Abilities")
	TArray<TSubclassOf<UGameplayAbility>>StartupAbilities;

	UPROPERTY(EditDefaultsOnly, Category = "Abilities")
	TArray<TSubclassOf<UGameplayAbility>>PassiveAbilities;

	/*HitReactMontage*/
	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	TObjectPtr<UAnimMontage>HitReactMontage_Normal;

	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	TObjectPtr<UAnimMontage>HitReactMontage_Blocking;

	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	TObjectPtr<UAnimMontage>HitReactMontage_Long;
	/*HitReactMontage*/

	/*Character Interface*/
	virtual FGameplayTag GetCharacterTag_Implementation()override;
	virtual void SetCharacterTag_Implementation(FGameplayTag InTag)override;

	virtual UStaticMeshComponent* GetWeaponStaticMeshComponent_Implementation()override;
	virtual UStaticMeshComponent* GetSecondaryWeaponStaticMeshComponent_Implementation()override;

	virtual void SetEHandheldType_Implementation(EHandheldType InE_HandheldType)override;
	virtual EHandheldType GetEHandheldType_Implementation()override;

	virtual bool GetWeaponIsEmpty_Implementation()override;
	virtual bool GetSecondaryWeaponIsEmpty_Implementation()override;

	virtual bool IsDead_Implementation()override;

	virtual bool GetIsBlocking_Implementation()override;
	virtual void SetIsBlocking_Implementation(bool InBool)override;

	virtual bool GetIsSpinning_Implementation()override;
	virtual void SetIsSpinning_Implementation(bool InBool)override;

	virtual USkeletalMeshComponent* GetCharacterMesh_Implementation()override;

	virtual UAnimMontage* GetHitReactMontage_Implementation(int32 Index)override;

	virtual void BroadcastTraceCheckEndDelegate_Implementation()override;
	virtual void BroadcastBlockingEndDelegate_Implementation()override;
	virtual void BroadcastSpinningEndDelegate_Implementation()override;

	virtual void SetWeaponIsEmpty_Implementation(bool InBool)override;
	virtual void SetSecondaryWeaponIsEmpty_Implementation(bool InBool)override;

	virtual void SetPreVelocity_Implementation(FVector InVector)override;
	virtual FVector GetPreVelocity_Implementation()override;

	virtual void SetWeaponAttachment_Implementation(const FGameItemInfo& WeaponInfo)override;

	virtual FGameplayTag GetCurrentWeaponType_Implementation()override;
	virtual void SetCurrentWeaponType_Implementation(FGameplayTag InType)override;

	virtual void SetIsFalling_Implementation(bool InBool)override;
	virtual bool GetIsFalling_Implementation()override;

	virtual void Die_Implementation()override;

	virtual bool GetIsDodge_Implementation()override;
	/*Character Interface*/

	UPROPERTY(BlueprintAssignable)
	FOnTraceCheckEnd OnTraceCheckEnd;

	UPROPERTY(BlueprintAssignable)
	FOnBlockingEnd OnBlockingEnd;

	UPROPERTY(BlueprintAssignable)
	FOnBlockingEnd OnSpinningEnd;

	UPROPERTY(BlueprintAssignable)
	FOnWeaponTypeChange OnWeaponTypeChange;

	UPROPERTY(BlueprintAssignable)
	FWhenFallingEnd WhenFallingEnd;

protected:
	
	virtual void BeginPlay() override;

	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent>CharacterASC;
	UPROPERTY()
	TObjectPtr<UAttributeSet>CharacterAS;

	virtual void InitAbilityActorInfo();

	virtual void HitReactTagChanged(const FGameplayTag CallbackTag, int32 NewCount);
	virtual void RunTagChanged(const FGameplayTag CallbackTag, int32 NewCount);
	virtual void DodgeTagChanged(const FGameplayTag CallbackTag, int32 NewCount);

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	FGameplayTag CharacterTag = FGameplayTag();

	/*Handheld*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Equipment")
	TObjectPtr<UStaticMeshComponent>Weapon;

	UPROPERTY(BlueprintReadOnly, Category = "Handheld State")
	bool WeaponIsEmpty = true;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Equipment")
	TObjectPtr<UStaticMeshComponent>SecondaryWeapon;

	UPROPERTY(BlueprintReadOnly, Category = "Handheld State")
	bool SecondaryWeaponIsEmpty = true;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Equipment")
	TObjectPtr<UStaticMeshComponent>UnusedWeapon;

	UPROPERTY(BlueprintReadOnly, Category = "Handheld State")
	EHandheldType E_HandheldType = EHandheldType::None;

	UPROPERTY(BlueprintReadOnly, Category = "Handheld State")
	bool IsSpining = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Equipment")
	TObjectPtr<UStaticMeshComponent>Helmet;
	/*Handheld*/

	/*Attribute & Ability*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attribute")
	TSubclassOf<UGameplayEffect> BaseAttribute;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attribute")
	TSubclassOf<UGameplayEffect> DerivedAttribute;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attribute")
	TSubclassOf<UGameplayEffect> VitalAttribute;

	void AppliedEffectToSelf(TSubclassOf<UGameplayEffect> GameplayEffectClass);
	virtual void InitializeDefaultAttributes();

	void AddCharacterAbilities();
	/*Attribute & Ability*/

	/*Move*/
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Character Default")
	float BaseWalkSpeed = 100.f;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Character Default")
	float RunSpeed = 120.f;

	FVector PreVelocity = FVector();//To Dodge

	bool IsFalling = false;
	float FallingTime = 0.f;

	UFUNCTION(BlueprintCallable)
	void TickForFalling();

	UPROPERTY(EditDefaultsOnly, Category = "Character Default")
	TObjectPtr<UAnimMontage> LandMontage;

	UFUNCTION()
	void FallingEnd(float Time);
	/*Move*/

	/*Ingredients*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Default")
	ECharacterClass CharacterClass = ECharacterClass::Warrior;

	UPROPERTY(BlueprintReadOnly, SaveGame)
	bool bDead = false;

	FGameplayTag CurrentWeaponType = FGameplayTag();
	/*Ingredients*/
};