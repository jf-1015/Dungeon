// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Ability/Range/RangeGameplayAbility.h"
#include "Interaction/CharacterInterface.h"//
#include "Interaction/PlayerInterface.h"//
#include "Actor/DungeonProjectile.h"//
#include "AbilitySystem/DungeonAbilitySystemLibrary.h"//
#include "Data/ProjectileClass.h"//
#include "GameFramework/ProjectileMovementComponent.h"//

void URangeGameplayAbility::SpawnProjectile(bool IsAiming, bool IsLockTarget, bool bOverridePitch, float PitchOverride, bool InIsArrow)
{
	const FDungeonGameplayTags Tags = FDungeonGameplayTags::Get();

	if (ICharacterInterface::Execute_GetDurability(GetAvatarActorFromActorInfo(), false) != 0)
	{
		ICharacterInterface::Execute_SetDurability(GetAvatarActorFromActorInfo(), -1, false);
	}

	FTransform SpawnTransform;

	if (!HandleTransform(SpawnTransform, IsAiming, IsLockTarget, bOverridePitch, PitchOverride, InIsArrow))return;

	ADungeonProjectile* Projectile = GetWorld()->SpawnActorDeferred<ADungeonProjectile>(
		ProjectileClass,
		SpawnTransform,
		GetOwningActorFromActorInfo(),
		Cast<APawn>(GetOwningActorFromActorInfo()),
		ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

	Projectile->OwnerName = GetAvatarActorFromActorInfo()->GetFName();

	Projectile->SetOwner(GetAvatarActorFromActorInfo());

	Projectile->DamageEffectParams = MakeDamageEffectParams();

	Projectile->FinishSpawning(SpawnTransform);
}

void URangeGameplayAbility::SpawnActor_Throw()
{
	FGameItemInfo CurrentConsumableInfo = IPlayerInterface::Execute_GetEquipedConsumableInfo(GetAvatarActorFromActorInfo());
	UProjectileClass* Projectiles = UDungeonAbilitySystemLibrary::GetProjectileClass(GetAvatarActorFromActorInfo());

	FTransform SpawnTransform;
	SpawnTransform.SetLocation(IPlayerInterface::Execute_GetThrowPointTransform(GetAvatarActorFromActorInfo()).GetLocation());

	ADungeonProjectile* Projectile = GetWorld()->SpawnActorDeferred<ADungeonProjectile>(
		Projectiles->GetSubclassByItemIndex(CurrentConsumableInfo.ItemIndex),
		SpawnTransform,
		GetOwningActorFromActorInfo(),
		Cast<APawn>(GetOwningActorFromActorInfo()),
		ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

	Projectile->OwnerName = GetAvatarActorFromActorInfo()->GetFName();

	Projectile->SetOwner(GetAvatarActorFromActorInfo());

	Projectile->DamageEffectParams = MakeDamageEffectParams();

	Projectile->ProjectileMovement->Velocity = IPlayerInterface::Execute_GetThrowVelocity(GetAvatarActorFromActorInfo());

	Projectile->FinishSpawning(SpawnTransform);
}

bool URangeGameplayAbility::HandleTransform(FTransform& InTransform, bool IsAiming, bool IsLockTarget, bool bOverridePitch, float PitchOverride, bool InIsArrow)
{
	const FDungeonGameplayTags Tags = FDungeonGameplayTags::Get();

	UStaticMeshComponent* Weapon = ICharacterInterface::Execute_GetWeaponStaticMeshComponent(GetAvatarActorFromActorInfo());
	if (!IsValid(Weapon))return false;

	if (ICharacterInterface::Execute_GetCurrentWeaponType(GetAvatarActorFromActorInfo()) == Tags.Weapon_Range)
	{
		FVector Socket_1 = Weapon->GetSocketLocation(FName("Socket_1"));
		FVector Socket_2 = Weapon->GetSocketLocation(FName("Socket_2"));

		FRotator Rotation = (Socket_1 - Socket_2).Rotation();

		if (bOverridePitch)
		{
			Rotation.Pitch = PitchOverride;
		}

		InTransform.SetLocation(Socket_1);
		if (IsAiming)
		{
			APlayerController* Controller = GetActorInfo().PlayerController.Get();

			Rotation = Controller->PlayerCameraManager->GetCameraRotation();

			int32 SizeX;
			int32 SizeY;
			FVector WorldLocation;
			FVector WorldDirection;
			Controller->GetViewportSize(SizeX, SizeY);
			Controller->DeprojectScreenPositionToWorld(SizeX / 2, SizeY / 2, WorldLocation, WorldDirection);
			InTransform.SetLocation(WorldLocation);
		}
		if (IsLockTarget)
		{
			FVector LockActorLocation = ICharacterInterface::Execute_GetTargetActor(GetAvatarActorFromActorInfo())->GetActorLocation();

			if (InIsArrow)
			{
				LockActorLocation += FVector(0.f, 0.f, 10.f);
			}

			Rotation = (LockActorLocation - Socket_1).Rotation();
		}
		InTransform.SetRotation(Rotation.Quaternion());

		return true;
	}
	else
	{
		if(ICharacterInterface::Execute_GetCurrentWeaponType(GetAvatarActorFromActorInfo()) == Tags.Weapon_Magic)
		{
			FVector Socket_1 = Weapon->GetSocketLocation(FName("Socket_1"));
			InTransform.SetLocation(Socket_1);
			InTransform.SetRotation(GetAvatarActorFromActorInfo()->GetActorRotation().Quaternion());

			if (IsLockTarget)
			{
				FVector LockActorLocation = ICharacterInterface::Execute_GetTargetActor(GetAvatarActorFromActorInfo())->GetActorLocation();
				
				InTransform.SetRotation((LockActorLocation - Socket_1).Rotation().Quaternion());
			}

			return true;
		}
	}

	return false;
}