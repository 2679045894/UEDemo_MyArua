// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/AuraAbilitySystemComponent.h"


#include "AbilitySystemBlueprintLibrary.h"
#include "AuraGameplayTags.h"
#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "AbilitySystem/Abilitys/AuraGameplayAbility.h"
#include "AbilitySystem/Data/AbilityInfo.h"
#include "Aura/AuraLogChannels.h"
#include "Interaction/PlayerInterface.h"


void UAuraAbilitySystemComponent::AbilityActorInfoSet()
{
	//将委托和函数绑定()
	OnGameplayEffectAppliedDelegateToSelf.AddUObject(this,&UAuraAbilitySystemComponent::ClientEffectApplied);
}

//回调函数
void UAuraAbilitySystemComponent::ClientEffectApplied_Implementation(UAbilitySystemComponent* ASC, const FGameplayEffectSpec& EffectSpec,
	FActiveGameplayEffectHandle ActiveEffectHandle)
{
	//标签容器，用于存储某个效果/对象的所有附加的标签
	FGameplayTagContainer TagContainer;
	EffectSpec.GetAllAssetTags(TagContainer);
	EffectAssetTags.Broadcast(TagContainer);
}

//赋予所有在StartupAbilities中的技能
void UAuraAbilitySystemComponent::AddCharacterAbilities(const TArray<TSubclassOf<UGameplayAbility>>& StartupAbilities)
{
	for (TSubclassOf<UGameplayAbility> AbilityClass : StartupAbilities)
	{
		//1.创建技能规格
		FGameplayAbilitySpec AbilitySpec=FGameplayAbilitySpec(AbilityClass,1);
		//将技能规格中的Ability进行转换
		if (UAuraGameplayAbility* AuraAbility=Cast<UAuraGameplayAbility>(AbilitySpec.Ability))
		{
			//2.绑定输入标签(向技能的动态标签集合中添加一个标签)（通过某个按键标签(StartupInputTag)触发技能）
			AbilitySpec.DynamicAbilityTags.AddTag(AuraAbility->StartupInputTag);
			AbilitySpec.DynamicAbilityTags.AddTag(FAuraGameplayTags::Get().Abilities_Status_Equipped);
			//3.赋予技能
			GiveAbility(AbilitySpec);
		}
		bStartupAbilitiesGiven=true;
		AbilitiesGivenDelegate.Broadcast();
	}
}

void UAuraAbilitySystemComponent::AddCharacterPassiveAbilities(
	const TArray<TSubclassOf<UGameplayAbility>>& StartupPassiveAbilities)
{
	for (const TSubclassOf<UGameplayAbility> AbilityClass : StartupPassiveAbilities)
	{
		FGameplayAbilitySpec AbilitySpec=FGameplayAbilitySpec(AbilityClass,1);
		GiveAbilityAndActivateOnce(AbilitySpec);
	}
}

void UAuraAbilitySystemComponent::AbilityInputTagHeld(const FGameplayTag &InputTag)
{
	if (!InputTag.IsValid())return;
	//循环遍历所有可激活的技能
	for (FGameplayAbilitySpec& AbilitySpec:GetActivatableAbilities())
	{
		//检查输入标签匹配
		if (AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag))
		{
			//标记输入按下状态 - 通知技能系统该技能对应的输入已被按下
			AbilitySpecInputPressed(AbilitySpec);
			//检查技能是否已激活 - 判断当前技能是否处于未激活状态
			if (!AbilitySpec.IsActive())
			{
				//如果技能未激活，则尝试激活该技能。
				TryActivateAbility(AbilitySpec.Handle);
			}
		}
	}
}

void UAuraAbilitySystemComponent::AbilityInputTagReleased(const FGameplayTag &InputTag)
{
	if (!InputTag.IsValid())return;

	for (FGameplayAbilitySpec& AbilitySpec:GetActivatableAbilities())
	{
		if (AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag))
		{
			//通知技能系统该技能对应的输入已被释放
			AbilitySpecInputReleased(AbilitySpec);
		}
	}
}

void UAuraAbilitySystemComponent::ForEachAbility(const FForEachAbility& Delegate)
{
	FScopedAbilityListLock ActiveScopeLock(*this);
	for (const FGameplayAbilitySpec&AbilitySpec:GetActivatableAbilities())
	{
		//在判断的同时传参(AbilitySpec)
		if (!Delegate.ExecuteIfBound(AbilitySpec))
		{
			UE_LOG(LogAura,Error,TEXT("在函数[%hs]运行委托失败"),__FUNCTION__);
		}
	}
}

