// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/InteractableActor.h"
#include "Components/BoxComponent.h"//
#include "Components/WidgetComponent.h"//
#include "Components/SceneComponent.h"//
#include "Actor/PickupItems.h"//

AInteractableActor::AInteractableActor()
{
	PrimaryActorTick.bCanEverTick = false;

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("StaicMeshComponent");
	StaticMeshComponent->SetupAttachment(GetRootComponent());

	OverlapBox = CreateDefaultSubobject<UBoxComponent>("Box");
	OverlapBox->SetupAttachment(StaticMeshComponent);

	InteractIcon = CreateDefaultSubobject<UWidgetComponent>("InteractIcon");
	InteractIcon->SetupAttachment(StaticMeshComponent);
}

bool AInteractableActor::StartInteractEvent_Implementation()
{
	if (InteractBool)
	{
		if (!CanTriggeredAgain)return false;
	}

	OnEventStart.Broadcast();
	return true;
}

void AInteractableActor::SetInteractIconVisibility_Implementation(bool bInteractIcon)
{
	if (IsValid(InteractIcon))
	{
		InteractIcon->SetVisibility(bInteractIcon);
	}
}

bool AInteractableActor::IsNPC_Implementation()
{
	return false;
}

int32 AInteractableActor::IsNeedItemToInteract_Implementation()
{
	return NeededItemIndex;
}

void AInteractableActor::LoadActor_Implementation()
{
	ISaveInterface::Execute_InitializeStatus(this);
}

void AInteractableActor::BeginPlay()
{
	Super::BeginPlay();
}