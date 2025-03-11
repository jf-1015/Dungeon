// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/AbilityTask/AsyncInputTask.h"
#include "ControlledPlayerController.h"//

UAsyncInputTask* UAsyncInputTask::CreateUAsyncInputTask(UGameplayAbility* OwningAbility)
{
	UAsyncInputTask* MyObj = NewAbilityTask<UAsyncInputTask>(OwningAbility);
	return MyObj;
}

void UAsyncInputTask::Activate()
{
	AControlledPlayerController* AvatarPC = Cast<AControlledPlayerController>(Ability->GetCurrentActorInfo()->PlayerController.Get());
	AvatarPC->OnItemSelected.AddDynamic(this, &UAsyncInputTask::FunctionToBindDelegate);
}

void UAsyncInputTask::FunctionToBindDelegate(AActor* PickupItem, bool bUseDirectly, bool bSelected)
{
	OnAsyncInput.Broadcast(PickupItem, bUseDirectly, bSelected);
}