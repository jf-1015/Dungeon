// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/PickupItems.h"
#include "Components/BoxComponent.h"//
#include "Components/StaticMeshComponent.h"//
#include "Components/WidgetComponent.h"//

APickupItems::APickupItems()
{
	PrimaryActorTick.bCanEverTick = false;

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("StaicMeshComponent");
	StaticMeshComponent->SetupAttachment(GetRootComponent());

	StaticMeshComponent->SetSimulatePhysics(true);

	OverlapBox = CreateDefaultSubobject<UBoxComponent>("Box");
	OverlapBox->SetupAttachment(StaticMeshComponent);

	PickupIcon = CreateDefaultSubobject<UWidgetComponent>("InteractIcon");
	PickupIcon->SetupAttachment(StaticMeshComponent);
}

void APickupItems::SetInteractIconVisibility_Implementation(bool bInteractIcon)
{
	if(IsValid(PickupIcon))
	{
		PickupIcon->SetVisibility(bInteractIcon);
	}
}

void APickupItems::BeginPlay()
{
	Super::BeginPlay();

	OverlapBox->OnComponentBeginOverlap.AddDynamic(this, &APickupItems::OnSphereOverlap);
}

void APickupItems::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
}