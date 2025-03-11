// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interaction/InteractableInterface.h"
#include "NPCBase.generated.h"

class UWidgetComponent;
class USphereComponent;
class APickupItems;

USTRUCT(BlueprintType)
struct FItemsToShop
{
	GENERATED_BODY();

	/*Sell Item*/
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TSubclassOf<APickupItems> ItemClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 SlotIndex = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 NumToSell = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 NumPurchasedOnce = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* ItemImage = nullptr;

	/*Exchange Item*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* ExchangeItemImage = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 AmountRequiredToExchange = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 ExchangeItemIndex = 0;
};

DECLARE_MULTICAST_DELEGATE_OneParam(FUpdateShop, TArray<FItemsToShop>&);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FCloseShop);

UCLASS()
class DUNGEON_API ANPCBase : public ACharacter, public IInteractableInterface
{
	GENERATED_BODY()

public:

	ANPCBase();

	/*Interactable Interface*/
	virtual bool StartInteractEvent_Implementation()override;
	virtual void SetInteractIconVisibility_Implementation(bool bInteractIcon)override;
	virtual bool IsNPC_Implementation()override;
	virtual bool IsPlayerInRange_Implementation()override;
	/*Interactable Interface*/

	UPROPERTY(BlueprintAssignable)
	FOnEventStart OnEventStart;

	UPROPERTY(BlueprintAssignable)
	FCloseShop CloseShop;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<FItemsToShop>ItemsToSell;

	FUpdateShop UpdateShop;

	void SetItemNumBySlotIndex(int32 SlotIndex, int32 NumPurchased);

	int32 GetItemNumBySlotIndex(int32 SlotIndex);

	UFUNCTION(BlueprintCallable)
	AActor* GetPlayerInRange();

	void SpawnItemToWorld(TSubclassOf<APickupItems> ItemClass, int32 Num);

protected:

	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UWidgetComponent>InteractIcon;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<USphereComponent>InteractRange;

	UPROPERTY()
	TObjectPtr<AActor>PlayerInRange;

	UPROPERTY(BlueprintReadWrite)
	bool IsShopOpen = false;

	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};