FGameplayTag UAuraAbilitySystemComponent::GetAbilityTagFromSpec(const FGameplayAbilitySpec& AbilitySpec)
{
	if (AbilitySpec.Ability)
	{
		for (FGameplayTag Tag:AbilitySpec.Ability.Get()->AbilityTags)
		{
			if (Tag.MatchesTag(FGameplayTag::RequestGameplayTag(FName("Abilities"))))
			{
				return Tag;
			}
		}
	}
	return FGameplayTag();
}

FGameplayTag UAuraAbilitySystemComponent::GetInputTagFromSpec(const FGameplayAbilitySpec& AbilitySpec)
{
	for (FGameplayTag Tag:AbilitySpec.DynamicAbilityTags)
	{
		if (Tag.MatchesTag(FGameplayTag::RequestGameplayTag(FName("InputTag"))))
		{
			return Tag;
		}
	}
	return FGameplayTag();
}

FGameplayTag UAuraAbilitySystemComponent::GetStatusTagFromSpec(const FGameplayAbilitySpec& AbilitySpec)
{
	for (FGameplayTag Tag:AbilitySpec.DynamicAbilityTags)
	{
		if (Tag.MatchesTag(FGameplayTag::RequestGameplayTag(FName("Abilities.Status"))))
		{
			return Tag;
		}
	}
	return FGameplayTag();
}

FGameplayAbilitySpec* UAuraAbilitySystemComponent::GetSpecFromAbilityTag(const FGameplayTag& AbilityTag)
{
	//域锁
	FScopedAbilityListLock ActiveScopeLoc(*this);
	//遍历已经应用的技能
	for (FGameplayAbilitySpec& AbilitySpec:GetActivatableAbilities())
	{
		for (FGameplayTag Tag:AbilitySpec.Ability.Get()->AbilityTags)
		{
			if (Tag.MatchesTag(AbilityTag))
			{
				return &AbilitySpec;
			}
		}
	}
	return nullptr;
}

void UAuraAbilitySystemComponent::OnRep_ActivateAbilities()
{
	Super::OnRep_ActivateAbilities();
	if (!bStartupAbilitiesGiven)
	{
		bStartupAbilitiesGiven = true;
		//通知所有监听者"技能初始化应用已完成"。
		AbilitiesGivenDelegate.Broadcast();
	}
}

void UAuraAbilitySystemComponent::UpgradeAttribute(const FGameplayTag& AttributeTag)
{
	if (GetAvatarActor()->Implements<UPlayerInterface>())
	{
		if (IPlayerInterface::Execute_GetAttributePoints(GetAvatarActor())>0)
		{
			ServerUpgradeAttribute(AttributeTag);
		}
	}
}

void UAuraAbilitySystemComponent::UpdateAbilityStatuses(int32 Level)
{
	UAbilityInfo* AbilityInfo=UAuraAbilitySystemLibrary::GetActiveAbilityInfo(GetAvatarActor());
	for (FAuraAbilityInfo& Info:AbilityInfo->AbilityInformation)
	{
		if (!Info.AbilityTag.IsValid())continue;
		if (Level<Info.LevelRequirement)continue;
		if (GetSpecFromAbilityTag(Info.AbilityTag)==nullptr)
		{
			FGameplayAbilitySpec AbilitySpec=FGameplayAbilitySpec(Info.Ability,1);
			AbilitySpec.DynamicAbilityTags.AddTag(FAuraGameplayTags::Get().Abilities_Status_Eligible);
			GiveAbility(AbilitySpec);
			//设置当前技能立即复制到每个客户端
			MarkAbilitySpecDirty(AbilitySpec);
			ClientUpdateAbilityStatus(Info.AbilityTag,FAuraGameplayTags::Get().Abilities_Status_Eligible);
		}
	}
}

bool UAuraAbilitySystemComponent::GetDescriptionByAbilityTag(const FGameplayTag& AbilityTag, FString& OutDescription,
	FString& OutNextLevelDescription)
{
	if (FGameplayAbilitySpec* AbilitySpec=GetSpecFromAbilityTag(AbilityTag))
	{
		if (UAuraGameplayAbility* Ability=Cast<UAuraGameplayAbility>(AbilitySpec->Ability))
		{
			OutDescription=Ability->GetDescription(AbilitySpec->Level);
			OutNextLevelDescription=Ability->GetNextLevelDescription(AbilitySpec->Level+1);
			return true;
		}
	}
	UAbilityInfo* AbilityInfo=UAuraAbilitySystemLibrary::GetActiveAbilityInfo(GetAvatarActor());
	OutDescription=UAuraGameplayAbility::GetLockedDescription(AbilityInfo->FindAbilityInfoForTag(AbilityTag).LevelRequirement);
	OutNextLevelDescription=FString();
	return false;
	
}

