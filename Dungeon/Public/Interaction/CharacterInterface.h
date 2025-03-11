// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "CharacterInterface.generated.h"

struct FGameplayTag;
struct FGameItemInfo;

UENUM(BlueprintType)
enum class EHandheldType :uint8
{
	None,
	OneHand,
	TwoHands
};

USTRUCT(BlueprintType)
struct FSavedEnemyVitalAttribute
{
	GENERATED_BODY();

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	float Health = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CurrentResilience = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Energy = 0.f;
};

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UCharacterInterface : public UInterface
{
	GENERATED_BODY()
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnTraceCheckEnd);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnBlockingEnd);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnSpinningEnd);

/**
 * 
 */
class DUNGEON_API ICharacterInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void SetTargetActor(AActor* InActor);
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	AActor* GetTargetActor();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	FGameplayTag GetCharacterTag();
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void SetCharacterTag(FGameplayTag InTag);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	UStaticMeshComponent* GetWeaponStaticMeshComponent();
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	UStaticMeshComponent* GetSecondaryWeaponStaticMeshComponent();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	FGameplayTag GetSecondaryWeaponHandheldType();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void SetHandheldStaticMeshByType(UStaticMesh* InMesh, FGameplayTag HandheldType);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void SetEHandheldType(EHandheldType InE_HandheldType);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	EHandheldType GetEHandheldType();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void SetWeaponIsEmpty(bool InBool);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void SetSecondaryWeaponIsEmpty(bool InBool);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	int32 GetWeaponIndex();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	int32 GetSecondaryWeaponIndex();

	UFUNCTION(BlueprintCallable,BlueprintNativeEvent)
	bool GetWeaponIsEmpty();
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	bool GetSecondaryWeaponIsEmpty();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	bool IsDead();
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void SetIsDead(bool InBool);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	bool GetIsBlocking();
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void SetIsBlocking(bool InBool);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	bool GetIsInterrupt();
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void SetIsInterrupt(bool InBool);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	bool GetIsSpinning();
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void SetIsSpinning(bool InBool);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	bool Use(UPARAM(ref) FGameItemInfo& Info);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	USkeletalMeshComponent* GetCharacterMesh();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	UAnimMontage* GetHitReactMontage(int32 Index);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void BroadcastTraceCheckEndDelegate();
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void BroadcastBlockingEndDelegate();
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void BroadcastSpinningEndDelegate();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void SetPreVelocity(FVector InVector);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	FVector GetPreVelocity();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void SetWeaponAttachment(const FGameItemInfo& WeaponInfo);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	FGameplayTag GetCurrentWeaponType();
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void SetCurrentWeaponType(FGameplayTag InType);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void SetIsFalling(bool InBool);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	bool GetIsFalling();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void GiveWeaponAbilityByTag(const FGameItemInfo& Info, FGameplayTag InputTag, bool bClear);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void Die();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	bool GetIsDodge();

	/*Durability*/
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void SetDurability(float InNum, bool bSecondary);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	float GetDurability(bool bSecondary);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void SaveEnemyVitalAttribute(FSavedEnemyVitalAttribute& InAttributeInfo);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void LoadAttribute(FSavedEnemyVitalAttribute InAttributeInfo);
};