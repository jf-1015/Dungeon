// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/CharacterAttributeSet.h"
#include "Net/UnrealNetwork.h"//
#include "GameplayEffectExtension.h"//
#include "AbilitySystemBlueprintLibrary.h"//
#include "DungeonGameplayTags.h"//
#include "AbilitySystem/DungeonAbilitySystemLibrary.h"//
#include "Interaction/CharacterInterface.h"//
#include "Interaction/EnemyInterface.h"//
#include "GameplayEffectComponents/TargetTagsGameplayEffectComponent.h"//
#include "AbilitySystem/DungeonAbilityTypes.h"//

UCharacterAttributeSet::UCharacterAttributeSet()
{
	const FDungeonGameplayTags Tags = FDungeonGameplayTags::Get();

	/*Base Attribute*/
	TagToAttribute.Add(Tags.Attribute_Base_Vitality, GetVitalityAttribute);
	TagToAttribute.Add(Tags.Attribute_Base_Power, GetPowerAttribute);
	TagToAttribute.Add(Tags.Attribute_Base_Dexterity, GetDexterityAttribute);
	TagToAttribute.Add(Tags.Attribute_Base_Intelligence, GetIntelligenceAttribute);
	TagToAttribute.Add(Tags.Attribute_Base_Endurance, GetEnduranceAttribute);
	TagToAttribute.Add(Tags.Attribute_Base_Resilience, GetResilienceAttribute);
	TagToAttribute.Add(Tags.Attribute_Base_Defence, GetDefenceAttribute);

	/*Base Derived Attribute*/
	TagToAttribute.Add(Tags.Attribute_Derived_MaxHealth, GetMaxHealthAttribute);
	TagToAttribute.Add(Tags.Attribute_Derived_MaxMana, GetMaxManaAttribute);
	TagToAttribute.Add(Tags.Attribute_Derived_MaxEnergy, GetMaxEnergyAttribute);

	/*Vital*/
	TagToAttribute.Add(Tags.Attribute_Vital_Health, GetHealthAttribute);
	TagToAttribute.Add(Tags.Attribute_Vital_Mana, GetManaAttribute);
	TagToAttribute.Add(Tags.Attribute_Vital_Energy, GetEnergyAttribute);
	TagToAttribute.Add(Tags.Attribute_Vital_Food, GetFoodAttribute);
}

void UCharacterAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(UCharacterAttributeSet, Health, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UCharacterAttributeSet, MaxHealth, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UCharacterAttributeSet, Mana, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UCharacterAttributeSet, MaxMana, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UCharacterAttributeSet, Energy, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UCharacterAttributeSet, MaxEnergy, COND_None, REPNOTIFY_Always);

	DOREPLIFETIME_CONDITION_NOTIFY(UCharacterAttributeSet, Vitality, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UCharacterAttributeSet, Power, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UCharacterAttributeSet, Dexterity, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UCharacterAttributeSet, Intelligence, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UCharacterAttributeSet, Endurance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UCharacterAttributeSet, Resilience, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UCharacterAttributeSet, Defence, COND_None, REPNOTIFY_Always);

	DOREPLIFETIME_CONDITION_NOTIFY(UCharacterAttributeSet, MaxFood, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UCharacterAttributeSet, Food, COND_None, REPNOTIFY_Always);
}

void UCharacterAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);

	if (Attribute == GetHealthAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, GetMaxHealth());
	}
	if (Attribute == GetManaAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, GetMaxMana());
	}
	if (Attribute == GetEnergyAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, GetMaxEnergy());
	}
	if (Attribute == GetFoodAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, 100.f);
	}
	if (Attribute == GetCurrentResilienceAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, GetResilience());
	}
}

void UCharacterAttributeSet::SetEffectProps(const FGameplayEffectModCallbackData& Data, FEffectProperties& EffectProps)
{
	EffectProps.ContextHandle = Data.EffectSpec.GetContext();
	EffectProps.SourceASC = EffectProps.ContextHandle.GetInstigatorAbilitySystemComponent();

	if (IsValid(EffectProps.SourceASC) && EffectProps.SourceASC->AbilityActorInfo.IsValid() && EffectProps.SourceASC->AbilityActorInfo->AvatarActor.IsValid())
	{
		EffectProps.SourceAvatarActor = EffectProps.SourceASC->AbilityActorInfo->AvatarActor.Get();
		EffectProps.SourceController = EffectProps.SourceASC->AbilityActorInfo->PlayerController.Get();

		if (EffectProps.SourceAvatarActor != nullptr && EffectProps.SourceController == nullptr)
		{
			if (APawn* Pawn = Cast<APawn>(EffectProps.SourceAvatarActor))
			{
				EffectProps.SourceController = Pawn->GetController();
			}
		}

		if (EffectProps.SourceController != nullptr)
		{
			EffectProps.SourceCharacter = Cast<ACharacter>(EffectProps.SourceController->GetPawn());
		}

	}

	if (Data.Target.AbilityActorInfo.IsValid() && Data.Target.AbilityActorInfo->AvatarActor.IsValid())
	{
		EffectProps.TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Data.Target.AbilityActorInfo->AvatarActor.Get());
		EffectProps.TargetAvatarActor = Data.Target.AbilityActorInfo->AvatarActor.Get();
		EffectProps.TargetController = Data.Target.AbilityActorInfo->PlayerController.Get();
		EffectProps.TargetCharacter = Cast<ACharacter>(EffectProps.TargetAvatarActor);
	}
}

