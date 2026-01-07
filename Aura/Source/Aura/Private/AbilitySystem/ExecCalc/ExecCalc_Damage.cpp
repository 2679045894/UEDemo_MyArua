// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/ExecCalc/ExecCalc_Damage.h"

#include "AuraGameplayTags.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "AbilitySystem/Data/CharacterClassInfo.h"
#include "Interaction/CombatInterface.h"
#include "kismet/GameplayStatics.h"

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

/*void UExecCalc_Damage::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
                                              FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	TMap<FGameplayTag, FGameplayEffectAttributeCaptureDefinition> TagsToCaptureDefs; 
	const FAuraGameplayTags& Tags=FAuraGameplayTags::Get();
	TagsToCaptureDefs.Add(Tags.Attributes_Secondary_Armor,DamageStatics().ArmorDef);
	TagsToCaptureDefs.Add(Tags.Attributes_Secondary_BlockChance,DamageStatics().BlockChanceDef);
	TagsToCaptureDefs.Add(Tags.Attributes_Secondary_ArmorPenetration,DamageStatics().ArmorPenetrationDef);
	TagsToCaptureDefs.Add(Tags.Attributes_Secondary_CriticalHitChancel,DamageStatics().CriticalHitChanceDef);
	TagsToCaptureDefs.Add(Tags.Attributes_Secondary_CriticalHitResistance,DamageStatics().CriticalHitResistanceDef);
	TagsToCaptureDefs.Add(Tags.Attributes_Secondary_CriticalHitDamage,DamageStatics().CriticalHitDamageDef);
	TagsToCaptureDefs.Add(Tags.Attributes_Resistance_Fire,DamageStatics().FireResistanceDef);
	TagsToCaptureDefs.Add(Tags.Attributes_Resistance_Lightning,DamageStatics().LightningResistanceDef);
	TagsToCaptureDefs.Add(Tags.Attributes_Resistance_Arcane,DamageStatics().ArcaneResistanceDef);
	TagsToCaptureDefs.Add(Tags.Attributes_Resistance_Physical,DamageStatics().PhysicalResistanceDef);
	
	UAbilitySystemComponent* SourceASC=ExecutionParams.GetSourceAbilitySystemComponent();
	UAbilitySystemComponent* TargetASC=ExecutionParams.GetTargetAbilitySystemComponent();

	AActor* SourceAvatar=SourceASC?SourceASC->GetAvatarActor():nullptr;
	AActor* TargetAvatar=TargetASC?TargetASC->GetAvatarActor():nullptr;
	
	ICombatInterface* SourceCombatInterface=Cast<ICombatInterface>(SourceAvatar);
	ICombatInterface* TargetCombatInterface=Cast<ICombatInterface>(TargetAvatar);
	if (SourceAvatar==nullptr||TargetAvatar==nullptr||SourceCombatInterface==nullptr||TargetCombatInterface==nullptr)return;
	
    FAggregatorEvaluateParameters EvaluateParameters;
	const FGameplayEffectSpec& Spec=ExecutionParams.GetOwningSpec();
	EvaluateParameters.SourceTags=Spec.CapturedSourceTags.GetAggregatedTags();
	EvaluateParameters.TargetTags=Spec.CapturedTargetTags.GetAggregatedTags();

	DetermineDebuff(ExecutionParams, Spec, EvaluateParameters,TagsToCaptureDefs);


	
	//通过标签获取目标值
	/*float Damage=Spec.GetSetByCallerMagnitude(FAuraGameplayTags::Get().Damage);#1#
	float Damage=0.f;
	for (const auto &Pair:FAuraGameplayTags::Get().DamageTypesToResistance)
	{
		FGameplayTag DamageTypeTag=Pair.Key;
		FGameplayTag DamageResistanceTag=Pair.Value;
		checkf(TagsToCaptureDefs.Contains(DamageResistanceTag),TEXT("TagsToCaptureDefs does not contain Tag"));

		FGameplayEffectAttributeCaptureDefinition CaptureDef=TagsToCaptureDefs[DamageResistanceTag];
		//通过标签获取目标值
		float DamageTypeValue=Spec.GetSetByCallerMagnitude(DamageTypeTag);
		float Resistance=0.f;
		//尝试捕获目标的护甲属性值，结果存储在Resistance 变量中
		ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(CaptureDef,EvaluateParameters,Resistance);
		DamageTypeValue=((100.f-Resistance)/100.f)*DamageTypeValue;
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
	float DamageTypeValue=0.f;
	if (UAuraAbilitySystemLibrary::IsRadiaDamage(ContextHandle))
	{
		if (ICombatInterface* CombatInterface=Cast<ICombatInterface>(TargetAvatar))
		{
			CombatInterface->GetOnDamageDelegate().AddLambda([&](float DamageAmount)
			{
				DamageTypeValue=DamageAmount;
			});
		}
		UGameplayStatics::ApplyRadialDamageWithFalloff(
			TargetAvatar,
			DamageTypeValue,
			0,
			UAuraAbilitySystemLibrary::GetRadiaDamageOrigin(ContextHandle),
			UAuraAbilitySystemLibrary::GetRadiaDamageInnerRadius(ContextHandle),
			UAuraAbilitySystemLibrary::GetRadiaDamageOuterRadius(ContextHandle),
			0,
			UDamageType::StaticClass(),
			TArray<AActor*>(),
			SourceAvatar,
			nullptr);
	}
	
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
}*/

