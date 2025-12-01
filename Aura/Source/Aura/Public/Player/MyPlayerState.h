// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/Data/LevelUpInfo.h"
#include "GameFramework/PlayerState.h"
#include "MyPlayerState.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnPlayerStateChanged,int32);

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
	UPROPERTY(BlueprintReadOnly,ReplicatedUsing=OnRep_XP,Category="Level")
	int32 XP=1;
	UPROPERTY(BlueprintReadOnly,ReplicatedUsing=OnRep_AttributePoints,Category="Level")
	int32 AttributePoints=3;
	UPROPERTY(BlueprintReadOnly,ReplicatedUsing=OnRep_SpellPoints,Category="Level")
	int32 SpellPoints=3;

	void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION()
	void OnRep_Level(int32 OldLevel) const;
	UFUNCTION()
	void OnRep_XP(int32 OldXP)const;
	UFUNCTION()
	void OnRep_AttributePoints(int32 OldAttributePoints)const;
	UFUNCTION()
	void OnRep_SpellPoints(int32 OldSpellPoints)const;
	
	//ForceInline 关键优化：强制编译器在调用点直接展开函数体代码，避免函数调用开销
	FORCEINLINE int32 GetPlayerLevel() const{return Level;}
	void AddToLevel(int32 InLevel);//增加等级
	void SetLevel(int32 InLevel);//设置当前等级
	
	FORCEINLINE int32 GetXP()const{return XP;}
	void AddToXP(int32 InXP);
	void SetXP(int32 InXP);

	void AddToAttributePoints(int32 InAttributePoints);
	void AddToSpellPoints(int32 InSpellPoints);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<ULevelUpInfo> LevelUpInfo;

	FOnPlayerStateChanged OnXPChangedDelegate;
	FOnPlayerStateChanged OnLevelChangedDelegate;

	FOnPlayerStateChanged OnAttributePointsChangedDelegate;
	FOnPlayerStateChanged OnSpellPointsChangedDelegate;

	FORCEINLINE int32 GetAttributePoints()const{ return AttributePoints; }
	FORCEINLINE int32 GetSpellPoints()const{ return SpellPoints; }
	
	
};
