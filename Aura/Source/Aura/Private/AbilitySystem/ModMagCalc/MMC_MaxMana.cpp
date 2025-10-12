// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/ModMagCalc/MMC_MaxMana.h"

#include "AbilitySystem/AuraAttributeSet.h"
#include "Interaction/CombatInterface.h"
#include "Player/MyPlayerState.h"

UMMC_MaxMana::UMMC_MaxMana()
{
	IntDef.AttributeToCapture=UAuraAttributeSet::GetIntelligenceAttribute();
	IntDef.AttributeSource=EGameplayEffectAttributeCaptureSource::Target;
	IntDef.bSnapshot=false;
	RelevantAttributesToCapture.Add(IntDef);
}

float UMMC_MaxMana::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	const FGameplayTagContainer* SourceTags=Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags=Spec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvaluateParameters;
	EvaluateParameters.SourceTags=SourceTags;
	EvaluateParameters.TargetTags=TargetTags;

	float Intelligence=.0f;
	GetCapturedAttributeMagnitude(IntDef,Spec,EvaluateParameters,Intelligence);
	Intelligence=FMath::Max<float>(Intelligence,.0f);

	ICombatInterface* CombatInterface=Cast<ICombatInterface>(Spec.GetContext().GetSourceObject());
	check(CombatInterface);
	const int32 PlayerLevel=CombatInterface->GetPlayerLevel();
	return 50.f+2.5f*Intelligence+15.f*PlayerLevel;
}
