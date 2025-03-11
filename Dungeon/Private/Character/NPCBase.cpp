// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/NPCBase.h"
#include "Components/SphereComponent.h"//
#include "Components/WidgetComponent.h"//
#include "Interaction/PlayerInterface.h"//
#include "Data/DungeonItemInfo.h"//
#include "Actor/PickupItems.h"//

ANPCBase::ANPCBase()
{
	PrimaryActorTick.bCanEverTick = false;

	InteractRange = CreateDefaultSubobject<USphereComponent>("Range");
	InteractRange->SetupAttachment(GetRootComponent());

	InteractRange->SetCollisionResponseToAllChannels(ECR_Ignore);
	InteractRange->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

	InteractIcon = CreateDefaultSubobject<UWidgetComponent>("InteractIcon");
	InteractIcon->SetupAttachment(GetMesh());

	InteractRange->OnComponentBeginOverlap.AddDynamic(this, &ANPCBase::OnBeginOverlap);
	InteractRange->OnComponentEndOverlap.AddDynamic(this, &ANPCBase::OnEndOverlap);
}

bool ANPCBase::StartInteractEvent_Implementation()
{
	if (PlayerInRange != nullptr)
	{
		OnEventStart.Broadcast();
	}
	return true;
}

void ANPCBase::SetInteractIconVisibility_Implementation(bool bInteractIcon)
{
	if (PlayerInRange != nullptr)
	{
		InteractIcon->SetVisibility(bInteractIcon);
		if (!bInteractIcon)
		{
			CloseShop.Broadcast();
		}
	}
}

bool ANPCBase::IsNPC_Implementation()
{
	return true;
}

bool ANPCBase::IsPlayerInRange_Implementation()
{
	if (PlayerInRange == nullptr)
	{
		return false;
	}
	else
	{
		return true;
	}
}

void ANPCBase::SetItemNumBySlotIndex(int32 SlotIndex, int32 NumPurchased)
{
	for ( FItemsToShop& Info : ItemsToSell)
	{
		if (Info.SlotIndex == SlotIndex)
		{
			Info.NumToSell -= NumPurchased;

			SpawnItemToWorld(Info.ItemClass, NumPurchased);

			UpdateShop.Broadcast(ItemsToSell);
			return;
		}
	}
}

int32 ANPCBase::GetItemNumBySlotIndex(int32 SlotIndex)
{
	for (FItemsToShop& Info : ItemsToSell)
	{
		if (Info.SlotIndex == SlotIndex)
		{
			return Info.NumToSell;
		}
	}
	return -1;
}

AActor* ANPCBase::GetPlayerInRange()
{
	return PlayerInRange;
}

void ANPCBase::SpawnItemToWorld(TSubclassOf<APickupItems> ItemClass, int32 Num)
{
	for(int32 i=0;i<Num;++i)
	{
		FTransform Transform;
		FVector BaseLocation = GetActorLocation() + GetActorForwardVector() * 15;

		BaseLocation = BaseLocation.RotateAngleAxis(FMath::RandRange(-5.f, 5.f), FVector::UpVector);

		Transform.SetLocation(BaseLocation);
		Transform.SetRotation(GetActorQuat());

		APickupItems* Item = GetWorld()->SpawnActorDeferred<APickupItems>(ItemClass, Transform);
		Item->FinishSpawning(Transform);
	}
}

void ANPCBase::BeginPlay()
{
	Super::BeginPlay();
}

void ANPCBase::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->Implements<UPlayerInterface>())
	{
		PlayerInRange = OtherActor;
	}
}

void ANPCBase::OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor->Implements<UPlayerInterface>())
	{
		CloseShop.Broadcast();

		InteractIcon->SetVisibility(false);
		PlayerInRange = nullptr;
	}
}