void UExecCalc_Damage::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
                                              FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
    TMap<FGameplayTag, FGameplayEffectAttributeCaptureDefinition> TagsToCaptureDefs; 
    const FAuraGameplayTags& Tags = FAuraGameplayTags::Get();
    TagsToCaptureDefs.Add(Tags.Attributes_Secondary_Armor, DamageStatics().ArmorDef);
    TagsToCaptureDefs.Add(Tags.Attributes_Secondary_BlockChance, DamageStatics().BlockChanceDef);
    TagsToCaptureDefs.Add(Tags.Attributes_Secondary_ArmorPenetration, DamageStatics().ArmorPenetrationDef);
    TagsToCaptureDefs.Add(Tags.Attributes_Secondary_CriticalHitChancel, DamageStatics().CriticalHitChanceDef);
    TagsToCaptureDefs.Add(Tags.Attributes_Secondary_CriticalHitResistance, DamageStatics().CriticalHitResistanceDef);
    TagsToCaptureDefs.Add(Tags.Attributes_Secondary_CriticalHitDamage, DamageStatics().CriticalHitDamageDef);
    TagsToCaptureDefs.Add(Tags.Attributes_Resistance_Fire, DamageStatics().FireResistanceDef);
    TagsToCaptureDefs.Add(Tags.Attributes_Resistance_Lightning, DamageStatics().LightningResistanceDef);
    TagsToCaptureDefs.Add(Tags.Attributes_Resistance_Arcane, DamageStatics().ArcaneResistanceDef);
    TagsToCaptureDefs.Add(Tags.Attributes_Resistance_Physical, DamageStatics().PhysicalResistanceDef);
    
    UAbilitySystemComponent* SourceASC = ExecutionParams.GetSourceAbilitySystemComponent();
    UAbilitySystemComponent* TargetASC = ExecutionParams.GetTargetAbilitySystemComponent();

    AActor* SourceAvatar = SourceASC ? SourceASC->GetAvatarActor() : nullptr;
    AActor* TargetAvatar = TargetASC ? TargetASC->GetAvatarActor() : nullptr;
    
    ICombatInterface* SourceCombatInterface = Cast<ICombatInterface>(SourceAvatar);
    ICombatInterface* TargetCombatInterface = Cast<ICombatInterface>(TargetAvatar);
    
    // 更详细的空指针检查
    if (!SourceAvatar || !TargetAvatar || !SourceCombatInterface || !TargetCombatInterface)
    {
        UE_LOG(LogTemp, Warning, TEXT("UExecCalc_Damage: Invalid actors or combat interfaces"));
        return;
    }
    
    FAggregatorEvaluateParameters EvaluateParameters;
    const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();
    EvaluateParameters.SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
    EvaluateParameters.TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

    DetermineDebuff(ExecutionParams, Spec, EvaluateParameters, TagsToCaptureDefs);

    // 通过标签获取目标值
    float Damage = 0.f;
    for (const auto &Pair : FAuraGameplayTags::Get().DamageTypesToResistance)
    {
        FGameplayTag DamageTypeTag = Pair.Key;
        FGameplayTag DamageResistanceTag = Pair.Value;
        checkf(TagsToCaptureDefs.Contains(DamageResistanceTag), TEXT("TagsToCaptureDefs does not contain Tag"));

        FGameplayEffectAttributeCaptureDefinition CaptureDef = TagsToCaptureDefs[DamageResistanceTag];
        // 通过标签获取目标值
        float DamageTypeValue = Spec.GetSetByCallerMagnitude(DamageTypeTag);
        float Resistance = 0.f;
        // 尝试捕获目标的护甲属性值，结果存储在Resistance变量中
        ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(CaptureDef, EvaluateParameters, Resistance);
        DamageTypeValue = ((100.f - Resistance) / 100.f) * DamageTypeValue;
        Damage += DamageTypeValue;
    }
    
    float TargetArmor = 0.f;
    // 尝试捕获目标的护甲属性值，结果存储在Armor变量中
    ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().ArmorDef, EvaluateParameters, TargetArmor);
    float SourceArmorPenetration = 0.f;
    ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().ArmorPenetrationDef, EvaluateParameters, SourceArmorPenetration);
    float TargetBlockChance = 0.f;
    ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().BlockChanceDef, EvaluateParameters, TargetBlockChance);
    float SourceCriticalHitChance = 0.f;
    ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().CriticalHitChanceDef, EvaluateParameters, SourceCriticalHitChance);
    float SourceCriticalHitDamage = 0.f;
    ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().CriticalHitDamageDef, EvaluateParameters, SourceCriticalHitDamage);
    float TargetCriticalHitResistance = 0.f;
    ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().CriticalHitResistanceDef, EvaluateParameters, TargetCriticalHitResistance);
    
    // ========== 这里是关键修复部分 ==========
    
    // 1. 检查CharacterClassInfo是否为空
    UCharacterClassInfo* CharacterClassInfo = UAuraAbilitySystemLibrary::GetCharacterClassInfo(TargetAvatar);
    if (!CharacterClassInfo)
    {
        UE_LOG(LogTemp, Error, TEXT("UExecCalc_Damage: CharacterClassInfo is null for TargetAvatar: %s"), 
            *GetNameSafe(TargetAvatar));
        return;
    }
    
    // 2. 检查DamageCalculationCoefficients是否为空
    if (!CharacterClassInfo->DamageCalculationCoefficients)
    {
        UE_LOG(LogTemp, Error, TEXT("UExecCalc_Damage: DamageCalculationCoefficients is null in CharacterClassInfo: %s"), 
            *GetNameSafe(CharacterClassInfo));
        return;
    }
    
    // 3. 安全地获取曲线 - 添加空指针检查
    FRealCurve* ArmorPenetrationCurve = CharacterClassInfo->DamageCalculationCoefficients->FindCurve(FName("ArmorPenetration"), FString());
    if (!ArmorPenetrationCurve)
    {
        UE_LOG(LogTemp, Warning, TEXT("UExecCalc_Damage: ArmorPenetration curve not found, using default value"));
        return;
    }
    
    // 4. 安全地获取等级
    int32 SourceLevel = 1;
    if (SourceAvatar->Implements<UCombatInterface>())
    {
        SourceLevel = ICombatInterface::Execute_GetPlayerLevel(SourceAvatar);
        SourceLevel = FMath::Max(SourceLevel, 1); // 确保至少为1
    }
    
    // 5. 安全地评估曲线
    const float ArmorPenetrationCoefficient = ArmorPenetrationCurve->Eval(SourceLevel);
    const float EffectiveArmor = TargetArmor * (100 - SourceArmorPenetration * ArmorPenetrationCoefficient) / 100.f;
    
    // 6. 安全地获取其他曲线
    FRealCurve* EffectiveArmorCurve = CharacterClassInfo->DamageCalculationCoefficients->FindCurve(FName("EffectiveArmor"), FString());
    if (!EffectiveArmorCurve)
    {
        UE_LOG(LogTemp, Warning, TEXT("UExecCalc_Damage: EffectiveArmor curve not found, using default value"));
        return;
    }
    
    // 7. 安全地获取目标等级
    int32 TargetLevel = 1;
    if (TargetAvatar->Implements<UCombatInterface>())
    {
        TargetLevel = ICombatInterface::Execute_GetPlayerLevel(TargetAvatar);
        TargetLevel = FMath::Max(TargetLevel, 1); // 确保至少为1
    }
    
    const float EffectiveArmorCoefficient = EffectiveArmorCurve->Eval(TargetLevel);

    FGameplayEffectContextHandle ContextHandle = Spec.GetContext();
    float DamageTypeValue = 0.f;
    if (UAuraAbilitySystemLibrary::IsRadiaDamage(ContextHandle))
    {
        if (ICombatInterface* CombatInterface = Cast<ICombatInterface>(TargetAvatar))
        {
            CombatInterface->GetOnDamageDelegate().AddLambda([&](float DamageAmount)
            {
                DamageTypeValue = DamageAmount;
            });
        }
        UGameplayStatics::ApplyRadialDamageWithFalloff(
            TargetAvatar,
            DamageTypeValue,
            0,
            UAuraAbilitySystemLibrary::GetRadiaDamageOrigin(ContextHandle),
            UAuraAbilitySystemLibrary::GetRadiaDamageInnerRadius(ContextHandle),
            UAuraAbilitySystemLibrary::GetRadiaDamageOuterRadius(ContextHandle),
            0,
            UDamageType::StaticClass(),
            TArray<AActor*>(),
            SourceAvatar,
            nullptr);
    }
    
    // 格挡
    const bool bBlocked = FMath::RandRange(1, 100) <= TargetBlockChance;
    UAuraAbilitySystemLibrary::SetIsBlockedHit(ContextHandle, bBlocked);
    if (bBlocked)
    {
        Damage /= 2;
    }
    
    // 护甲和穿透
    Damage *= (100 - EffectiveArmor * EffectiveArmorCoefficient) / 100.f;
    
    // 暴击
    FRealCurve* TargetCriticalHitResistanceCurve = CharacterClassInfo->DamageCalculationCoefficients->FindCurve(FName("CriticalHitResistance"), FString());
    if (!TargetCriticalHitResistanceCurve)
    {
        UE_LOG(LogTemp, Warning, TEXT("UExecCalc_Damage: CriticalHitResistance curve not found, using default value"));
        return;
    }
    
    const float CriticalHitResistanceCoefficient = TargetCriticalHitResistanceCurve->Eval(TargetLevel);
    bool bCriticalHit = FMath::RandRange(1, 100) <= SourceCriticalHitChance;
    UAuraAbilitySystemLibrary::SetIsCriticalHit(ContextHandle, bCriticalHit);
    
    if (bCriticalHit)
    {
        Damage = Damage * 2 + SourceCriticalHitDamage - TargetCriticalHitResistance * CriticalHitResistanceCoefficient;
    }
    
    // 创建修改器评估数据
    const FGameplayModifierEvaluatedData EvaluatedData(UAuraAttributeSet::GetIncomingDamageAttribute(), EGameplayModOp::Override, Damage);
    // 将修改器添加到输出中
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
			/*FGameplayEffectAttributeCaptureDefinition CaptureDef=TagsToCaptureDefs[ResistanceType];
			ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(CaptureDef,EvaluationParameters,TargetDeBuffResistance);
			TargetDeBuffResistance=FMath::Clamp(TargetDeBuffResistance,0.f,100.f);*/

			//计算负面效果是否应用
			//float EffectiveDeBuffChance=SourceDeBuffChance*(100-TargetDeBuffResistance)/100.f;
			float EffectiveDeBuffChance=SourceDeBuffChance;
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


