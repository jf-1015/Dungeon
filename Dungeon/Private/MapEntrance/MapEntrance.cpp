// Fill out your copyright notice in the Description page of Project Settings.


#include "MapEntrance/MapEntrance.h"
#include "Components/BoxComponent.h"//
#include "Components/WidgetComponent.h"//
#include "Interaction/PlayerInterface.h"//
#include "GameMode/GlobalGameModeBase.h"//
#include "Kismet/GameplayStatics.h"//

AMapEntrance::AMapEntrance() 
{
	PrimaryActorTick.bCanEverTick = false;

	MapEntranceMesh = CreateDefaultSubobject<UStaticMeshComponent>("MapEntranceMesh");
	MapEntranceMesh->SetupAttachment(GetRootComponent());

	OverlapBox = CreateDefaultSubobject<UBoxComponent>("Box");
	OverlapBox->SetupAttachment(MapEntranceMesh);

	InteractIcon = CreateDefaultSubobject<UWidgetComponent>("InteractIcon");
	InteractIcon->SetupAttachment(MapEntranceMesh);
}

bool AMapEntrance::StartInteractEvent_Implementation()
{
	OnEventStart.Broadcast();
	return true;
}

void AMapEntrance::SetInteractIconVisibility_Implementation(bool bInteractIcon)
{
	if (IsValid(InteractIcon))
	{
		InteractIcon->SetVisibility(bInteractIcon);
	}
}

bool AMapEntrance::IsNPC_Implementation()
{
	return false;
}

int32 AMapEntrance::IsNeedItemToInteract_Implementation()
{
	return -1;
}

void AMapEntrance::MoveToAnotherMap(AActor* Player)
{
	if (AGlobalGameModeBase* GM = Cast<AGlobalGameModeBase>(UGameplayStatics::GetGameMode(this)))
	{
		IPlayerInterface::Execute_SaveProgress(Player);

		FString MapAssetName = DestinationMap.ToSoftObjectPath().GetAssetName();
		GM->SaveWorldState(GetWorld(), MapAssetName);

		UGameplayStatics::OpenLevelBySoftObjectPtr(this, DestinationMap);
	}
}