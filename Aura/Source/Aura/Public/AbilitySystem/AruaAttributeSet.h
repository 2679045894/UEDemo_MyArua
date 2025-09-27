// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AruaAttributeSet.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UAruaAttributeSet : public UAttributeSet
{
	GENERATED_BODY()
public:
	UAruaAttributeSet();
	//函数中定义哪些属性需要在网络中进行同步复制。
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	//当客户端从服务器接收到这个变量的更新时，不会立刻应用新值，而是会先调用这个指定的函数
	UPROPERTY(BlueprintReadOnly,ReplicatedUsing=OnRep_Health,Category="Vital Attribute")
	FGameplayAttributeData Health;
	UPROPERTY(BlueprintReadOnly,ReplicatedUsing=OnRep_MaxHealth,Category="Vital Attribute")
	FGameplayAttributeData MaxHealth;
	UPROPERTY(BlueprintReadOnly,ReplicatedUsing=Rep_Mana,Category="Vital Attriubute")
	FGameplayAttributeData Mana;
	UPROPERTY(BlueprintReadOnly,ReplicatedUsing=Rep_MaxMana,Category="Vital Attribute")
	FGameplayAttributeData MaxMana;

	//接收属性变化之前的值，变量(Health)更新时调用这个函数
	UFUNCTION()
	void OnRep_Health(const FGameplayAttributeData& OldHeath) const;
	UFUNCTION()
	void OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth) const;
	UFUNCTION()
	void Rep_Mana(const FGameplayAttributeData& OldMana) const;
	UFUNCTION()
	void Rep_MaxMana(const FGameplayAttributeData& OldMana) const;
};
