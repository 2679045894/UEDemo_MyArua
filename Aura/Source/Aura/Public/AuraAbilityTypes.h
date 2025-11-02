#pragma once
#pragma once

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
	//设置器方法 允许其他系统设置这些标志
	void SetIsCriticalHit(bool bInIsCriticalHit){bIsCriticalHit=bInIsCriticalHit;}
	void SetIsBlockedHit(bool bInIsBlockedHit){bIsBlockedHit=bInIsBlockedHit;}

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
