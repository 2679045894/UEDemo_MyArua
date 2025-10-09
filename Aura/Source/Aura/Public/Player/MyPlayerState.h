// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "GameFramework/PlayerState.h"
#include "MyPlayerState.generated.h"

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class AURA_API AMyPlayerState : public APlayerState,public IAbilitySystemInterface
{
	GENERATED_BODY()
	AMyPlayerState();

protected:
	UPROPERTY(EditAnywhere,BlueprintReadOnly);
	TObjectPtr<UAuraAbilitySystemComponent> AbilitySystemComponent;
	
	UPROPERTY()
	//TObjectPtr<UAttributeSet> AttributeSet;
	TObjectPtr<UAuraAttributeSet> AttributeSet;

	
public:
	//封装，属性是私有的，外部只能通过这两个函数分别访问AbilitySystemComponent和AttributeSet变量
	virtual UAuraAbilitySystemComponent* GetAbilitySystemComponent() const override;
	UAuraAttributeSet* GetAttributeSet() const{ return AttributeSet; }

	UPROPERTY(BlueprintReadOnly,ReplicatedUsing=OnRep_Level,Category="Level")
	int32 Level=1;

	void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION()
	void OnRep_Level(int32 OldLevel) const;
	
	//forceinling 关键优化：强制编译器在调用点直接展开函数体代码，避免函数调用开销
	FORCEINLINE int32 GetPlayerLevel() const{return Level;}
};
