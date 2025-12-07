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

	RelevantAttributesToCapture.Add(DamageStatics().FireResistanceDef);
	RelevantAttributesToCapture.Add(DamageStatics().LightningResistanceDef);
	RelevantAttributesToCapture.Add(DamageStatics().ArcaneResistanceDef);
	RelevantAttributesToCapture.Add(DamageStatics().PhysicalResistanceDef);
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
	for (const auto &Pair:FAuraGameplayTags::Get().DamageTypesToResistance)
	{
		FGameplayTag DamageTypeTag=Pair.Key;
		FGameplayTag DamageResistanceTag=Pair.Value;
		checkf(AuraDamageStatics().TagsToCaptureDefs.Contains(DamageResistanceTag),TEXT("TagsToCaptureDefs does not contain Tag"));

		FGameplayEffectAttributeCaptureDefinition CaptureDef=AuraDamageStatics().TagsToCaptureDefs[DamageResistanceTag];
		//通过标签获取目标值
		float DamageTypeValue=Spec.GetSetByCallerMagnitude(DamageTypeTag);
		float Resistance=0.f;
		//尝试捕获目标的护甲属性值，结果存储在Resistance 变量中
		ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(CaptureDef,EvaluateParameters,Resistance);
		DamageTypeValue*=(100.f-Resistance)/100.f;
		Damage+=DamageTypeValue;
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
	const float ArmorPenetrationCoefficient=ArmorPenetrationCurve->Eval(SourceCombatInterface->Execute_GetPlayerLevel(SourceAvatar));
	const float EffectiveArmor=TargetArmor*(100-SourceArmorPenetration*ArmorPenetrationCoefficient)/100.f;
	
	FRealCurve* EffectiveArmorCurve=CharacterClassInfo->DamageCalculationCoefficients->FindCurve(FName("EffectiveArmor"),FString());
	const float EffectiveArmorCoefficient=EffectiveArmorCurve->Eval(TargetCombatInterface->Execute_GetPlayerLevel(TargetAvatar));

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
	const float CriticalHitResistanceCoefficient=TargetCriticalHitResistanceCurve->Eval(TargetCombatInterface->Execute_GetPlayerLevel(TargetAvatar));
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

void UExecCalc_Damage::DetermineDebuff(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
	const FGameplayEffectSpec& Spec, FAggregatorEvaluateParameters EvaluationParameters,
	const TMap<FGameplayTag, FGameplayEffectAttributeCaptureDefinition>& TagsToCaptureDefs) const
{
	FAuraGameplayTags GameplayTags=FAuraGameplayTags::Get();
	//遍历所有负面效果伤害类型，根据伤害类型是否赋值类判断是否需要应用负面效果
	for (auto & Pair:GameplayTags.DeBuffToResistance)
	{
		//获取负面效果伤害类型
		FGameplayTag DeBuffDamageType=Pair.Key;
		//获取到负面效果抵抗类型
		FGameplayTag ResistanceType=Pair.Value;
		float TypeDamage=Spec.GetSetByCallerMagnitude(DeBuffDamageType,false,-1.f);

		//如果负面效果设置了伤害，即使为0，也需要应用负面效果
		if (TypeDamage>-.5f)
		{
			//获取效果命中率
			float SourceDeBuffChance=Spec.GetSetByCallerMagnitude(GameplayTags.DeBuff_Chance,false,-1.f);

			//获取负面抵抗效果
			//计算目标收到的负面效果类型的抵抗
			float TargetDeBuffResistance=0.f;
			//检查对应的属性快照是否设置，防止报错
			checkf(TagsToCaptureDefs.Contains(ResistanceType),TEXT("无法获取属性快照"));
			//通过抗性标签获取属性快照的值
			FGameplayEffectAttributeCaptureDefinition CaptureDef=TagsToCaptureDefs[ResistanceType];
			ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(CaptureDef,EvaluationParameters,TargetDeBuffResistance);
			TargetDeBuffResistance=FMath::Clamp(TargetDeBuffResistance,0.f,100.f);

			//计算负面效果是否应用
			float EffectiveDeBuffChance=SourceDeBuffChance*(100-TargetDeBuffResistance)/100.f;
			if (bool bDeBuff=FMath::RandRange(1,100)<=EffectiveDeBuffChance)
			{
				FGameplayEffectContextHandle ContextHandle=Spec.GetContext();
				UAuraAbilitySystemLibrary::SetIsSuccessfulDeBuff(ContextHandle,true);
				float DeBuffDuration=Spec.GetSetByCallerMagnitude(GameplayTags.DeBuff_Duration,false,-1.f);
				float DeBuffFrequency=Spec.GetSetByCallerMagnitude(GameplayTags.DeBuff_Frequency,false,-1.f);
				//设置负面效果 伤害类型 伤害 持续时间 触发频率
				UAuraAbilitySystemLibrary::SetDeBuff(ContextHandle,DeBuffDamageType,TypeDamage,DeBuffDuration,DeBuffFrequency);
			}
		}
	}
}


