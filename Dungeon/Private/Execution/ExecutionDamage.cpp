// Fill out your copyright notice in the Description page of Project Settings.


#include "Execution/ExecutionDamage.h"
#include "AbilitySystem/CharacterAttributeSet.h"//
#include "AbilitySystem/DungeonAbilitySystemLibrary.h"//
#include "Data/DetailedEquipmemtInfo.h"//
#include "Interaction/CharacterInterface.h"//
#include "DungeonGameplayTags.h"//
#include "AbilitySystemBlueprintLibrary.h"//

struct TargetStatics
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(Power);
	DECLARE_ATTRIBUTE_CAPTUREDEF(Dexterity);
	DECLARE_ATTRIBUTE_CAPTUREDEF(Intelligence);
	DECLARE_ATTRIBUTE_CAPTUREDEF(Defence);
	TargetStatics()
	{
		DEFINE_ATTRIBUTE_CAPTUREDEF(UCharacterAttributeSet, Power, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UCharacterAttributeSet, Dexterity, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UCharacterAttributeSet, Intelligence, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UCharacterAttributeSet, Defence, Target, false);
	}
};

static TargetStatics& TS()
{
	static TargetStatics TS;
	return TS;
}

struct SourceStatics
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(Power);
	DECLARE_ATTRIBUTE_CAPTUREDEF(Dexterity);
	DECLARE_ATTRIBUTE_CAPTUREDEF(Intelligence);
	SourceStatics()
	{
		DEFINE_ATTRIBUTE_CAPTUREDEF(UCharacterAttributeSet, Power, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UCharacterAttributeSet, Dexterity, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UCharacterAttributeSet, Intelligence, Source, false);
	}
};

static SourceStatics& SS()
{
	static SourceStatics SS;
	return SS;
}

UExecutionDamage::UExecutionDamage()
{
	RelevantAttributesToCapture.Add(TS().PowerDef);
	RelevantAttributesToCapture.Add(TS().DexterityDef);
	RelevantAttributesToCapture.Add(TS().IntelligenceDef);
	RelevantAttributesToCapture.Add(TS().DefenceDef);

	RelevantAttributesToCapture.Add(SS().PowerDef);
	RelevantAttributesToCapture.Add(SS().DexterityDef);
	RelevantAttributesToCapture.Add(SS().IntelligenceDef);
}