//判断当前技能实例是否处于目标技能装备插槽
bool UAuraAbilitySystemComponent::AbilityHasSlot(FGameplayAbilitySpec* Spec, const FGameplayTag& Slot)
{
	for (FGameplayTag Tag:Spec->DynamicAbilityTags)
	{
		if (Tag.MatchesTagExact(Slot))
		{
			return true;
		}
	}
	return false;
}

void UAuraAbilitySystemComponent::ClearSlot(FGameplayAbilitySpec* Spec)
{
	const FGameplayTag Slot=GetInputTagFromSpec(*Spec);
	Spec->DynamicAbilityTags.RemoveTag(Slot);
	//标记为需要同步
	MarkAbilitySpecDirty(*Spec);
}

void UAuraAbilitySystemComponent::ClearAbilitiesOfSlot(const FGameplayTag& Slot)
{
	FScopedAbilityListLock ActiveScopeLock(*this);
	for (FGameplayAbilitySpec& Spec:GetActivatableAbilities())
	{
		if (AbilityHasSlot(&Spec,Slot))
		{
			ClearSlot(&Spec);
		}
	}
}

void UAuraAbilitySystemComponent::ClientEquipAbility_Implementation(const FGameplayTag& AbilityTag,
	const FGameplayTag& Status, const FGameplayTag& Slot, const FGameplayTag& PreviousSlot)
{
	AbilityEquippedDelegate.Broadcast(AbilityTag,Status,Slot,PreviousSlot);
}

void UAuraAbilitySystemComponent::ServerEquipAbility_Implementation(const FGameplayTag& AbilityTag,
                                                                    const FGameplayTag& SlotTag)
{
	if (FGameplayAbilitySpec* AbilitySpec=GetSpecFromAbilityTag(AbilityTag))
	{
		//技能之前装配的插槽
		const FGameplayTag& PreSlot=GetInputTagFromSpec(*AbilitySpec);
		//当前技能的状态标签
		const FGameplayTag& Status=GetStatusTagFromSpec(*AbilitySpec);

		const FAuraGameplayTags AuraGameplayTags=FAuraGameplayTags::Get();
		if (Status.MatchesTagExact(AuraGameplayTags.Abilities_Status_Unlocked)||Status.MatchesTagExact(AuraGameplayTags.Abilities_Status_Equipped))
		{
			ClearAbilitiesOfSlot(SlotTag);
			ClearSlot(AbilitySpec);
			AbilitySpec->DynamicAbilityTags.AddTag(SlotTag);
		}
		if (Status.MatchesTagExact(AuraGameplayTags.Abilities_Status_Unlocked))
		{
			AbilitySpec->DynamicAbilityTags.RemoveTag(AuraGameplayTags.Abilities_Status_Unlocked);
			AbilitySpec->DynamicAbilityTags.AddTag(AuraGameplayTags.Abilities_Status_Equipped);
		}
		ClientEquipAbility(AbilityTag,Status,SlotTag,PreSlot);
		MarkAbilitySpecDirty(*AbilitySpec);
	}
}

void UAuraAbilitySystemComponent::ServerSpendSpellPoint_Implementation(const FGameplayTag& AbilityTag)
{
	if (FGameplayAbilitySpec* AbilitySpec=GetSpecFromAbilityTag(AbilityTag))
	{
		if (IPlayerInterface* PlayerInterface=Cast<IPlayerInterface>(GetAvatarActor()))
		{
			PlayerInterface->Execute_AddToSpellPoints(GetAvatarActor(),-1);
		}
		FAuraGameplayTags AuraGameplayTags=FAuraGameplayTags::Get();
		FGameplayTag StatusTag=GetStatusTagFromSpec(*AbilitySpec);

		if (StatusTag.MatchesTagExact(AuraGameplayTags.Abilities_Status_Eligible))
		{
			AbilitySpec->DynamicAbilityTags.AddTag(AuraGameplayTags.Abilities_Status_Unlocked);
			AbilitySpec->DynamicAbilityTags.RemoveTag(AuraGameplayTags.Abilities_Status_Eligible);
			AbilitySpec->Level+=1;
		}
		else if (StatusTag.MatchesTagExact(AuraGameplayTags.Abilities_Status_Equipped)||StatusTag.MatchesTagExact(AuraGameplayTags.Abilities_Status_Unlocked))
		{
			AbilitySpec->Level+=1;
		}

		ClientUpdateAbilityStatus(AbilityTag,StatusTag);
		//将当前技能复制到每个客户端
		MarkAbilitySpecDirty(*AbilitySpec);
	}
}

