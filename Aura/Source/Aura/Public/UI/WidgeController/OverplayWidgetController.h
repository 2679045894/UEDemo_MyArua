// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgeController/AuraWidgetController.h"
#include "UI/Widge/AuraUserWidget.h"
#include "OverplayWidgetController.generated.h"

USTRUCT(BlueprintType,Blueprintable)
struct FUIWidgetRow:public FTableRowBase
{
	GENERATED_BODY();
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	FGameplayTag MessageTag;
	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	FText Message;
	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	TSubclassOf<UAuraUserWidget> MessageWidget;
	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	UTexture2D* Image;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAttributeChangedSignature, float, NewValue);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMessageWidgetRowSignature,FUIWidgetRow,Row);

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class AURA_API UOverplayWidgetController : public UAuraWidgetController
{
	GENERATED_BODY()
public:
	//WidgetController 被创建后，系统自动调用这两个函数
	//确保 UI 控件在创建时就能显示正确的初始属性值
	virtual void BroadcastInitialValues() override;
	//绑定属性变化委托，建立属性变化时的回调机制
	virtual void BindCallbacksToDependencies() override;
	
	//多播委托定义  可在蓝图中bind
	UPROPERTY(BlueprintAssignable,Category="GAS|Attributes")
	FOnAttributeChangedSignature OnHealthChanged;

	UPROPERTY(BlueprintAssignable,Category="GAS|Attributes")
	FOnAttributeChangedSignature OnMaxHealthChanged;

	UPROPERTY(BlueprintAssignable,Category="GAS|Attributes")
	FOnAttributeChangedSignature OnManaChanged;

	UPROPERTY(BlueprintAssignable,Category="GAS|Attributes")
	FOnAttributeChangedSignature OnMaxManaChanged;

	UPROPERTY(BlueprintAssignable,Category="GAS|Message")
	FMessageWidgetRowSignature MessageWidgetRowDelegate;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	TObjectPtr<UDataTable> MessageWidgetDataTable;

	
protected:
	//通过标签名获取行命名信息（确保行命名和标签名一致）
	//传入目标数据表格以及当前标签
	template<typename T>
	T* GetDataTableRowByTag(UDataTable* DataTable,FGameplayTag Tag);
};

//模板函数  返回指针节省开销，如果返回的是结构体信息，内存占用较大
template <typename T>
T* UOverplayWidgetController::GetDataTableRowByTag(UDataTable* DataTable, FGameplayTag Tag)
{
	return DataTable->FindRow<T>(Tag.GetTagName(),TEXT("find failure"));
}