void UCharacterAttributeSet::AddHurtTag(FEffectProperties& EffectProps)
{
	FDungeonGameplayTags Tags = FDungeonGameplayTags::Get();

	FGameplayEffectContextHandle ContextHandle = EffectProps.SourceASC->MakeEffectContext();
	ContextHandle.AddSourceObject(EffectProps.SourceAvatarActor);

	UGameplayEffect* DebuffEffect = NewObject<UGameplayEffect>(GetTransientPackage(), FName("Hurt"));

	DebuffEffect->DurationPolicy = EGameplayEffectDurationType::HasDuration;
	DebuffEffect->DurationMagnitude = FScalableFloat(6.f);

	UTargetTagsGameplayEffectComponent& Component = DebuffEffect->FindOrAddComponent<UTargetTagsGameplayEffectComponent>();
	FInheritedTagContainer TagContainer = FInheritedTagContainer();
	TagContainer.Added.AddTag(Tags.Hurt);
	Component.SetAndApplyTargetTagChanges(TagContainer);

	DebuffEffect->StackingType = EGameplayEffectStackingType::AggregateByTarget;
	DebuffEffect->StackLimitCount = 1;
	DebuffEffect->StackDurationRefreshPolicy = EGameplayEffectStackingDurationPolicy::RefreshOnSuccessfulApplication;
	DebuffEffect->StackExpirationPolicy = EGameplayEffectStackingExpirationPolicy::ClearEntireStack;

	if (FGameplayEffectSpec* MutableSpec = new FGameplayEffectSpec(DebuffEffect, ContextHandle, 1.f))
	{
		EffectProps.TargetASC->ApplyGameplayEffectSpecToSelf(*MutableSpec);
	}
}

void UCharacterAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	FEffectProperties EffectProps;
	SetEffectProps(Data, EffectProps);

	if (Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		SetHealth(FMath::Clamp(GetHealth(), 0.f, GetMaxHealth()));
	}
	if (Data.EvaluatedData.Attribute == GetManaAttribute())
	{
		SetMana(FMath::Clamp(GetMana(), 0.f, GetMaxMana()));
	}
	if (Data.EvaluatedData.Attribute == GetEnergyAttribute())
	{
		SetEnergy(FMath::Clamp(GetEnergy(), 0.f, GetMaxEnergy()));

		if (GetEnergy() == 0.f)
		{
			UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(EffectProps.TargetAvatarActor, FDungeonGameplayTags::Get().Ability_Run_End, FGameplayEventData());
		}
	}
	if (Data.EvaluatedData.Attribute == GetFoodAttribute())
	{
		SetFood(FMath::Clamp(GetFood(), 0.f, 100.f));
	}
	if (Data.EvaluatedData.Attribute == GetCurrentResilienceAttribute())
	{
		SetCurrentResilience(FMath::Clamp(GetCurrentResilience(), 0.f, GetResilience()));
	}

	if (Data.EvaluatedData.Attribute == GetIncomingDamageAttribute())
	{
		if (ICharacterInterface::Execute_GetIsDodge(EffectProps.TargetAvatarActor))return;

		float LocalDamage = GetIncomingDamage();
		SetIncomingDamage(0.f);

		if (LocalDamage > 0.f)
		{
			float NewHealth = (GetHealth() - LocalDamage);
			SetHealth(FMath::Clamp(NewHealth, 0.f, GetMaxHealth()));

			bool bDead = NewHealth <= 0.f;

			if (bDead)
			{
				ICharacterInterface::Execute_Die(EffectProps.TargetAvatarActor);
			}
			else
			{
				if (EffectProps.TargetCharacter == EffectProps.SourceCharacter)return;

				AddHurtTag(EffectProps);

				if (UDungeonAbilitySystemLibrary::IsBlocked(EffectProps.ContextHandle))
				{
					float EnduranceCut = UDungeonAbilitySystemLibrary::GetEnduranceCut(EffectProps.ContextHandle);

					float NewEnergy = GetEnergy() - EnduranceCut;
					SetEnergy(FMath::Clamp(NewEnergy, 0.f, GetMaxEnergy()));

					if (EffectProps.SourceASC->HasMatchingGameplayTag(FDungeonGameplayTags::Get().Ability_Break))
					{
						IEnemyInterface::Execute_DicardCurrentWeapon(EffectProps.TargetAvatarActor, true);
						ICharacterInterface::Execute_SetIsBlocking(EffectProps.TargetAvatarActor, false);

						FGameplayEventData EventData;
						EventData.EventMagnitude = 1;
						UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(EffectProps.TargetAvatarActor, FDungeonGameplayTags::Get().Ability_HitReact, EventData);
						return;
					}

					if (UDungeonAbilitySystemLibrary::ShoudlReactBlock(EffectProps.ContextHandle))
					{
						if (NewEnergy <= 0)
						{
							//HitReact_Long
							FGameplayEventData EventData;
							EventData.EventMagnitude = 3;
							UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(EffectProps.TargetAvatarActor, FDungeonGameplayTags::Get().Ability_HitReact, EventData);
							return;
						}
						else
						{
							//HitReact_Blocking
							FGameplayEventData EventData;
							EventData.EventMagnitude = 2;
							UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(EffectProps.TargetAvatarActor, FDungeonGameplayTags::Get().Ability_HitReact, EventData);
							return;
						}
					}
				}
				else
				{
					float ResilienceCut = UDungeonAbilitySystemLibrary::GetResilienceCut(EffectProps.ContextHandle);
					float NewCurrentResilience = GetCurrentResilience() - ResilienceCut;
					SetCurrentResilience(FMath::Clamp(NewCurrentResilience, 0.f, GetResilience()));
					if (NewCurrentResilience <= 0)
					{
						//HitReact_Normal
						FGameplayEventData EventData;
						EventData.EventMagnitude = 1;
						UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(EffectProps.TargetAvatarActor, FDungeonGameplayTags::Get().Ability_HitReact, EventData);
						return;
					}
				}
			}
		}
	}
}

