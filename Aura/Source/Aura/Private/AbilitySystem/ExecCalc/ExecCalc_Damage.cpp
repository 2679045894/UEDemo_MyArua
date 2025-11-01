// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/ExecCalc/ExecCalc_Damage.h"

#include "AuraGameplayTags.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"

UExecCalc_Damage::UExecCalc_Damage()
{
	RelevantAttributesToCapture.Add(DamageStatics().ArmorDef);
	RelevantAttributesToCapture.Add(DamageStatics().BlockChanceDef);
	RelevantAttributesToCapture.Add(DamageStatics().ArmorPenetrationDef);
}

void UExecCalc_Damage::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
                                              FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	const UAbilitySystemComponent* SourceASC=ExecutionParams.GetSourceAbilitySystemComponent();
	const UAbilitySystemComponent* TargetASC=ExecutionParams.GetTargetAbilitySystemComponent();
	
    FAggregatorEvaluateParameters EvaluateParameters;
	const FGameplayEffectSpec& Spec=ExecutionParams.GetOwningSpec();
	EvaluateParameters.SourceTags=Spec.CapturedSourceTags.GetAggregatedTags();
	EvaluateParameters.TargetTags=Spec.CapturedTargetTags.GetAggregatedTags();
	float Damage=Spec.GetSetByCallerMagnitude(FAuraGameplayTags::Get().Damage);

	float TargetArmor=0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().ArmorDef,EvaluateParameters,TargetArmor);
	float SourceArmorPenetration=0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().ArmorPenetrationDef,EvaluateParameters,SourceArmorPenetration);

	float TargetBlockChance=0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().BlockChanceDef,EvaluateParameters,TargetBlockChance);
	if (const bool bBlocked=FMath::RandRange(1,100)<=TargetBlockChance)
	{
		Damage/=2.f;
	}
	const float EffectiveArmor=TargetArmor*(100-SourceArmorPenetration*0.25f)/100.f;
	Damage*=(100-EffectiveArmor*0.333f)/100.f;
	
	const FGameplayModifierEvaluatedData EvaluatedData(UAuraAttributeSet::GetIncomingDamageAttribute(),EGameplayModOp::Override,Damage);
	OutExecutionOutput.AddOutputModifier(EvaluatedData);
}


