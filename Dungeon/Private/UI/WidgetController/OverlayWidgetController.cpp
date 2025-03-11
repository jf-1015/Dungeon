// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgetController/OverlayWidgetController.h"
#include "AbilitySystem/CharacterAttributeSet.h"//
#include "ControlledPlayerState.h"//
#include "DungeonGameplayTags.h"//

void UOverlayWidgetController::BroadcastInitialValues()
{
	UCharacterAttributeSet* CharacterAS = Cast<UCharacterAttributeSet>(AttributeSet);

	for (auto& Pair : CharacterAS->TagToAttribute)
	{
		if(AttributesToShow.HasTagExact(Pair.Key))
		{
			OnAttributeChangedDelegate.Broadcast(Pair.Value().GetNumericValue(AttributeSet), Pair.Key);
		}
	}
}

void UOverlayWidgetController::BindCallBacksToDependencies()
{
	FDungeonGameplayTags Tags = FDungeonGameplayTags::Get();
	UCharacterAttributeSet* CharacterAS = Cast<UCharacterAttributeSet>(AttributeSet);

	AttributesToShow.AddTag(Tags.Attribute_Derived_MaxHealth);
	AttributesToShow.AddTag(Tags.Attribute_Vital_Health);
	AttributesToShow.AddTag(Tags.Attribute_Vital_Mana);
	AttributesToShow.AddTag(Tags.Attribute_Vital_Energy);
	AttributesToShow.AddTag(Tags.Attribute_Vital_Food);

	for (auto& Pair : CharacterAS->TagToAttribute)
	{
		if (AttributesToShow.HasTagExact(Pair.Key))
		{
			AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(Pair.Value()).AddLambda
			([this, Pair](const FOnAttributeChangeData& Data)
				{
					OnAttributeChangedDelegate.Broadcast(Pair.Value().GetNumericValue(AttributeSet), Pair.Key);
				}
			);
		}
	}
}