// Fill out your copyright notice in the Description page of Project Settings.


#include "MMC/MMC_MaxHealth.h"
#include "AbilitySystem/CharacterAttributeSet.h"//

UMMC_MaxHealth::UMMC_MaxHealth()
{
	VitalityDef.AttributeToCapture = UCharacterAttributeSet::GetVitalityAttribute();
	VitalityDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Source;
	VitalityDef.bSnapshot = false;

	RelevantAttributesToCapture.Add(VitalityDef);
}

float UMMC_MaxHealth::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters Params;
	Params.SourceTags = SourceTags;
	Params.TargetTags = TargetTags;

	float Vitality = 0.f;
	GetCapturedAttributeMagnitude(VitalityDef, Spec, Params, Vitality);
	Vitality = FMath::Max<float>(0.f, Vitality);

	return 40 + FMath::FloorToInt(Vitality / 5) * 20;
}