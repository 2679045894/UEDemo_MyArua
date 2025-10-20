// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilitys/AuraGameplayAbility.h"
#include "MyActor/AuraProjectile.h"
#include "AuraProjectileSpell.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UAuraProjectileSpell : public UAuraGameplayAbility
{
	GENERATED_BODY()

	//激活游戏能力
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	UFUNCTION(BlueprintCallable,Category="Projectile")
	virtual void SpawnProjectile();
	
	//火球类
	UPROPERTY(EditAnywhere)
	TSubclassOf<AAuraProjectile> ProjectileClass;
};
