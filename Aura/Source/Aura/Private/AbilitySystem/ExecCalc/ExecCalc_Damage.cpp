// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/ExecCalc/ExecCalc_Damage.h"

#include "AbilitySystem/AuraAbilitySystemComponent.h"

UExecCalc_Damage::UExecCalc_Damage()
{
	RelevantAttributesToCapture.Add(DamageStatics().ArmorDef);
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
	float Armor=0.f;
	//尝试捕获目标的护甲属性值。结果存储在 Armor 变量中
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().ArmorDef,EvaluateParameters,Armor);
	Armor=FMath::Max<float>(0.f,Armor);
	++Armor;
	//创建修改器评估数据
	FGameplayModifierEvaluatedData EvaluatedData(DamageStatics().ArmorProperty,EGameplayModOp::Additive,Armor);
	//将修改器添加到输出中，这会导致目标的护甲属性实际增加1点。
	OutExecutionOutput.AddOutputModifier(EvaluatedData);
}