void UExecutionDamage::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, 
	FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	UAbilitySystemComponent* SourceASC = ExecutionParams.GetSourceAbilitySystemComponent();
	UAbilitySystemComponent* TargetASC = ExecutionParams.GetTargetAbilitySystemComponent();

	AActor* SourceAvatar = SourceASC ? SourceASC->GetAvatarActor() : nullptr;
	AActor* TargerAvatar = SourceASC ? TargetASC->GetAvatarActor() : nullptr;

	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();

	const FGameplayTagContainer* SourceTag = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTag = Spec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvaluationParamters;
	EvaluationParamters.SourceTags = SourceTag;
	EvaluationParamters.TargetTags = TargetTag;

	FGameplayEffectContextHandle EffectContextHandle = Spec.GetContext();

	//
	FDungeonGameplayTags Tags = FDungeonGameplayTags::Get();
	UDetailedEquipmemtInfo* EquipmentInfomation = UDungeonAbilitySystemLibrary::GetEquipmentInfomation(SourceAvatar);

	/*Target*/
	float TargetPower = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(TS().PowerDef, EvaluationParamters, TargetPower);
	TargetPower = FMath::Max(0.f, TargetPower);

	float TargetDexterity = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(TS().DexterityDef, EvaluationParamters, TargetDexterity);
	TargetDexterity = FMath::Max(0.f, TargetDexterity);

	float TargetIntelligence = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(TS().IntelligenceDef, EvaluationParamters, TargetIntelligence);
	TargetIntelligence = FMath::Max(0.f, TargetIntelligence);

	float TargetDefence = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(TS().DefenceDef, EvaluationParamters, TargetDefence);
	TargetDefence = FMath::Max(0.f, TargetDefence);
	/*Target*/

	/*Source*/
	float SourcePower = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(SS().PowerDef, EvaluationParamters, SourcePower);
	SourcePower = FMath::Max(0.f, SourcePower);

	float SourceDexterity = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(SS().DexterityDef, EvaluationParamters, SourceDexterity);
	SourceDexterity = FMath::Max(0.f, SourceDexterity);

	float SourceIntelligence = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(SS().IntelligenceDef, EvaluationParamters, SourceIntelligence);
	SourceIntelligence = FMath::Max(0.f, SourceIntelligence);
	/*Source*/

	float Damage = 0.f;
	float ResilienceCut = 0.f;

	if (!UDungeonAbilitySystemLibrary::IsThrowable(EffectContextHandle))
	{
		FWeaponInfo AttackerWeapon = EquipmentInfomation->GetWeaponInfoForItemIndex(ICharacterInterface::Execute_GetWeaponIndex(SourceAvatar));

		Damage += AttackerWeapon.BaseDamage 
			+ AttackerWeapon.PowerCoefficient * SourcePower
			+ AttackerWeapon.DexterityCoefficient * SourceDexterity
			+ AttackerWeapon.IntelligenceCoefficient * SourceIntelligence;

		ResilienceCut = AttackerWeapon.ResilienceCut;

		if (ICharacterInterface::Execute_GetCurrentWeaponType(SourceAvatar) == Tags.Weapon_Melee)
		{
			if (ICharacterInterface::Execute_GetWeaponIndex(SourceAvatar) != -1)
			{
				if (ICharacterInterface::Execute_GetDurability(SourceAvatar, false) != 0)
				{
					ICharacterInterface::Execute_SetDurability(SourceAvatar, -1, false);
				}
				else
				{
					Damage *= 0.5;
					ResilienceCut *= 0.5;
				}
			}
		}
		else
		{
			if (ICharacterInterface::Execute_GetDurability(SourceAvatar, false) == 0)
			{
				Damage *= 0.5;
				ResilienceCut *= 0.5;
			}
		}
	}
	else
	{
		Damage += Spec.GetSetByCallerMagnitude(Tags.Throwable_BaseDamage)
			+ Spec.GetSetByCallerMagnitude(Tags.Throwable_PowCE) * SourcePower
			+ Spec.GetSetByCallerMagnitude(Tags.Throwable_DexCE) * SourceDexterity
			+ Spec.GetSetByCallerMagnitude(Tags.Throwable_IntCE) * SourceIntelligence;

		ResilienceCut = Spec.GetSetByCallerMagnitude(Tags.Throwable_ResilienceCut);

		if (Spec.GetSetByCallerMagnitude(Tags.Throwable_Durability) == 0)
		{
			Damage *= 0.5;
			ResilienceCut *= 0.5;
		}
	}

	float Avoidance = 0.f;
	if (UDungeonAbilitySystemLibrary::IsBlocked(EffectContextHandle))
	{
		FWeaponInfo Shield = EquipmentInfomation->GetWeaponInfoForItemIndex(ICharacterInterface::Execute_GetSecondaryWeaponIndex(TargerAvatar));
		Avoidance += Shield.AvoidanceRate 
			+ Shield.PowerCoefficient * SourcePower 
			+ Shield.DexterityCoefficient * SourceDexterity 
			+ Shield.IntelligenceCoefficient * SourceIntelligence;

		UDungeonAbilitySystemLibrary::SetEnduranceCut(EffectContextHandle, ResilienceCut * 2);

		if (ICharacterInterface::Execute_GetDurability(TargerAvatar, true) != 0)
		{
			ICharacterInterface::Execute_SetDurability(TargerAvatar, -1, true);
		}
		else
		{
			Avoidance *= 0.5;
		}
	}
	else
	{
		UDungeonAbilitySystemLibrary::SetResilienceCut(EffectContextHandle, ResilienceCut);
	}

	Damage = Damage * ((100 - Avoidance) / 100) - TargetDefence * 0.5;
	Damage = FMath::Max(0.f, Damage);

	FGameplayModifierEvaluatedData DamageData(UCharacterAttributeSet::GetIncomingDamageAttribute(), EGameplayModOp::Additive, Damage);
	OutExecutionOutput.AddOutputModifier(DamageData);
}