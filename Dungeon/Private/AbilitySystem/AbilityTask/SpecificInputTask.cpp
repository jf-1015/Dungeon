// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/AbilityTask/SpecificInputTask.h"
#include "ControlledPlayerController.h"//

USpecificInputTask* USpecificInputTask::CreateUSpecificInputTask(UGameplayAbility* OwningAbility, const FGameplayTag& InputTag)
{
	USpecificInputTask* MyObj = NewAbilityTask<USpecificInputTask>(OwningAbility);
	MyObj->ListenTag = InputTag;
	return MyObj;
}

void USpecificInputTask::Activate()
{
	AControlledPlayerController* AvatarPC = Cast<AControlledPlayerController>(Ability->GetCurrentActorInfo()->PlayerController.Get());
	AvatarPC->OnInputPressed.AddDynamic(this, &USpecificInputTask::FunctionToBindDelegate);
}

void USpecificInputTask::FunctionToBindDelegate(const FGameplayTag& InputTag)
{
	if(InputTag==ListenTag)
	{
		OnSpecificInput.Broadcast();
	}
}