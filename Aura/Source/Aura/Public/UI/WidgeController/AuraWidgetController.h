// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "Player/MyPlayerController.h"
#include "Player/MyPlayerState.h"
#include "AbilitySystem/Data/AbilityInfo.h"
#include "AuraWidgetController.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAttributeChangedSignature, float, NewValue);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAbilityInfoSignature,const FAuraAbilityInfo&,Info);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerStateChangedSignature,int32,NewValue);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAttributeInfoSignature,const FAuraAttributeInfo&,Info);
USTRUCT(Blueprintable)
struct FWidgetControllerParams
{
	GENERATED_BODY()
	//提供默认构造函数和参数化构造函数（构造函数初始化列表）
	FWidgetControllerParams(){}
	FWidgetControllerParams(APlayerController* PC,APlayerState* PS,UAbilitySystemComponent* ASC,UAttributeSet* AS)
	:PlayerController(PC),PlayerState(PS),AbilitySystemComponent(ASC),AttributeSet(AS){}
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TObjectPtr<APlayerController> PlayerController=nullptr;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TObjectPtr<APlayerState> PlayerState=nullptr;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent=nullptr;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TObjectPtr<UAttributeSet> AttributeSet=nullptr;
};
UCLASS()
class AURA_API UAuraWidgetController : public UObject
{
	GENERATED_BODY()
public:

	//将外部传入的结构体4个变量内容一一对应赋值
	UFUNCTION(BlueprintCallable)
	void SetWidgetController(const FWidgetControllerParams& WCParams);
	virtual void BroadcastInitialValues();
	virtual void BindCallbacksToDependencies();
	
	UPROPERTY(BlueprintReadOnly,Category="WidgetController")
	TObjectPtr<APlayerController> PlayerController;
	UPROPERTY(BlueprintReadOnly,Category="WidgetController")
	TObjectPtr<APlayerState> PlayerState;
	UPROPERTY(BlueprintReadOnly,Category="WidgetController")
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;
	UPROPERTY(BlueprintReadOnly,Category="WidgetController")
	TObjectPtr<UAttributeSet> AttributeSet;

	UPROPERTY()
	TObjectPtr<AMyPlayerController> AuraPlayerController;
	UPROPERTY()
	TObjectPtr<AMyPlayerState> AuraPlayerState;
	UPROPERTY()
	TObjectPtr<UAuraAbilitySystemComponent> AuraAbilitySystemComponent;
	UPROPERTY()
	TObjectPtr<UAuraAttributeSet> AuraAttributeSet;
	

	UPROPERTY(BlueprintAssignable,Category="GAS|Attributes")
	FAbilityInfoSignature AbilityInfoDelegate;

	UPROPERTY(BlueprintAssignable,Category="GAS|Attributes")
	FOnPlayerStateChangedSignature SpellPointChanged;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Widget Data")
	TObjectPtr<UAbilityInfo> AbilityInfo;

public:
	UFUNCTION()
	void BroadcastAbilityInfo();
	
	AMyPlayerController* GetAuraPlayerController();
	AMyPlayerState* GetAuraPlayerState();
	UAuraAbilitySystemComponent* GetAuraAbilitySystemComponent();
	UAuraAttributeSet* GetAuraAttributeSet();
	
	
};
	
