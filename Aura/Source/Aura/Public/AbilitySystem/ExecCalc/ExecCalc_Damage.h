// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectExecutionCalculation.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "ExecCalc_Damage.generated.h"


struct AuraDamageStatics
{
	//声明一个属性捕获定义变量
	DECLARE_ATTRIBUTE_CAPTUREDEF(Armor);
	AuraDamageStatics()
	{
		//定义并初始化属性捕获配置
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet,Armor,Target,false);
	}
};

static const AuraDamageStatics& DamageStatics()
{
	static AuraDamageStatics DmgStatics;
	return DmgStatics;
}
UCLASS()
class AURA_API UExecCalc_Damage : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()
public:
	UExecCalc_Damage();
	
	virtual void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;
	
};
