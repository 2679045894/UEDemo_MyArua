// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MVVMViewModelBase.h"
#include "MVVM_LoadSlot.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSetWidgetSwitcherIndex,int32,SwitcherIndex);
/**
 * 
 */
UCLASS()
class AURA_API UMVVM_LoadSlot : public UMVVMViewModelBase
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintAssignable)
	FSetWidgetSwitcherIndex SetWidgetSwitcherIndex;

	void InitializeSlot()const;

	void SetSlotName(const FString& InSlotName);
	FString GetSlotName()const{return SlotName;}
private:
	UPROPERTY(BlueprintReadOnly,FieldNotify,Setter,Getter,meta=(AllowPrivateAccess))
	FString SlotName;
};
