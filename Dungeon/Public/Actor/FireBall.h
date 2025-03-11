// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actor/DungeonProjectile.h"
#include "FireBall.generated.h"

class USphereComponent;
class UNiagaraSystem;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnHomingEnd);

/**
 * 
 */
UCLASS()
class DUNGEON_API AFireBall : public ADungeonProjectile
{
	GENERATED_BODY()

public:

	AFireBall();

	UFUNCTION()
	virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	virtual void OnBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UPROPERTY(BlueprintAssignable)
	FOnHomingEnd OnHomingEnd;

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<USphereComponent>ExplosionSphere;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<USphereComponent>HomingSphere;

	UPROPERTY(EditAnywhere, Category = "FireBall")
	TObjectPtr<UNiagaraSystem>ImpactEffect;

	UPROPERTY(EditAnywhere, Category = "FireBall")
	float MaxAccelerationMagnitude = 400.f;

	UPROPERTY(EditAnywhere, Category = "FireBall")
	float MinAccelerationMagnitude = 200.f;

	UPROPERTY(EditAnywhere, Category = "FireBall")
	float LifeSpan = 5, f;
	
	bool bExploded = false;

	UFUNCTION()
	void WhenDestroy(AActor* InActor);
};