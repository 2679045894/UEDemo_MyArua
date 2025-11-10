// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"  // 添加这行！
#include "Interaction/CombatInterface.h"
#include "MyCharacterBase.generated.h"

class  UGameplayAbility;

UCLASS()
class AURA_API AMyCharacterBase : public ACharacter,public IAbilitySystemInterface,public ICombatInterface
{
	GENERATED_BODY()

public:
	AMyCharacterBase();

protected:

	virtual void BeginPlay() override;

public:	

	virtual void Tick(float DeltaTime) override;


	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//必须实现这个函数，因为调用了接口
	virtual UAuraAbilitySystemComponent* GetAbilitySystemComponent() const override;

	//自定义的一个函数，用于外部访问私有变量（AttributeSet）
	UAttributeSet* GetAttributeSet() const {return AttributeSet;}

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TObjectPtr<USkeletalMeshComponent> Weapon;
protected:
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TObjectPtr<UAuraAbilitySystemComponent> AbilitySystemComponent;
	
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UAttributeSet> AttributeSet;

	//初始化属性_游戏效果
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="InitialAttributes")
	TSubclassOf<UGameplayEffect> DefaultPrimaryAttribute;
	//初始化次级属性
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="InitialAttributes")
	TSubclassOf<UGameplayEffect> DefaultSecondaryAttributes;
	//初始化重要属性
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="InitialAttribute")
	TSubclassOf<UGameplayEffect> DefaultVitalAttributes;
	
	virtual void InitialAbilityActorInfo();
	//初始化属性(应用初始化游戏效果)
	virtual void InitializeDefaultAttributes() const;
	void ApplyEffectToSelf(TSubclassOf<UGameplayEffect> GameplayEffectClass,float Level) const;


	//实现战斗接口的获取等级方法
	virtual int32 GetPlayerLevel() override;

	//用来存放角色在游戏开始时就自动拥有的技能
	UPROPERTY(EditAnywhere,Category="Abilities")
	TArray<TSubclassOf<UGameplayAbility>> StartupAbilities;

	//添加技能
	void AddCharacterAbilities() const;

	//武器插槽名(投射点)
	UPROPERTY(EditAnywhere,Category="Combat")
	FName WeaponTioSocketName;

	virtual FVector GetCombatSocketLocation_Implementation();

	virtual void Die() override;

	//物理模拟死亡效果
	UFUNCTION(NetMulticast,Reliable)
	virtual void MulticastHandleDeath();

	UPROPERTY(EditAnywhere,Category="Dissolve")
	TObjectPtr<UMaterialInstance> DissolveMaterialInstance;
	UPROPERTY(EditAnywhere,Category="Dissolve")
	TObjectPtr<UMaterialInstance> WeaponDissolveMaterialInstance;

	void Dissolve();

	UFUNCTION(BlueprintImplementableEvent)
	void StartDissolveTimeline(UMaterialInstanceDynamic* DynamicMaterialInstance);

	UFUNCTION(BlueprintImplementableEvent)
	void StartWeaponDissolveTimeline(UMaterialInstanceDynamic* DynamicMaterialInstance);
};