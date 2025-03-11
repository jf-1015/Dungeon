// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interaction/InteractableInterface.h"//
#include "Interaction/SaveInterface.h"//
#include "InteractableActor.generated.h"

class USceneComponent;
class UBoxComponent;
class UWidgetComponent;

class APickupItems;

UCLASS()
class DUNGEON_API AInteractableActor : public AActor, public IInteractableInterface, public ISaveInterface
{
	GENERATED_BODY()
	
public:

	AInteractableActor();

	/*Interactable Interface*/
	virtual bool StartInteractEvent_Implementation()override;
	virtual void SetInteractIconVisibility_Implementation(bool bInteractIcon)override;
	virtual bool IsNPC_Implementation()override;
	virtual int32 IsNeedItemToInteract_Implementation()override;
	/*Interactable Interface*/

	/*Save Interface*/
	virtual void LoadActor_Implementation()override;
	/*Save Interface*/

	UPROPERTY(BlueprintAssignable)
	FOnEventStart OnEventStart;

protected:

	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UStaticMeshComponent>StaticMeshComponent;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UBoxComponent> OverlapBox;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UWidgetComponent>InteractIcon;

	UPROPERTY(BlueprintReadWrite, SaveGame)
	bool InteractBool = false;

	UPROPERTY(EditDefaultsOnly, Category = "Base")
	bool CanTriggeredAgain = true;

	UPROPERTY(EditDefaultsOnly, Category = "Base")
	int32 NeededItemIndex = -1;
};