// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "AsyncInputTask.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnAsyncInput, AActor*, PickupItem, bool, bUseDirectly, bool, bSelected);

/**
 * 
 */
UCLASS()
class DUNGEON_API UAsyncInputTask : public UAbilityTask
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintAssignable)
	FOnAsyncInput OnAsyncInput;

	UFUNCTION(BlueprintCallable, Category = "Ability|Tasks", meta = (DisplayName = "WaitAsyncInput", HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "true"))
	static UAsyncInputTask* CreateUAsyncInputTask(UGameplayAbility* OwningAbility);

	virtual void Activate()override;

private:

	UFUNCTION()
	void FunctionToBindDelegate(AActor* PickupItem, bool bUseDirectly, bool bSelected);

};