// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AuraAbilityTypes.h"
#include "AbilitySystem/Abilitys/AuraGameplayAbility.h"
#include "Interaction/CombatInterface.h"
#include "AuraDamageGameplayAbility.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UAuraDamageGameplayAbility : public UAuraGameplayAbility
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere,Category="Projectile")
	TSubclassOf<UGameplayEffect> DamageEffectClass;

	UPROPERTY(EditAnywhere, Category="Projectile")
	TMap<FGameplayTag,FScalableFloat> DamageTypes;

	UFUNCTION(BlueprintCallable)
	void CauseDamage(AActor* TargetActor);

	UFUNCTION(BlueprintPure)
	static FTaggedMontage GetRandomTaggedMontageFromArray(const TArray<FTaggedMontage> &TaggedMontages);

	UFUNCTION(BlueprintPure)
	float GetDamageByDamageType(float InLevel,const FGameplayTag& DamageType);
/////////////////////DeBuff////////////////////
	UPROPERTY(EditDefaultsOnly, Category="Damage")
	FGameplayTag DebuffDamageType=FGameplayTag();

	UPROPERTY(EditDefaultsOnly, Category="Damage")
	float DeBuffChance=20.f;

	UPROPERTY(EditDefaultsOnly, Category="Damage")
	float DeBuffDamage=5.f;

	UPROPERTY(EditDefaultsOnly, Category="Damage")
	float DeBuffDuration=5.f;

	UPROPERTY(EditDefaultsOnly, Category="Damage")
	float DeBuffFrequency=1.f;

	UPROPERTY(EditDefaultsOnly, Category="Damage")
	float DeathImpulseMagnitude=60.f;

	UPROPERTY(EditDefaultsOnly, Category="Damage")
	float KnockForceChance=50.f;

	UPROPERTY(EditDefaultsOnly, Category="Damage")
	float KnockForceMagnitude=1000.f;

	UFUNCTION(BlueprintPure)
	FDamageEffectParams MakeDamageEffectParamsFromClassDefault(AActor* TargetActor=nullptr);
};
