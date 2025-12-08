// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/DeBuff/DeBuffNiagaraComponent.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Interaction/CombatInterface.h"

UDeBuffNiagaraComponent::UDeBuffNiagaraComponent()
{
	bAutoActivate=false;
}

void UDeBuffNiagaraComponent::BeginPlay()
{
	Super::BeginPlay();
	ICombatInterface* CombatInterface=Cast<ICombatInterface>(GetOwner());
	if (UAbilitySystemComponent* ASC=UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetOwner()))
	{
		ASC->RegisterGameplayTagEvent(DeBuffTag,EGameplayTagEventType::NewOrRemoved).AddUObject(this,&UDeBuffNiagaraComponent::DeBuffTagChanged);
	}
	else if (CombatInterface)
	{
		CombatInterface->GetOnASCRegistered().AddWeakLambda(this,[this](UAbilitySystemComponent* InASC)
		{
			InASC->RegisterGameplayTagEvent(DeBuffTag,EGameplayTagEventType::NewOrRemoved).AddUObject(this,&UDeBuffNiagaraComponent::DeBuffTagChanged);
		});
	}
	if (CombatInterface)
	{
		CombatInterface->GetOnDeathDelegate().AddDynamic(this,&UDeBuffNiagaraComponent::OnOwnerDeath);
	}
	
}

void UDeBuffNiagaraComponent::DeBuffTagChanged(const FGameplayTag CallBackTag, int32 NewCount)
{
	GEngine->AddOnScreenDebugMessage(-1,5.f,FColor::Red,FString::Printf(TEXT("DeBuffTag:%d"),NewCount));
	if (NewCount > 0)
	{
		Activate();
	}
	else
	{
		Deactivate();
	}
}

void UDeBuffNiagaraComponent::OnOwnerDeath(AActor* DeadActor)
{
	Deactivate();
}
