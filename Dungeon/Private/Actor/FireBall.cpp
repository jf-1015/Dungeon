// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/FireBall.h"
#include "Components/SphereComponent.h"//
#include "Components/BoxComponent.h"//
#include "AbilitySystemComponent.h"//
#include "AbilitySystemBlueprintLibrary.h"//
#include "AbilitySystem/DungeonAbilitySystemLibrary.h"//
#include "Interaction/CharacterInterface.h"//
#include "Interaction/PlayerInterface.h"//
#include "DungeonGameplayTags.h"//
#include "GameFramework/ProjectileMovementComponent.h"//
#include "NiagaraFunctionLibrary.h"//
#include "Kismet/KismetSystemLibrary.h"//

AFireBall::AFireBall()
{
	Box->SetSimulatePhysics(false);

	HomingSphere = CreateDefaultSubobject<USphereComponent>("HomingSphere");
	HomingSphere->SetupAttachment(GetRootComponent());

	HomingSphere->OnComponentBeginOverlap.AddDynamic(this, &AFireBall::OnSphereOverlap);

	ExplosionSphere = CreateDefaultSubobject<USphereComponent>("ExplosionSphere");
	ExplosionSphere->SetupAttachment(GetRootComponent());

	SetLifeSpan(LifeSpan);

	OnDestroyed.AddDynamic(this, &AFireBall::WhenDestroy);
}

void AFireBall::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!IsValidOverlap(OtherActor))return;

	const FDungeonGameplayTags DungeonTags = FDungeonGameplayTags::Get();
	AActor* ClosetActor = nullptr;

	FGameplayTag TargetTag = FGameplayTag();
	if (ICharacterInterface::Execute_GetCharacterTag(GetOwner()) == DungeonTags.Character_Enemy)
	{
		TargetTag = DungeonTags.Character_Player;
	}
	else if (ICharacterInterface::Execute_GetCharacterTag(GetOwner()) == DungeonTags.Character_Player)
	{
		TargetTag = DungeonTags.Character_Enemy;
	}

	UDungeonAbilitySystemLibrary::GetClosestCharacterInSightByTag(
		this, 
		ClosetActor, 
		TArray<AActor*>(),
		HomingSphere->GetScaledSphereRadius(), 
		GetActorLocation(),
		TargetTag);

	if (IsValid(ClosetActor))
	{
		ProjectileMovement->bIsHomingProjectile = true;

		ProjectileMovement->HomingTargetComponent = ClosetActor->GetRootComponent();
		ProjectileMovement->HomingAccelerationMagnitude = FMath::RandRange(MinAccelerationMagnitude, MaxAccelerationMagnitude);

		HomingSphere->OnComponentBeginOverlap.RemoveAll(this);

		OnHomingEnd.Broadcast();
	}
}

void AFireBall::OnBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	TArray<AActor*>ActorInExplosionRange;

	TArray<TEnumAsByte<EObjectTypeQuery>>ObjectTypes;
	ObjectTypes.Add(EObjectTypeQuery::ObjectTypeQuery3);

	TArray<AActor*>ActorsToIgnore;
	ActorsToIgnore.Add(GetOwner());

	UKismetSystemLibrary::SphereOverlapActors(GetWorld(), 
		GetActorLocation(), 
		ExplosionSphere->GetScaledSphereRadius(), 
		ObjectTypes, 
		nullptr, 
		ActorsToIgnore,
		ActorInExplosionRange);

	for (AActor* Actor : ActorInExplosionRange)
	{
		if (IsValid(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Actor)))
		{
			if (ICharacterInterface::Execute_GetIsDodge(Actor))return;

			DamageEffectParams.TargetAbilitySystemComponent = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Actor);

			CheckIsbBlocked(Actor);

			DamageEffectParams.bBlcoked = IsBlocked;
			DamageEffectParams.bReactBlock = ShouldReactBlock;

			UDungeonAbilitySystemLibrary::ApplyDamageEffect(DamageEffectParams);

			UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, ImpactEffect, GetActorLocation());
			bExploded = true;
		}
	}
	Destroy();
}

void AFireBall::WhenDestroy(AActor* InActor)
{
	if (!bExploded)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, ImpactEffect, GetActorLocation());

		TArray<AActor*>ActorInExplosionRange;

		TArray<TEnumAsByte<EObjectTypeQuery>>ObjectTypes;
		ObjectTypes.Add(EObjectTypeQuery::ObjectTypeQuery3);

		UKismetSystemLibrary::SphereOverlapActors(GetWorld(),
			GetActorLocation(),
			ExplosionSphere->GetScaledSphereRadius(),
			ObjectTypes,
			nullptr,
			TArray<AActor*>(),
			ActorInExplosionRange);

		for (AActor* Actor : ActorInExplosionRange)
		{
			if (IsValid(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Actor)))
			{
				if (ICharacterInterface::Execute_GetIsDodge(Actor))return;

				DamageEffectParams.TargetAbilitySystemComponent = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Actor);

				CheckIsbBlocked(Actor);

				DamageEffectParams.bBlcoked = IsBlocked;
				DamageEffectParams.bReactBlock = ShouldReactBlock;

				UDungeonAbilitySystemLibrary::ApplyDamageEffect(DamageEffectParams);
			}
		}
	}
}