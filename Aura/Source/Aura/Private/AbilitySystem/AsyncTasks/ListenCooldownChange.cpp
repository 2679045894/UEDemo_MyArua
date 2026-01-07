// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/AsyncTasks/ListenCooldownChange.h"

UListenCooldownChange* UListenCooldownChange::ListenForCooldownChange(
	UAuraAbilitySystemComponent* AbilitySystemComponent, const FGameplayTag InCooldownTag)
{
	UListenCooldownChange* ListenCooldownChange=NewObject<UListenCooldownChange>();
	ListenCooldownChange->ASC=AbilitySystemComponent;
	ListenCooldownChange->CooldownTag=InCooldownTag;

	if (!IsValid(AbilitySystemComponent)||!IsValid(ListenCooldownChange))
	{
		ListenCooldownChange->EndTask();
		return nullptr;
	}
	//监听冷却标签变动，并绑定回调，用于获取冷却结束
	AbilitySystemComponent->RegisterGameplayTagEvent(InCooldownTag,
		EGameplayTagEventType::NewOrRemoved).AddUObject(
		ListenCooldownChange,
		&UListenCooldownChange::CooldownTagChanged);
	//监听GE应用回调，获取冷却激活，用于获取技能开始进入冷却
	AbilitySystemComponent->OnActiveGameplayEffectAddedDelegateToSelf.AddUObject(
		ListenCooldownChange,&UListenCooldownChange::OnActiveEffectAdded);
	return ListenCooldownChange;
}

void UListenCooldownChange::EndTask()
{
	if (!IsValid(ASC))return;
	//取消对冷却标签的监听
	ASC->RegisterGameplayTagEvent(CooldownTag,EGameplayTagEventType::NewOrRemoved).RemoveAll(this);

	//设置此对象可以被销毁，如果此对象不再被引用，即可被销毁
	SetReadyToDestroy();
	//标记此实例为垃圾资源，可以被回收
	MarkAsGarbage();
}

void UListenCooldownChange::CooldownTagChanged(const FGameplayTag InCooldownTag, int32 NewCount)
{
	if (!IsValid(ASC)) return;
    
	// 只在服务器或自主代理上处理冷却结束
	if (!ASC->GetOwner()->HasAuthority() && !ASC->IsOwnerActorAuthoritative())
	{
		// 对于客户端，需要更精确的判断
		return;
	}
    
	if (NewCount == 0)
	{
		// 添加额外验证，确保冷却确实结束了
		FGameplayEffectQuery Query = FGameplayEffectQuery::MakeQuery_MatchAnyOwningTags(
			CooldownTag.GetSingleTagContainer());
		TArray<float> TimesRemaining = ASC->GetActiveEffectsTimeRemaining(Query);
        
		// 只有当确实没有剩余时间时才广播冷却结束
		if (TimesRemaining.Num() == 0 || TimesRemaining[0] <= 0.1f)
		{
			CooldownEnd.Broadcast(0.f);
			bIsFirstAttempt = false;
		}
	}
}

void UListenCooldownChange::OnActiveEffectAdded(UAbilitySystemComponent* TargetASC,
	const FGameplayEffectSpec& SpecApplied, FActiveGameplayEffectHandle ActiveEffectHandle)
{
	//获取设置到自身的所有标签
	FGameplayTagContainer AssetTags;
	SpecApplied.GetAllAssetTags(AssetTags);
	//获取到GE设置给Actor的标签
	FGameplayTagContainer GrantedTags;
	SpecApplied.GetAllGrantedTags(GrantedTags);

	//判断应用的GE是否设置此冷却标签
	if (AssetTags.HasTagExact(CooldownTag)||GrantedTags.HasTagExact(CooldownTag))
	{
		//创建一个查询对象，用于查询目标 Actor 身上被 GE 授予的标签
		FGameplayEffectQuery GameplayEffectQuery=FGameplayEffectQuery::MakeQuery_MatchAnyOwningTags(
			CooldownTag.GetSingleTagContainer());
		//返回查询到的所有包含此冷却GE的剩余时间的GE
		TArray<float> TimesRemaining=ASC->GetActiveEffectsTimeRemaining(GameplayEffectQuery);
		if (TimesRemaining.Num()>0)
		{
			//获取最高的冷却时间
			float TimeRemaining=TimesRemaining[0];
			for (int32 i=0;i<TimesRemaining.Num();i++)
			{
				if (TimeRemaining<TimesRemaining[i])
				{
					TimeRemaining=TimesRemaining[i];
				}
			}
			bIsFirstAttempt = true;
			//广播初始时间
			CooldownStart.Broadcast(TimeRemaining);
		}
	}
}


