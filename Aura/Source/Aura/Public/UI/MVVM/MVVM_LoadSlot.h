// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MVVMViewModelBase.h"
#include "Game/LoadScreenSaveGame.h"
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
	//切换存档显示的用户控件的委托
	UPROPERTY(BlueprintAssignable)
	FSetWidgetSwitcherIndex SetWidgetSwitcherIndex;

	void InitializeSlot()const;

	void SetSlotName(const FString& InSlotName);
	FString GetSlotName()const{return SlotName;}

	void SetPlayerName(const FString& InPlayerName);
	FString GetPlayerName()const{return PlayerName;}

	void SetMapName(const FString& InMapName);
	FString GetMapName()const{return MapName;}

	void SetPlayerLevel(const int32 InLevel);
	int32 GetPlayerLevel()const{return PlayerLevel;}

	//当前视图模型的索引，对应存档的索引
	UPROPERTY()
	int32 SlotIndex;

	//存储玩家出生点位置标签
	UPROPERTY()
	FName PlayerStartTag;

	//当前进入加载存档界时，此存档应该显示的用户控件界面
	UPROPERTY()
	TEnumAsByte<ESaveSlotStatus> LoadSlotStatus;
private:
	UPROPERTY(BlueprintReadOnly,FieldNotify,Setter,Getter,meta=(AllowPrivateAccess))
	FString SlotName;
	UPROPERTY(BlueprintReadOnly,FieldNotify,Setter,Getter,meta=(AllowPrivateAccess))
	FString PlayerName;
	UPROPERTY(BlueprintReadOnly,FieldNotify,Setter,Getter,meta=(AllowPrivateAccess))
	FString MapName;
	UPROPERTY(BlueprintReadOnly,FieldNotify,Setter,Getter,meta=(AllowPrivateAccess))
	int32 PlayerLevel;
};
