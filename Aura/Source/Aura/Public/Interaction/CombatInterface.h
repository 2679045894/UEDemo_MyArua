// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "NiagaraSystem.h"
#include "AbilitySystem/Data/CharacterClassInfo.h"
#include "UObject/Interface.h"
#include "CombatInterface.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnASCRegistered,UAbilitySystemComponent*);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDeath,AActor*,DeadActor);

class UAnimMontage;

USTRUCT(BlueprintType)
struct FTaggedMontage
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UAnimMontage* Montage = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag MontageTag;

	//部位对应的标签
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag SocketTag;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	USoundBase* ImpactSound;
	
	//技能发动位置
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FName CombatTipSocketName;
};
UINTERFACE(MinimalAPI, Blueprintable, BlueprintType)
class UCombatInterface : public UInterface
{
	GENERATED_BODY()
};


class AURA_API ICombatInterface
{
	GENERATED_BODY()

public:
	//通过结构体获取对应的骨骼插槽位置
	UFUNCTION(BlueprintCallable,BlueprintNativeEvent)
	FVector GetCombatSocketLocation(const FGameplayTag &MontageTag);

	UFUNCTION(BlueprintImplementableEvent,BlueprintCallable)
	void UpdateFacingTarget(const FVector& TargetLocation);

	UFUNCTION(BlueprintNativeEvent,BlueprintCallable)
	UAnimMontage* GetHitReactMontage();

	UFUNCTION()
	virtual void Die()=0;

	UFUNCTION(BlueprintNativeEvent,BlueprintCallable)
	bool IsDead();

	UFUNCTION(BlueprintNativeEvent,BlueprintCallable)
	AActor* GetAvatar();

	UFUNCTION(BlueprintNativeEvent,BlueprintCallable)
	TArray<FTaggedMontage> GetAttackMontages();
	
	UFUNCTION(BlueprintNativeEvent,BlueprintCallable)
	UNiagaraSystem* GetBloodEffect();

	UFUNCTION(BlueprintNativeEvent,BlueprintCallable)
	FTaggedMontage GetTaggedMontageByTag(const FGameplayTag& Tag);

	UFUNCTION(BlueprintNativeEvent,BlueprintCallable)
	int32 GetMinionCount();

	UFUNCTION(BlueprintNativeEvent,BlueprintCallable)
	void IncrementMinionCount(const int32 Amount);

	UFUNCTION(BlueprintNativeEvent,BlueprintCallable)
	ECharacterClass GetCharacterClass();

	UFUNCTION(BlueprintNativeEvent)
	int32 GetPlayerLevel();

	virtual FOnASCRegistered& GetOnASCRegistered()=0;

	virtual FOnDeath& GetOnDeathDelegate()=0;
};
