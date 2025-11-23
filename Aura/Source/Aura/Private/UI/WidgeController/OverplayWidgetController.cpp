// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgeController/OverplayWidgetController.h"

#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "Player/MyPlayerState.h"


void UOverplayWidgetController::BroadcastInitialValues()
{
	if (UAuraAttributeSet* AruaAttributeSet=Cast<UAuraAttributeSet>(AttributeSet))
	{
		//传入的值进行广播，在蓝图中调用该委托的时候会提供一个参数(GetHealth)
		OnHealthChanged.Broadcast(AruaAttributeSet->GetHealth());
		OnMaxHealthChanged.Broadcast(AruaAttributeSet->GetMaxHealth());
		OnManaChanged.Broadcast(AruaAttributeSet->GetMana());
		OnMaxManaChanged.Broadcast(AruaAttributeSet->GetMaxMana());
	}
}
// 绑定属性变化委托，建立属性变化时的回调机制。自动调用
//告诉 GAS："当这些属性发生变化时，请自动调用我指定的函数"。
//初始化时调用一次(建立绑定关系)
void UOverplayWidgetController::BindCallbacksToDependencies()
{
	if (UAuraAttributeSet* AruaAttributeSet=Cast<UAuraAttributeSet>(AttributeSet))
	{
		//AruaAttributeSet->GetHealthAttribute()为什么不直接gethealth
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
			AruaAttributeSet->GetHealthAttribute()).AddLambda([this](const FOnAttributeChangeData &Data)
			{
				OnHealthChanged.Broadcast(Data.NewValue);
			});
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
			AruaAttributeSet->GetMaxHealthAttribute()).AddLambda([this](const FOnAttributeChangeData &Data)
			{
				OnMaxHealthChanged.Broadcast(Data.NewValue);
			});
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
			AruaAttributeSet->GetManaAttribute()).AddLambda([this](const FOnAttributeChangeData &Data)
			{
				OnManaChanged.Broadcast(Data.NewValue);
			});
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
			AruaAttributeSet->GetMaxManaAttribute()).AddLambda([this](const FOnAttributeChangeData &Data)
			{
				OnMaxManaChanged.Broadcast(Data.NewValue);
			});
	}
	if (UAuraAbilitySystemComponent* ASC=Cast<UAuraAbilitySystemComponent>(AbilitySystemComponent))
	{
		if (ASC->bStartupAbilitiesGiven)
		{
			//如果执行到此处时，技能的初始化工作已经完成，则直接调用初始化回调
			OnInitializeStartupAbilities(ASC);
		}
		else
		{
			//如果执行到此处，技能初始化还未完成，将通过绑定委托，监听广播的形式触发初始化完成回调
			ASC->AbilityGivenDelegate.AddUObject(this,&UOverplayWidgetController::OnInitializeStartupAbilities);
		}
	}
	//获取委托，通过lambda表达式添加绑定函数
	Cast<UAuraAbilitySystemComponent>(AbilitySystemComponent)->EffectAssetTags.AddLambda(
		[this/*!!!!*/](FGameplayTagContainer& TagContainer)
		{
			for (auto Tag :TagContainer)
			{
				//这相当于一个过滤器，过滤出Message的子标签，因为hi有这些标签才有对应的行命名信息，如果不是Message下的标签，则会报空
				FGameplayTag MessageTag=FGameplayTag::RequestGameplayTag(FName("Message"));
				if (Tag.MatchesTag(MessageTag))
				{	//模板函数
					FUIWidgetRow* Row=GetDataTableRowByTag<FUIWidgetRow>(MessageWidgetDataTable,Tag);
					MessageWidgetRowDelegate.Broadcast(*Row);
				}
			}
		}
	);
	AMyPlayerState* AuraPlayerState=Cast<AMyPlayerState>(PlayerState);
	AuraPlayerState->OnXPChangedDelegate.AddUObject(this,&UOverplayWidgetController::OnXPChanged);
}

void UOverplayWidgetController::OnInitializeStartupAbilities(
	UAuraAbilitySystemComponent* AuraAbilitySystemComponent) const
{
	if (!AuraAbilitySystemComponent->bStartupAbilitiesGiven)return;

	FForEachAbility BroadcastDelegate;
	BroadcastDelegate.BindLambda([this,AuraAbilitySystemComponent](const FGameplayAbilitySpec&AbilitySpec)
	{
		FAuraAbilityInfo Info=AbilityInfo->FindAbilityInfoForTag(UAuraAbilitySystemComponent::GetAbilityTagFormSpec(AbilitySpec));
		Info.InputTag=UAuraAbilitySystemComponent::GetInputTagFormSpec(AbilitySpec);
		AbilityInfoDelegate.Broadcast(Info);
	});
	AuraAbilitySystemComponent->ForEachAbility(BroadcastDelegate);
}

void UOverplayWidgetController::OnXPChanged(int32 NewXP)const
{
	const AMyPlayerState* AuraPlayerState=Cast<AMyPlayerState>(PlayerState);
	ULevelUpInfo* LevelUpInfo=AuraPlayerState->LevelUpInfo;
	check(LevelUpInfo);
	int32 Level=LevelUpInfo->FindLevelForXP(NewXP);
	int32 MaxLevel=LevelUpInfo->LevelUpInformation.Num();

	if (Level<=MaxLevel&&Level>0)
	{
		const int32 LevelUpRequirement=LevelUpInfo->LevelUpInformation[Level].LevelUpRequirement;
		const int32 PreviousLevelUpRequirement=LevelUpInfo->LevelUpInformation[Level-1].LevelUpRequirement;
		const float XPPercent=static_cast<float>(NewXP-PreviousLevelUpRequirement)/(LevelUpRequirement-PreviousLevelUpRequirement);
		OnXPPercentChangedDelegate.Broadcast(XPPercent);
	}
}






