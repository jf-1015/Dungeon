// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AbilitySystem/DungeonAbilityTypes.h"//
#include "Interaction/ProjectileInterface.h"//
#include "DungeonProjectile.generated.h"

class UStaticMeshComponent;
class UBoxComponent;
class UProjectileMovementComponent;
class APickupItems;

UCLASS()
class DUNGEON_API ADungeonProjectile : public AActor, public IProjectileInterface
{
	GENERATED_BODY()
	
public:	

	/*Projectile Interface*/
	virtual bool GetIsBlocked_Implementation()override;
	virtual void SetIsBlocked_Implementation(bool InBool)override;
	/*Projectile Interface*/

	FName OwnerName = FName();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TObjectPtr<UProjectileMovementComponent>ProjectileMovement;

	UPROPERTY(BlueprintReadWrite)
	FDamageEffectParams DamageEffectParams;

	UPROPERTY(EditDefaultsOnly, Category = "Projectile")
	bool IsArrow = false;

	UPROPERTY(EditDefaultsOnly, Category = "Projectile")
	TSubclassOf<APickupItems>RecycleItemClass;

	UPROPERTY(EditDefaultsOnly, Category = "Projectile")
	TObjectPtr<UStaticMesh> BrokenMesh;

	UPROPERTY(EditDefaultsOnly, Category = "Projectile")
	int32 InitialSpeed = 550.f;

	UPROPERTY(EditDefaultsOnly, Category = "Projectile")
	int32 MaxSpeed = 550.f;

	UPROPERTY(EditDefaultsOnly, Category = "Projectile")
	bool CanRecycle = false;

	UPROPERTY(EditDefaultsOnly, Category = "Projectile")
	bool ShouldReactBlock = false;

	/*Throwable*/
	UPROPERTY(EditDefaultsOnly, Category = "Throwable")
	bool IsThrowable = false;

	UPROPERTY(EditDefaultsOnly, Category = "Throwable")
	float BaseDamage = 0.f;

	UPROPERTY(EditDefaultsOnly, Category = "Throwable")
	float PowCE = 0.f;

	UPROPERTY(EditDefaultsOnly, Category = "Throwable")
	float DexCE = 0.f;

	UPROPERTY(EditDefaultsOnly, Category = "Throwable")
	float IntCE = 0.f;

	UPROPERTY(EditDefaultsOnly, Category = "Throwable")
	float ResilienceCut = 0.f;
	/*Throwable*/

	UPROPERTY()
	int32 Durability = 1;

	ADungeonProjectile();

	UFUNCTION()
	FVector GetComponentVelocity();

	UFUNCTION()
	virtual void OnBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);	

protected:

	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UStaticMeshComponent>Projectile;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UBoxComponent>Box;

	bool IsBlocked = false;

	bool IsValidOverlap(AActor* OtherActor);

	void SpawnRecycleItemToWorld();

	void CheckIsbBlocked(AActor* OtherActor);
};