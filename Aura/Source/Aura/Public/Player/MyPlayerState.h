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
	/**
	 * 
	 */
	UPROPERTY()
	//TObjectPtr<UAttributeSet> AttributeSet;
	TObjectPtr<UAuraAttributeSet> AttributeSet;
public:
	//封装，属性是私有的，外部只能通过这两个函数分别访问AbilitySystemComponent和AttributeSet变量
	virtual UAuraAbilitySystemComponent* GetAbilitySystemComponent() const override;
	UAuraAttributeSet* GetAttributeSet() const{ return AttributeSet; }

	//void CheckAttributeSetRegistration() const;
};
