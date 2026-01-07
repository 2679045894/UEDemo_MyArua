// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "ListenCooldownChange.generated.h"

struct FGameplayAbilitySpec;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCooldownChangeSignature,float,TimeRemaining);
/**
 * 
 */
UCLASS(BlueprintType,meta=(ExposedAsyncProxy="AsyncTask"))
class AURA_API UListenCooldownChange : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintAssignable)
	FCooldownChangeSignature CooldownStart;

	UPROPERTY(BlueprintAssignable)
	FCooldownChangeSignature CooldownEnd;

	UFUNCTION(BlueprintCallable,meta=(BlueprintInternalUseOnly="true"))
	static UListenCooldownChange* ListenForCooldownChange(UAuraAbilitySystemComponent* AbilitySystemComponent,const FGameplayTag InCooldownTag);

	UFUNCTION(BlueprintCallable)
	void EndTask();

protected:
	UPROPERTY()
	TObjectPtr<UAuraAbilitySystemComponent> ASC;

	//记录监听的冷却标签
	FGameplayTag CooldownTag;

	//监听冷却标签回调函数
	void CooldownTagChanged(const FGameplayTag InCooldownTag,int32 NewCount);

	//监听ASC激活GE的回调
	void OnActiveEffectAdded(UAbilitySystemComponent* TargetASC, const FGameplayEffectSpec& SpecApplied, FActiveGameplayEffectHandle ActiveEffectHandle);

	bool bIsFirstAttempt=false;
};
