// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgeController/SpellMenuWidgetController.h"

#include "AuraGameplayTags.h"

void USpellMenuWidgetController::BroadcastInitialValues()
{
	CurrentSpellPoints=GetAuraPlayerState()->GetSpellPoints();
	SpellPointChanged.Broadcast(GetAuraPlayerState()->GetSpellPoints());
	BroadcastAbilityInfo();
}

void USpellMenuWidgetController::BindCallbacksToDependencies()
{
	//广播技能数据更新，用于更新技能按钮的显示状态
	GetAuraAbilitySystemComponent()->AbilityStatusChangedDelegate.AddLambda(
		[this](const FGameplayTag& AbilityTag,const FGameplayTag& StatusTag,int32 NewLevel)
	{
		if (SelectedAbility.Ability.MatchesTagExact(AbilityTag))
		{
			SelectedAbility.Status=StatusTag;
			SelectedAbility.Level=NewLevel;
			BroadcastSpellGlobeSelected();
		}
		if (AbilityInfo)
		{
			FAuraAbilityInfo Info=AbilityInfo->FindAbilityInfoForTag(AbilityTag);
			Info.StatusTag=StatusTag;
			AbilityInfoDelegate.Broadcast(Info);
		}
	});
	//绑定技能点变动回调
	GetAuraPlayerState()->OnSpellPointsChangedDelegate.AddLambda([this](const int32 SpellPoints)
	{
		SpellPointChanged.Broadcast(GetAuraPlayerState()->SpellPoints);
		CurrentSpellPoints=GetAuraPlayerState()->SpellPoints;
		BroadcastSpellGlobeSelected();
	});

	GetAuraAbilitySystemComponent()->AbilityEquippedDelegate.AddUObject(this,&USpellMenuWidgetController::OnAbilityEquipped);
}

FGameplayTag USpellMenuWidgetController::SpellGlobeSelected(const FGameplayTag& AbilityTag)
{
	bWaitingForEquipSelection=true;
	Count=0;
	FGameplayTag AbilityStatus;
	const bool bTagValid=AbilityTag.IsValid();
	const bool bTagNone=AbilityTag.MatchesTagExact(FAuraGameplayTags::Get().Abilities_Type_None);
	const FGameplayAbilitySpec* AbilitySpec=GetAuraAbilitySystemComponent()->GetSpecFromAbilityTag(AbilityTag);
	const bool bSpecValid=AbilitySpec!=nullptr;

	if (!bTagValid||bTagNone||!bSpecValid)
	{
		AbilityStatus=FAuraGameplayTags::Get().Abilities_Status_Locked;
	}
	else
	{
		AbilityStatus=GetAuraAbilitySystemComponent()->GetStatusTagFromSpec(*AbilitySpec);
		SelectedAbility.Level=AbilitySpec->Level;
	}

	//更新结构体
	SelectedAbility.Ability=AbilityTag;
	SelectedAbility.Status=AbilityStatus;
	BroadcastSpellGlobeSelected();
	return AbilityStatus;
}

void USpellMenuWidgetController::ShouldEnableButtons(const FGameplayTag& AbilityStatus, bool HasSpellPoints,
	bool& bShouldEnableSpellPoints, bool& bShouldEnableEquip, bool& bShouldDemotionPoints)
{
	const FAuraGameplayTags AuraGameplayTags=FAuraGameplayTags::Get();
	if (AbilityStatus.MatchesTagExact(AuraGameplayTags.Abilities_Status_Equipped))
	{
		bShouldEnableSpellPoints=HasSpellPoints;
		bShouldDemotionPoints=true;
		bShouldEnableEquip=true;
	}
	else if (AbilityStatus.MatchesTagExact(AuraGameplayTags.Abilities_Status_Eligible))
	{
		bShouldEnableSpellPoints=HasSpellPoints;
		bShouldDemotionPoints=false;
		bShouldEnableEquip=false;
	}
	else if (AbilityStatus.MatchesTagExact(AuraGameplayTags.Abilities_Status_Unlocked))
	{
		bShouldEnableSpellPoints=HasSpellPoints;
		bShouldDemotionPoints=true;
		bShouldEnableEquip=true;
	}
	else if (AbilityStatus.MatchesTagExact(AuraGameplayTags.Abilities_Status_Locked))
	{
		bShouldEnableSpellPoints=false;
		bShouldDemotionPoints=false;
		bShouldEnableEquip=false;
	}
	

}