void UAuraAbilitySystemComponent::ServerDemotionSpellPoint_Implementation(const FGameplayTag& AbilityTag)
{
	if (FGameplayAbilitySpec* AbilitySpec=GetSpecFromAbilityTag(AbilityTag))
	{
		if (AbilitySpec->Level==1)
			return;
		if (IPlayerInterface* PlayerInterface=Cast<IPlayerInterface>(GetAvatarActor()))
		{
			PlayerInterface->Execute_AddToSpellPoints(GetAvatarActor(),1);
		}
		FAuraGameplayTags AuraGameplayTags=FAuraGameplayTags::Get();
		FGameplayTag StatusTag=GetStatusTagFromSpec(*AbilitySpec);
		if (StatusTag.MatchesTagExact(AuraGameplayTags.Abilities_Status_Unlocked)||StatusTag.MatchesTagExact(AuraGameplayTags.Abilities_Status_Equipped))
		{
			AbilitySpec->Level-=1;
			if (AbilitySpec->Level<1)
			{
				AbilitySpec->DynamicAbilityTags.RemoveTag(AuraGameplayTags.Abilities_Status_Unlocked);
				AbilitySpec->DynamicAbilityTags.RemoveTag(AuraGameplayTags.Abilities_Status_Equipped);
				AbilitySpec->DynamicAbilityTags.AddTag(AuraGameplayTags.Abilities_Status_Eligible);
				StatusTag=AuraGameplayTags.Abilities_Status_Eligible;
			}
		}
		ClientUpdateAbilityStatus(AbilityTag,StatusTag);
		MarkAbilitySpecDirty(*AbilitySpec);
	}
}


/*//ASC是在服务器运行，我们再增加一个客户端执行的函数，用于广播到每个客户端
void UAuraAbilitySystemComponent::ClientUpdateAbilityStatus_Implementation(const FGameplayTag& AbilityTag,
	const FGameplayTag& StatusTag)
{
	FGameplayAbilitySpec* AbilitySpec= GetSpecFromAbilityTag(AbilityTag);
	AbilityStatusChangedDelegate.Broadcast(AbilityTag,StatusTag,AbilitySpec->Level);
}*/
void UAuraAbilitySystemComponent::ClientUpdateAbilityStatus_Implementation(const FGameplayTag& AbilityTag,
	const FGameplayTag& StatusTag)
{
	// 尝试多次获取，防止时序问题
	FGameplayAbilitySpec* AbilitySpec = GetSpecFromAbilityTag(AbilityTag);
    
	if (!AbilitySpec)
	{
		// 如果立即获取失败，尝试延迟获取
		FTimerHandle TimerHandle;
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, [this, AbilityTag, StatusTag]()
		{
			FGameplayAbilitySpec* RetrySpec = GetSpecFromAbilityTag(AbilityTag);
			if (RetrySpec)
			{
				AbilityStatusChangedDelegate.Broadcast(AbilityTag, StatusTag, RetrySpec->Level);
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("Failed to find AbilitySpec for tag: %s"), 
					*AbilityTag.ToString());
				// 使用默认等级广播
				AbilityStatusChangedDelegate.Broadcast(AbilityTag, StatusTag, 1);
			}
		}, 0.1f, false); // 延迟0.1秒重试
		return;
	}
    
	AbilityStatusChangedDelegate.Broadcast(AbilityTag, StatusTag, AbilitySpec->Level);
}

void UAuraAbilitySystemComponent::ServerUpgradeAttribute_Implementation(const FGameplayTag& AttributeTag)
{
	FGameplayEventData Payload;
	Payload.EventTag=AttributeTag;
	Payload.EventMagnitude=1.f;
	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(GetAvatarActor(),AttributeTag,Payload);

	if (GetAvatarActor()->Implements<UPlayerInterface>())
	{
		IPlayerInterface::Execute_AddToAttributePoints(GetAvatarActor(),-1);
	}
}