void UCharacterAttributeSet::Rep_Health(const FGameplayAttributeData& OldHealth) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCharacterAttributeSet, Health, OldHealth);
}

void UCharacterAttributeSet::Rep_MaxHealth(const FGameplayAttributeData& OldMaxHealth) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCharacterAttributeSet, MaxHealth, OldMaxHealth);
}

void UCharacterAttributeSet::Rep_Mana(const FGameplayAttributeData& OldMana) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCharacterAttributeSet, Mana, OldMana);
}

void UCharacterAttributeSet::Rep_MaxMana(const FGameplayAttributeData& OldMaxMana) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCharacterAttributeSet, MaxMana, OldMaxMana);
}

void UCharacterAttributeSet::Rep_Energy(const FGameplayAttributeData& OldEnergy) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCharacterAttributeSet, Energy, OldEnergy);
}

void UCharacterAttributeSet::Rep_MaxEnergy(const FGameplayAttributeData& OldMaxEnergy) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCharacterAttributeSet, MaxEnergy, OldMaxEnergy);
}

void UCharacterAttributeSet::Rep_Vitality(const FGameplayAttributeData& OldVitality) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCharacterAttributeSet, Vitality, OldVitality);
}

void UCharacterAttributeSet::Rep_Power(const FGameplayAttributeData& OldPower) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCharacterAttributeSet, Power, OldPower);
}

void UCharacterAttributeSet::Rep_Dexterity(const FGameplayAttributeData& OldDexterity) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCharacterAttributeSet, Dexterity, OldDexterity);
}

void UCharacterAttributeSet::Rep_Intelligence(const FGameplayAttributeData& OldIntelligence) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCharacterAttributeSet, Intelligence, OldIntelligence);
}

void UCharacterAttributeSet::Rep_Endurance(const FGameplayAttributeData& OldEndurance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCharacterAttributeSet, Endurance, OldEndurance);
}

void UCharacterAttributeSet::Rep_Resilience(const FGameplayAttributeData& OldResilience) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCharacterAttributeSet, Resilience, OldResilience);
}

void UCharacterAttributeSet::Rep_CurrentResilience(const FGameplayAttributeData& OldCurrentResilience) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCharacterAttributeSet, CurrentResilience, OldCurrentResilience);
}

void UCharacterAttributeSet::Rep_Defence(const FGameplayAttributeData& OldDefence) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCharacterAttributeSet, Defence, OldDefence);
}

void UCharacterAttributeSet::Rep_MaxFood(const FGameplayAttributeData& OldMaxFood) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCharacterAttributeSet, MaxFood, OldMaxFood);
}

void UCharacterAttributeSet::Rep_Food(const FGameplayAttributeData& OldFood) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCharacterAttributeSet, Food, OldFood);
}