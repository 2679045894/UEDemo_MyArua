// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Data/AttributeInfo.h"
#include "UI/WidgeController/AuraWidgetController.h"
#include "AttributeMenuWidgetController.generated.h"


/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class AURA_API UAttributeMenuWidgetController : public UAuraWidgetController
{
	GENERATED_BODY()
public:
	virtual void BindCallbacksToDependencies() override;

	UFUNCTION(BlueprintCallable)
	virtual void BroadcastInitialValues() override;

	UPROPERTY(BlueprintAssignable,Category="GAS|Attributes")
	FAttributeInfoSignature AttributeInfoDelegate;

	//外部添加AuraAttributeInfo(DataAsset)
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UAttributeInfo> AuraAttributeInfo;
	
	UPROPERTY(BlueprintAssignable,Category="GAS|Attributes")
	FOnPlayerStateChangedSignature OnAttributePointChangedDelegate;

	UPROPERTY(BlueprintAssignable,Category="GAS|Attirbutes")
	FOnPlayerStateChangedSignature OnSpellPointChangedDelegate;
	
	void BroadcastAttributeInfo(const FGameplayTag& AttributeTag, const FGameplayAttribute& Attribute) const;

	UFUNCTION(BlueprintCallable,Category="GAS|Attributes")
	void UpgradeAttribute(const FGameplayTag& AttributeTag);
	
};
