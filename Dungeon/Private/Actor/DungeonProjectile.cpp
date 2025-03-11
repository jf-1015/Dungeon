// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/DungeonProjectile.h"
#include "Components/BoxComponent.h"//
#include "Components/StaticMeshComponent.h"//
#include "GameFramework/ProjectileMovementComponent.h"//
#include "Interaction/CharacterInterface.h"//
#include "AbilitySystemComponent.h"//
#include "AbilitySystemBlueprintLibrary.h"//
#include "AbilitySystem/DungeonAbilityTypes.h"//
#include "AbilitySystem/DungeonAbilitySystemLibrary.h"//
#include "Kismet/KismetMathLibrary.h"//
#include "Actor/PickupItems.h"//

bool ADungeonProjectile::GetIsBlocked_Implementation()
{
	return IsBlocked;
}

void ADungeonProjectile::SetIsBlocked_Implementation(bool InBool)
{
	IsBlocked = InBool;
}

ADungeonProjectile::ADungeonProjectile()
{
	PrimaryActorTick.bCanEverTick = false;

	Box = CreateDefaultSubobject<UBoxComponent>("Box");
	SetRootComponent(Box);

	Box->SetSimulatePhysics(true);
	Box->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	Box->SetCollisionResponseToAllChannels(ECR_Overlap);

	Box->IgnoreActorWhenMoving(GetOwner(), true);

	Box->OnComponentBeginOverlap.AddDynamic(this, &ADungeonProjectile::OnBoxOverlap);

	Projectile = CreateDefaultSubobject<UStaticMeshComponent>("Projectile");
	Projectile->SetupAttachment(Box);

	Projectile->SetSimulatePhysics(false);
	Projectile->SetCollisionObjectType(ECollisionChannel::ECC_PhysicsBody);
	Projectile->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Projectile->SetCollisionResponseToAllChannels(ECR_Block);

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>("ProjectileMovement");
	ProjectileMovement->InitialSpeed = InitialSpeed;
	ProjectileMovement->MaxSpeed = MaxSpeed;

}

FVector ADungeonProjectile::GetComponentVelocity()
{
	return Box->GetPhysicsLinearVelocity();
}

void ADungeonProjectile::OnBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!IsValidOverlap(OtherActor))return;

	if (IsValid(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OtherActor)))
	{
		if (ICharacterInterface::Execute_GetIsDodge(OtherActor))return;

		DamageEffectParams.TargetAbilitySystemComponent = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OtherActor);

		CheckIsbBlocked(OtherActor);

		DamageEffectParams.bBlcoked = IsBlocked;
		DamageEffectParams.bReactBlock = ShouldReactBlock;

		DamageEffectParams.bThrowable = IsThrowable;
		if (IsThrowable)
		{
			DamageEffectParams.BaseDamage = BaseDamage;
			DamageEffectParams.PowCE = PowCE;
			DamageEffectParams.DexCE = DexCE;
			DamageEffectParams.IntCE = IntCE;
			DamageEffectParams.ResilienceCut = ResilienceCut;
			DamageEffectParams.Durability = Durability;
		}

		if (CanRecycle)
		{
			bool bBroken = false;

			if (IsArrow)
			{
				bBroken = FMath::RandRange(1, 100) > 50;
			}
				
			if (bBroken)
			{
				Box->OnComponentBeginOverlap.RemoveAll(this);
				Projectile->SetStaticMesh(BrokenMesh);
				UDungeonAbilitySystemLibrary::ApplyDamageEffect(DamageEffectParams);
			}
			else
			{
				SpawnRecycleItemToWorld();

				UDungeonAbilitySystemLibrary::ApplyDamageEffect(DamageEffectParams);

				Destroy();
			}
		}
		else
		{
			UDungeonAbilitySystemLibrary::ApplyDamageEffect(DamageEffectParams);
			Destroy();
		}	
	}
	else
	{
		if(CanRecycle)
		{
			bool bBroken = false;

			if (IsArrow)
			{
				bBroken = FMath::RandRange(1, 100) > 70;
			}

			if (bBroken)
			{
				Box->OnComponentBeginOverlap.RemoveAll(this);
				Projectile->SetStaticMesh(BrokenMesh);
			}
			else
			{
				SpawnRecycleItemToWorld();

				Destroy();
			}	
		}
		else
		{
			Destroy();
		}
	}
}

void ADungeonProjectile::BeginPlay()
{
	Super::BeginPlay();	
}

bool ADungeonProjectile::IsValidOverlap(AActor* OtherActor)
{
	if (DamageEffectParams.SourceAbilitySystemComponent == nullptr)return false;
	AActor* SourceAvatar = DamageEffectParams.SourceAbilitySystemComponent->GetAvatarActor();
	if (SourceAvatar == OtherActor)return false;
	return true;
}

void ADungeonProjectile::SpawnRecycleItemToWorld()
{
	FTransform Transform;
	Transform.SetLocation(GetActorTransform().GetLocation());
	Transform.SetRotation(GetActorTransform().GetRotation());

	APickupItems* Item = GetWorld()->SpawnActorDeferred<APickupItems>(
		RecycleItemClass,
		Transform);

	if (Item->ItemInfo.ItemType == FDungeonGameplayTags::Get().ItemType_Weapon && Durability > 0)
	{
		Item->ItemInfo.Durability = Durability - 1;
	}

	Item->FinishSpawning(GetActorTransform());
}

void ADungeonProjectile::CheckIsbBlocked(AActor* OtherActor)
{
	if (OtherActor->Implements<UCharacterInterface>())
	{
		if (ICharacterInterface::Execute_GetSecondaryWeaponHandheldType(OtherActor) == FDungeonGameplayTags::Get().Handheld_Secondary_Shield 
			&& 
			ICharacterInterface::Execute_GetSecondaryWeaponStaticMeshComponent(OtherActor) != nullptr)
		{
			TSet<AActor*>OverlapingActors;
			ICharacterInterface::Execute_GetSecondaryWeaponStaticMeshComponent(OtherActor)->GetOverlappingActors(OverlapingActors);

			for (AActor* Actor : OverlapingActors)
			{
				if (Actor->Implements<UProjectileInterface>())
				{
					IProjectileInterface::Execute_SetIsBlocked(Actor, true);
				}
			}
		}
	}
}