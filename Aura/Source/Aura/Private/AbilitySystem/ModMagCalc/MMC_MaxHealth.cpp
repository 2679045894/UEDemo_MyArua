// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/ModMagCalc/MMC_MaxHealth.h"

#include "AbilitySystem/AuraAttributeSet.h"
#include "Interaction/CombatInterface.h"

UMMC_MaxHealth::UMMC_MaxHealth()
{
	//指定捕获AuraSet中的Vigor属性
	VigorDef.AttributeToCapture=UAuraAttributeSet::GetVigorAttribute();
	//指定从哪个角色获取属性值，Target 表示计算自己的最大生命值时使用自己的活力值
	VigorDef.AttributeSource=EGameplayEffectAttributeCaptureSource::Target;
	//控制属性值的捕获时机  false (实时模式) 每次计算时都获取最新的活力值
	VigorDef.bSnapshot=false;
	// 向GAS系统注册这个属性捕获需求
	RelevantAttributesToCapture.Add(VigorDef);
}

float UMMC_MaxHealth::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const override
{
	//获取效果施放者和目标的游戏标签
	//意义: 标签可以影响计算逻辑，比如某些职业或状态可能有加成
	const FGameplayTagContainer* SourceTags=Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags=Spec.CapturedTargetTags.GetAggregatedTags();

	//创建属性评估的上下文参数  确保属性计算考虑到当前的游戏状态和标签
	FAggregatorEvaluateParameters EvaluateParameters;
	EvaluateParameters.SourceTags=SourceTags;
	EvaluateParameters.TargetTags=TargetTags;

	//实际获取角色的活力数值
	float Viogr=0.f;
	GetCapturedAttributeMagnitude(VigorDef,Spec,EvaluateParameters,Viogr);
	Viogr=FMath::Max<float>(Viogr,0.f);

	ICombatInterface* CombatInterface=Cast<ICombatInterface>(Spec.GetContext().GetSourceObject());
	check(CombatInterface);
	const int32 PlayerLevel=CombatInterface->GetPlayerLevel();
	return 80.f+2.5f*Viogr+10.f*PlayerLevel;
}