void USpellMenuWidgetController::BroadcastSpellGlobeSelected()
{
	bool bShouldEnableSpellPoints=false;
	bool bShouldEnableEquip=false;
	bool bShouldDemotionPoints=false;
	ShouldEnableButtons(SelectedAbility.Status,CurrentSpellPoints>0,bShouldEnableSpellPoints,bShouldEnableEquip,bShouldDemotionPoints);
	OnSpellGlobeSelectedDelegate.Broadcast(bShouldEnableSpellPoints,bShouldEnableEquip,bShouldDemotionPoints,SelectedAbility.Level);
	FString SpellDescription=FString();
	FString SpellNextDescription=FString();
	GetAuraAbilitySystemComponent()->GetDescriptionByAbilityTag(SelectedAbility.Ability,SpellDescription,SpellNextDescription);
	SpellDescriptionSignature.Broadcast(SpellDescription,SpellNextDescription);
}

void USpellMenuWidgetController::SpendPointButtonPressed(const FGameplayTag& AbilityTag)
{
	if (GetAuraAbilitySystemComponent())
	{
		GetAuraAbilitySystemComponent()->ServerSpendSpellPoint(AbilityTag);
	}
}

void USpellMenuWidgetController::DemotionPointButtonPressed(const FGameplayTag& AbilityTag)
{
	if (GetAuraAbilitySystemComponent())
	{
		GetAuraAbilitySystemComponent()->ServerDemotionSpellPoint(AbilityTag);
	}
}

void USpellMenuWidgetController::EquipButtonPressed(const FGameplayTag& SlotTag, const FGameplayTag& AbilityType)
{
	Count++;
	if (Count==2)
	{
		Count=0;
		bWaitingForEquipSelection=false;
	}
	const FGameplayTag SelectedAbilityType=AbilityInfo->FindAbilityInfoForTag(SelectedAbility.Ability).AbilityType;
	if (!SelectedAbilityType.MatchesTagExact(AbilityType))
	{
		return;
	}
	GetAuraAbilitySystemComponent()->ServerEquipAbility(SelectedAbility.Ability,SlotTag);
}

/*void USpellMenuWidgetController::OnAbilityEquipped(const FGameplayTag& AbilityTag, const FGameplayTag& Status,
	const FGameplayTag& Slot, const FGameplayTag& PreSlot)
{
	const FAuraGameplayTags GameplayTags=FAuraGameplayTags::Get();

	if (bWaitingForEquipSelection&&Count==2)
	{
		FAuraAbilityInfo LastSlotInfo;
		LastSlotInfo.StatusTag=GameplayTags.Abilities_Status_Unlocked;
		LastSlotInfo.InputTag=PreSlot;
		LastSlotInfo.AbilityTag=GameplayTags.Abilities_None;
		AbilityInfoDelegate.Broadcast(LastSlotInfo);
		Count=0;
	}
	FAuraAbilityInfo Info=AbilityInfo->FindAbilityInfoForTag(AbilityTag);
	Info.StatusTag=Status;
	Info.InputTag=Slot;
	AbilityInfoDelegate.Broadcast(Info);
}*/
void USpellMenuWidgetController::OnAbilityEquipped(const FGameplayTag& AbilityTag, const FGameplayTag& Status,
	const FGameplayTag& Slot, const FGameplayTag& PreSlot)
{
	const FAuraGameplayTags GameplayTags = FAuraGameplayTags::Get();
    
	// 处理上一个插槽（如果技能是从其他插槽移动过来的）
	if (PreSlot.IsValid() && !PreSlot.MatchesTagExact(GameplayTags.Abilities_Type_None))
	{
		// 查找是否还有其他技能在这个插槽上
		// 这里可能需要从ASC查询
		FAuraAbilityInfo LastSlotInfo;
		LastSlotInfo.StatusTag = GameplayTags.Abilities_Status_Unlocked;
		LastSlotInfo.InputTag = PreSlot;
		LastSlotInfo.AbilityTag = GameplayTags.Abilities_None;
		AbilityInfoDelegate.Broadcast(LastSlotInfo);
	}
    
	// 更新当前装备的技能
	FAuraAbilityInfo Info = AbilityInfo->FindAbilityInfoForTag(AbilityTag);
	Info.StatusTag = Status;
	Info.InputTag = Slot;
	AbilityInfoDelegate.Broadcast(Info);
    
	// 关键修复：广播所有技能信息，确保UI正确更新
	BroadcastAbilityInfo();
}
