// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Ability/Melee/MeleeGameplayAbility.h"
#include "Kismet/KismetSystemLibrary.h"//
#include "Interaction/CharacterInterface.h"//
#include "Interaction/PlayerInterface.h"//
#include "AbilitySystemBlueprintLibrary.h"//
#include "DungeonGameplayTags.h"////
#include "AbilitySystem/DungeonAbilitySystemLibrary.h"//
#include "Kismet/KismetMathLibrary.h"//
#include "Actor/DungeonProjectile.h"//
#include "Data/ProjectileClass.h"//
#include "Data/DetailedEquipmemtInfo.h"//
#include "GameFramework/ProjectileMovementComponent.h"//

void UMeleeGameplayAbility::TraceCheckTimerFunction(
	TArray<AActor*>& Outs,
	UPARAM(ref)TArray<AActor*>& TraceLineActors,
	const TArray<AActor*>& ActorsToIgnore,
	float InRaidus,
	UPARAM(ref)UStaticMeshComponent* InWeaponComponent)
{
	TArray<FHitResult>HitResults;

	TArray<TEnumAsByte<EObjectTypeQuery>>ObjectTypes;
	ObjectTypes.Add(EObjectTypeQuery::ObjectTypeQuery3);

	for (FName SocketName : WeaponSocketNames)
	{
		UKismetSystemLibrary::SphereTraceMultiForObjects(this,
			NameToLocation.FindChecked(SocketName),
			InWeaponComponent->GetSocketLocation(SocketName),
			InRaidus,
			ObjectTypes,
			false,
			ActorsToIgnore,
			EDrawDebugTrace::ForDuration,
			HitResults,
			true,
			FLinearColor::Red,
			FLinearColor::Green,
			0.5);

		for (FHitResult& HitResult : HitResults)
		{
			if (HitResult.GetActor()->Implements<UCharacterInterface>() && !ICharacterInterface::Execute_IsDead(HitResult.GetActor()))
			{
				if (!TraceLineActors.Contains(HitResult.GetActor()))
				{
					TraceLineActors.Add(HitResult.GetActor());
				}
			}
		}
	}
	Outs = TraceLineActors;
}

void UMeleeGameplayAbility::PreTraceCheck(UPARAM(ref) UStaticMeshComponent* InWeaponComponent)
{
	bBlocked = false;
	NameToLocation.Empty();

	WeaponSocketNames = InWeaponComponent->GetAllSocketNames();

	for (FName SocketName : WeaponSocketNames)
	{
		NameToLocation.Add(SocketName, InWeaponComponent->GetSocketLocation(SocketName));
	}
}

void UMeleeGameplayAbility::PrepareForNext(UPARAM(ref)UStaticMeshComponent* InWeaponComponent)
{
	NameToLocation.Empty();

	WeaponSocketNames = InWeaponComponent->GetAllSocketNames();

	for (FName SocketName : WeaponSocketNames)
	{
		NameToLocation.Add(SocketName, InWeaponComponent->GetSocketLocation(SocketName));
	}
}

void UMeleeGameplayAbility::ApplyEffectInOneRound(AActor* HitActor)
{
	if (!HitActors.Contains(HitActor))
	{
		HitActors.Add(HitActor);

		if (CheckIsOnFrontAndBlocking(HitActor))
		{
			bBlocked = true;

			FDamageEffectParams Params = MakeDamageEffectParams(HitActor);
			Params.bBlcoked = bBlocked;
			UDungeonAbilitySystemLibrary::ApplyDamageEffect(Params);
		}
		else
		{
			bBlocked = false;

			FDamageEffectParams Params = MakeDamageEffectParams(HitActor);
			Params.bBlcoked = bBlocked;
			UDungeonAbilitySystemLibrary::ApplyDamageEffect(Params);
		}
	}
}

bool UMeleeGameplayAbility::CheckIsOnFrontAndBlocking(AActor* InActor)
{
	FVector TargetForwardVector = InActor->GetActorForwardVector();
	FVector SourceForwardVector = GetAvatarActorFromActorInfo()->GetActorForwardVector();

	float Value = TargetForwardVector.Dot(SourceForwardVector);
	float Angle = UKismetMathLibrary::DegAcos(Value);

	if (135 <= Angle && Angle <= 180)
	{
		if (ICharacterInterface::Execute_GetIsBlocking(InActor))
		{
			return true;
		}
	}
	return false;
}

void UMeleeGameplayAbility::ThrowCurrentWeapon_1H()
{
	FGameItemInfo CurrentWeaponInfo = IPlayerInterface::Execute_GetEquipedWeaponInfo(GetAvatarActorFromActorInfo());
	UProjectileClass* Projectiles = UDungeonAbilitySystemLibrary::GetProjectileClass(GetAvatarActorFromActorInfo());

	FTransform SpawnTransform;
	SpawnTransform.SetLocation(IPlayerInterface::Execute_GetThrowPointTransform(GetAvatarActorFromActorInfo()).GetLocation());

	ADungeonProjectile* Projectile = GetWorld()->SpawnActorDeferred<ADungeonProjectile>(
		Projectiles->GetSubclassByItemIndex(CurrentWeaponInfo.ItemIndex),
		SpawnTransform,
		GetOwningActorFromActorInfo(),
		Cast<APawn>(GetOwningActorFromActorInfo()),
		ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

	Projectile->OwnerName = GetAvatarActorFromActorInfo()->GetFName();

	Projectile->SetOwner(GetAvatarActorFromActorInfo());

	Projectile->DamageEffectParams = MakeDamageEffectParams();

	Projectile->ProjectileMovement->Velocity = IPlayerInterface::Execute_GetThrowVelocity(GetAvatarActorFromActorInfo());

	IPlayerInterface::Execute_ExpendItem(GetAvatarActorFromActorInfo(), CurrentWeaponInfo.ItemIndex, 1);

	/*Inherit Weapon Durability*/
	Projectile->Durability = CurrentWeaponInfo.Durability;

	Projectile->FinishSpawning(SpawnTransform);
}

void UMeleeGameplayAbility::SetCost(FGameplayTag HandheldType)
{
	if (HandheldType == FDungeonGameplayTags::Get().Handheld_2H)
	{
		CostGameplayEffectClass = CostEffectClass2H;
	}
	else
	{
		CostGameplayEffectClass = CostEffectClass1H;
	}
}

UAnimMontage* UMeleeGameplayAbility::GetWeaponHeavyAttackMontage()
{
	AActor* Player = GetAvatarActorFromActorInfo();
	FGameItemInfo WeaponInfo = IPlayerInterface::Execute_GetEquipedWeaponInfo(Player);

	SetCost(WeaponInfo.HandheldType);

	UDetailedEquipmemtInfo* EquipmentInfomation = UDungeonAbilitySystemLibrary::GetEquipmentInfomation(Player);
	return EquipmentInfomation->GetWeaponInfoForItemIndex(WeaponInfo.ItemIndex).HeavyAttack;
}