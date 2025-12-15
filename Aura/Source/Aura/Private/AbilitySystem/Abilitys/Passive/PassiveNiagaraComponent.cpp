// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilitys/Passive/PassiveNiagaraComponent.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "Interaction/CombatInterface.h"

UPassiveNiagaraComponent::UPassiveNiagaraComponent()
{
}

void UPassiveNiagaraComponent::BeginPlay()
{
	if (UAuraAbilitySystemComponent* AuraASC=Cast<UAuraAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetOwner())))
	{
		AuraASC->ActivePassiveEffectDelegate.AddUObject(this,&UPassiveNiagaraComponent::OnPassiveActivate);
	}
	else if (ICombatInterface* CombatInterface=Cast<ICombatInterface>(GetOwner()))
	{
		CombatInterface->GetOnASCRegistered().AddWeakLambda(this,[this](UAbilitySystemComponent* InASC)
		{
			if (UAuraAbilitySystemComponent* AuraASC=Cast<UAuraAbilitySystemComponent>(InASC))
			{
				AuraASC->ActivePassiveEffectDelegate.AddUObject(this,&UPassiveNiagaraComponent::OnPassiveActivate);
			}
		});
	}
}

void UPassiveNiagaraComponent::OnPassiveActivate(const FGameplayTag& AbilityTag, const bool bActivate)
{
	if (AbilityTag.MatchesTag(PassiveSpellTag))
	{
		if (bActivate)
		{
			if (!IsActive())Activate();
		}
		else
		{
			Deactivate();
		}
	}
}
