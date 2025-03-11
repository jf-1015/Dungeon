// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interaction/PickupInterface.h"//
#include "GameplayTagContainer.h"//
#include "Data/DungeonItemInfo.h"//
#include "PickupItems.generated.h"

class UBoxComponent;
class UWidgetComponent;
class UGameplayEffect;

UCLASS()
class DUNGEON_API APickupItems : public AActor, public IPickupInterface
{
	GENERATED_BODY()
	
public:	
	
	APickupItems();

	/*Pickup Interface*/
	virtual void SetInteractIconVisibility_Implementation(bool bInteractIcon)override;
	/*Pickup Interface*/

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pickup Info")
	FGameItemInfo ItemInfo;

	void SetMesh(UStaticMesh* InMesh)
	{
		StaticMeshComponent->SetStaticMesh(InMesh);
	}

	UStaticMesh* GetMesh()
	{
		return StaticMeshComponent->GetStaticMesh();
	}

protected:
	
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent>StaticMeshComponent;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UBoxComponent> OverlapBox;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UWidgetComponent>PickupIcon;

private:

	UFUNCTION()
	void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

};