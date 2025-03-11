// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "SpecificInputTask.generated.h"

struct FGameplayTag;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnSpecificInput);

/**
 * 
 */
UCLASS()
class DUNGEON_API USpecificInputTask : public UAbilityTask
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintAssignable)
	FOnSpecificInput OnSpecificInput;

	UFUNCTION(BlueprintCallable, Category = "Ability|Tasks", meta = (DisplayName = "WaitSpecificInput", HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "true"))
	static USpecificInputTask* CreateUSpecificInputTask(UGameplayAbility* OwningAbility, const FGameplayTag& InputTag);

	virtual void Activate()override;

protected:

	FGameplayTag ListenTag = FGameplayTag();

private:

	UFUNCTION()
	void FunctionToBindDelegate(const FGameplayTag& InputTag);
	
};