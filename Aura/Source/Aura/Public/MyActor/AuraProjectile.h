// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AuraAbilityTypes.h"
#include "GameplayEffect.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Actor.h"
#include "GameFramework/ProjectileMovementComponent.h"

#include "AuraProjectile.generated.h"

class UNiagaraSystem;
UCLASS()
class AURA_API AAuraProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	AAuraProjectile();

protected:

	virtual void BeginPlay() override;
	virtual void Destroyed() override;

public:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USphereComponent> SphereComponent;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UProjectileMovementComponent> ProjectileMovementComponent;
	
	//爆炸音效
	UPROPERTY(EditAnywhere,Category="Imapct")
	TObjectPtr<USoundBase> ImpactSound;
	//飞行过程中的循环音效
	UPROPERTY(EditAnywhere,Category="Imapct")
	TObjectPtr<USoundBase> LoopingSound;
	//爆炸粒子
	UPROPERTY(EditAnywhere,Category="Imapct")
	TObjectPtr<UNiagaraSystem> ImpactEffect;
	//飞行过程中的循环音效组件
	UPROPERTY()
	TObjectPtr<UAudioComponent> LoopingSoundComponent;

	//一个场景组件，用于确定当前投掷物的攻击目标
	UPROPERTY()
	TObjectPtr<USceneComponent> HomingTargetSceneComponent;

	UPROPERTY(EditDefaultsOnly)
	//自动销毁时间
	float LifeSpan = 15.f;
	UPROPERTY(BlueprintReadOnly)
	bool bHit=false;
	UFUNCTION(BlueprintCallable)
	void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
	UFUNCTION()
	void OnHit();
	UPROPERTY()
	FDamageEffectParams DamageEffectParams;
};
