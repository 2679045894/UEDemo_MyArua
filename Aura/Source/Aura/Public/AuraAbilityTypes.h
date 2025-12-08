#pragma once
#pragma once

#include "GameplayEffect.h"
#include "GameplayEffectTypes.h"
#include "AuraAbilityTypes.generated.h"

USTRUCT(BlueprintType)
struct FAuraGameplayContext:public FGameplayEffectContext
{
	GENERATED_BODY();
public:
	//访问器方法 提供安全的布尔值访问，封装内部实现
	bool IsCriticalHit() const {return bIsCriticalHit;}
	bool IsBlockedHit() const {return bIsBlockedHit;}
	bool IsSuccessfulDeBuff() const {return bIsSuccessfulDeBuff;}
	float GetDeBuffDamage() const {return DeBuffDamage;}
	float GetDeBuffDuration() const {return DeBuffDuration;}
	float GetDeBuffFrequency() const {return DeBuffFrequency;}
	TSharedPtr<FGameplayTag> GetDeBuffDamageTypeTag() const {return DeBuffDamageType;}
	FVector GetDeathImpulse() const{return DeathImpulse;}
	//设置器方法 允许其他系统设置这些标志
	void SetIsCriticalHit(bool bInIsCriticalHit){bIsCriticalHit=bInIsCriticalHit;}
	void SetIsBlockedHit(bool bInIsBlockedHit){bIsBlockedHit=bInIsBlockedHit;}
	void SetIsSuccessfulDeBuff(bool bInIsSuccessfulDeBuff){bIsSuccessfulDeBuff=bInIsSuccessfulDeBuff;}
	void SetDeBuffDamage(float InDeBuffDamage){DeBuffDamage=InDeBuffDamage;}
	void SetDeBuffDuration(float InDeBuffDuration){DeBuffDuration=InDeBuffDuration;}
	void SetDeBuffFrequency(float InDeBuffFrequency){DeBuffFrequency=InDeBuffFrequency;}
	void SetDeBuffDamageType(TSharedPtr<FGameplayTag>& InDamageType){DeBuffDamageType=InDamageType;}
	void SetDeathImpulse(FVector& InDeathImpulse){DeathImpulse=InDeathImpulse;}
	

	//返回此上下文结构体的类型信息  序列化需要  重新写一遍逻辑(父类中有相同的方法，可以直接调用)
	virtual UScriptStruct* GetScriptStruct() const
	{
		return  FGameplayEffectContext::StaticStruct();
	}

	//核心目的：提供多态深拷贝。  重新写一遍逻辑(父类中有相同的方法，可以直接调用)
	virtual FGameplayEffectContext* Duplicate() const;

	//自己实现   实现 FGameplayEffectContext 对象的网络同步。
	virtual bool NetSerialize(FArchive& Ar, class UPackageMap* Map, bool& bOutSuccess) override;

private:
	UPROPERTY()
	bool bIsBlockedHit=false;
	UPROPERTY()
	bool bIsCriticalHit=false;
	UPROPERTY()
	bool bIsSuccessfulDeBuff=false;//成功应用负面效果
	UPROPERTY()
	float DeBuffDamage=0.f;
	UPROPERTY()
	float DeBuffDuration=0.f;
	UPROPERTY()
	float DeBuffFrequency=0.f;
	TSharedPtr<FGameplayTag> DeBuffDamageType;
	UPROPERTY()
	FVector DeathImpulse = FVector::ZeroVector;
};

USTRUCT(BlueprintType)
struct FDamageEffectParams
{
	GENERATED_BODY()
	FDamageEffectParams(){}

	UPROPERTY()
	TObjectPtr<UObject> WorldContextObject=nullptr;

	UPROPERTY()
	TSubclassOf<UGameplayEffect> DamageGameplayEffectClass=nullptr;

	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> SourceASC;

	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> TargetASC;

	UPROPERTY()
	TMap<FGameplayTag,float> DamageTypes;//技能造成的多种伤害类型

	UPROPERTY()
	float AbilityLevel;
	
	UPROPERTY()
	FGameplayTag DeBuffDamageType=FGameplayTag();

	UPROPERTY()
	float DeBuffChance;

	UPROPERTY()
	float DeBuffDamage;

	UPROPERTY()
	float DeBuffDuration;

	UPROPERTY()
	float DeBuffFrequency;

	UPROPERTY()
	float DeathImpulseMagnitude;//死亡受到的冲击力

	UPROPERTY()
	FVector DeathImpulse=FVector::ZeroVector;//死亡时受到的冲击力朝向
};


//特化 为虚幻的底层类型系统提供元数据。
template<>
struct TStructOpsTypeTraits<FAuraGameplayContext>:public TStructOpsTypeTraitsBase2<FAuraGameplayContext>
{
	enum
	{
		WithNetSerializer=true,
		WithCopy=true
	};
};
