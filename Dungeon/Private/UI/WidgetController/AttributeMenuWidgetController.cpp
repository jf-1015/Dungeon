// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgetController/AttributeMenuWidgetController.h"
#include "AbilitySystem/CharacterAttributeSet.h"//
#include "Data/DungeonAttributeInfo.h"//

void UAttributeMenuWidgetController::BroadcastInitialValues()
{
	check(AttributeInfo);

	UCharacterAttributeSet* CharacterAS = Cast<UCharacterAttributeSet>(AttributeSet);

	for (auto& Pair : CharacterAS->TagToAttribute)
	{
		BroadcastAttributeInfo(Pair.Key, Pair.Value());
	}
}

void UAttributeMenuWidgetController::BindCallBacksToDependencies()
{
	check(AttributeInfo);

	UCharacterAttributeSet* CharacterAS = Cast<UCharacterAttributeSet>(AttributeSet);

	for (auto& Pair : CharacterAS->TagToAttribute)
	{
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(Pair.Value()).AddLambda
		([this, Pair](const FOnAttributeChangeData& Data)
			{
				BroadcastAttributeInfo(Pair.Key, Pair.Value());
			}
		);
	}
}

void UAttributeMenuWidgetController::BroadcastAttributeInfo(const FGameplayTag& Tag, const FGameplayAttribute& Attribute)
{
	FAttributeInfo Info = AttributeInfo->FindAttributeInfoForTag(Tag);
	Info.AttributeValue = Attribute.GetNumericValue(AttributeSet);
	OnAttributeInfoDelegate.Broadcast(Info);
}