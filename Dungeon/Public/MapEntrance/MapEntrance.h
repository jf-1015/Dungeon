// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerStart.h"
#include "Interaction/InteractableInterface.h"//
#include "MapEntrance.generated.h"

class UBoxComponent;
class UWidgetComponent;

/**
 * 
 */
UCLASS()
class DUNGEON_API AMapEntrance : public AActor, public IInteractableInterface
{
	GENERATED_BODY()

public:

	AMapEntrance();

	/*Interactable Interface*/
	virtual bool StartInteractEvent_Implementation()override;
	virtual void SetInteractIconVisibility_Implementation(bool bInteractIcon)override;
	virtual bool IsNPC_Implementation()override;
	virtual int32 IsNeedItemToInteract_Implementation()override;
	/*Interactable Interface*/

	UPROPERTY(EditAnywhere)
	TSoftObjectPtr<UWorld>DestinationMap;

	UPROPERTY(BlueprintAssignable)
	FOnEventStart OnEventStart;

protected:

	UFUNCTION(BlueprintCallable)
	void MoveToAnotherMap(AActor* Player);

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UBoxComponent> OverlapBox;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UStaticMeshComponent>MapEntranceMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UWidgetComponent>InteractIcon;
};