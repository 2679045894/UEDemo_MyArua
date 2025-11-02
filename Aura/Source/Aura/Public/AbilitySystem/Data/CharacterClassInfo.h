// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffect.h"
#include "Engine/DataAsset.h"
#include "CharacterClassInfo.generated.h"

UENUM(BlueprintType)
enum class ECharacterClass : uint8
{
	Elementalist,
	Warrior,
	Ranger
};

USTRUCT(BlueprintType)
struct FCharacterClassDefault
{
	GENERATED_BODY()
	// 职业特定的 - 在结构体中
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly,Category="Class Default")
	TSubclassOf<UGameplayEffect> PrimaryAttributeClass;// 每个职业不同
};

UCLASS()
class AURA_API UCharacterClassInfo : public UDataAsset
{
	GENERATED_BODY()
public:
	// 全局共享的 - 在主类中

	//职业专属配置查找表
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly,Category="Class Default")
	TMap<ECharacterClass, FCharacterClassDefault> CharacterClassInformation;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly,Category="Class Default")// 所有职业都一样
	TSubclassOf<UGameplayEffect> SecondaryAttributeClass;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly,Category="Class Default")// 所有职业都一样
	TSubclassOf<UGameplayEffect> VitalAttributeClass;
	
	FCharacterClassDefault GetCharacterClassDefaultInfo(ECharacterClass CharacterClass);

	//为所有职业的角色提供一套通用的、共享的游戏能力。
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly,Category="Class Default")
	TArray<TSubclassOf<UGameplayAbility>> CommonAbilitiesClass;

	UPROPERTY(EditDefaultsOnly,Category="Class Default")
	TObjectPtr<UCurveTable> DamageCalculationCoefficients;
};
