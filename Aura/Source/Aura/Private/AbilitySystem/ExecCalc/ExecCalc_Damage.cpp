// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/ExecCalc/ExecCalc_Damage.h"

#include "AuraGameplayTags.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "AbilitySystem/Data/CharacterClassInfo.h"
#include "Interaction/CombatInterface.h"

UExecCalc_Damage::UExecCalc_Damage()
{
	RelevantAttributesToCapture.Add(DamageStatics().ArmorDef);
	//格挡
	RelevantAttributesToCapture.Add(DamageStatics().BlockChanceDef);
	RelevantAttributesToCapture.Add(DamageStatics().ArmorPenetrationDef);
	RelevantAttributesToCapture.Add(DamageStatics().CriticalHitChanceDef);
	RelevantAttributesToCapture.Add(DamageStatics().CriticalHitDamageDef);
	RelevantAttributesToCapture.Add(DamageStatics().CriticalHitResistanceDef);
}

void UExecCalc_Damage::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
                                              FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	UAbilitySystemComponent* SourceASC=ExecutionParams.GetSourceAbilitySystemComponent();
	UAbilitySystemComponent* TargetASC=ExecutionParams.GetTargetAbilitySystemComponent();

	AActor* SourceAvatar=SourceASC?SourceASC->GetAvatarActor():nullptr;
	AActor* TargetAvatar=TargetASC?TargetASC->GetAvatarActor():nullptr;
	
	ICombatInterface* SourceCombatInterface=Cast<ICombatInterface>(SourceAvatar);
	ICombatInterface* TargetCombatInterface=Cast<ICombatInterface>(TargetAvatar);
	
    FAggregatorEvaluateParameters EvaluateParameters;
	const FGameplayEffectSpec& Spec=ExecutionParams.GetOwningSpec();
	EvaluateParameters.SourceTags=Spec.CapturedSourceTags.GetAggregatedTags();
	EvaluateParameters.TargetTags=Spec.CapturedTargetTags.GetAggregatedTags();
	//通过标签获取目标值
	/*float Damage=Spec.GetSetByCallerMagnitude(FAuraGameplayTags::Get().Damage);*/
	float Damage=0.f;
	for (FGameplayTag DamageTag:FAuraGameplayTags::Get().DamageTypes)
	{
		Damage+=Spec.GetSetByCallerMagnitude(DamageTag.GetTagName());
	}
	float TargetArmor=0.f;
	//尝试捕获目标的护甲属性值，结果存储在 Armor 变量中
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().ArmorDef,EvaluateParameters,TargetArmor);
	float SourceArmorPenetration=0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().ArmorPenetrationDef,EvaluateParameters,SourceArmorPenetration);
	float TargetBlockChance=0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().BlockChanceDef,EvaluateParameters,TargetBlockChance);
	float SourceCriticalHitChance=0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().CriticalHitChanceDef,EvaluateParameters,SourceCriticalHitChance);
	float SourceCriticalHitDamage=0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().CriticalHitDamageDef,EvaluateParameters,SourceCriticalHitDamage);
	float TargetCriticalHitResistance=0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().CriticalHitResistanceDef,EvaluateParameters,TargetCriticalHitResistance);
	
	UCharacterClassInfo* CharacterClassInfo=UAuraAbilitySystemLibrary::GetCharacterClassInfo(TargetAvatar);
	
	FRealCurve* ArmorPenetrationCurve=CharacterClassInfo->DamageCalculationCoefficients->FindCurve(FName("ArmorPenetration"),FString());
	const float ArmorPenetrationCoefficient=ArmorPenetrationCurve->Eval(SourceCombatInterface->GetPlayerLevel());
	const float EffectiveArmor=TargetArmor*(100-SourceArmorPenetration*ArmorPenetrationCoefficient)/100.f;
	
	FRealCurve* EffectiveArmorCurve=CharacterClassInfo->DamageCalculationCoefficients->FindCurve(FName("EffectiveArmor"),FString());
	const float EffectiveArmorCoefficient=EffectiveArmorCurve->Eval(TargetCombatInterface->GetPlayerLevel());

	FGameplayEffectContextHandle ContextHandle=Spec.GetContext();

	//格挡
	const bool bBlocked=FMath::RandRange(1,100)<=TargetBlockChance;
	UAuraAbilitySystemLibrary::SetIsBlockedHit(ContextHandle,bBlocked);
	if (bBlocked)
	{
		Damage/=2;
	}
	//护甲和穿透
	Damage*=(100-EffectiveArmor*EffectiveArmorCoefficient)/100.f;
	//暴击
	FRealCurve* TargetCriticalHitResistanceCurve=CharacterClassInfo->DamageCalculationCoefficients->FindCurve(FName("CriticalHitResistance"),FString());
	const float CriticalHitResistanceCoefficient=TargetCriticalHitResistanceCurve->Eval(TargetCombatInterface->GetPlayerLevel());
	bool bCriticalHit=FMath::RandRange(1,100)<=SourceCriticalHitChance;
	UAuraAbilitySystemLibrary::SetIsCriticalHit(ContextHandle,bCriticalHit);
	if (bCriticalHit)
	{
		Damage=Damage*2+SourceCriticalHitDamage-TargetCriticalHitResistance*CriticalHitResistanceCoefficient;
	}
	//创建修改器评估数据
	const FGameplayModifierEvaluatedData EvaluatedData(UAuraAttributeSet::GetIncomingDamageAttribute(),EGameplayModOp::Override,Damage);
	//将修改器添加到输出中
	OutExecutionOutput.AddOutputModifier(EvaluatedData);
}


