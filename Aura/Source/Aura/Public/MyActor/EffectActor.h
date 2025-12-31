// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffect.h"
#include "GameFramework/Actor.h"
#include "EffectActor.generated.h"
UENUM(BlueprintType)
enum class EEffectApplicationPolicy:uint8
{
	ApplyOnOverlap,
	ApplyOnEndOverlap,
	DoNotApply
};

UENUM(BlueprintType)
//给InfiniteEffect使用
enum class EEffectRemovalPolicy:uint8
{
	// 当结束重叠时移除效果
	RemoveOnEndOverlap,
	// 不移除效果
	DoNotRemove
};
UCLASS()
class AURA_API AEffectActor : public AActor
{
	GENERATED_BODY()
	
public:	
	AEffectActor();

protected:
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;

public:
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="SceneComponents")
	TObjectPtr<USceneComponent> SceneComponent;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Applied Effects")
	TSubclassOf<UGameplayEffect> InstantGameplayEffectClass;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Applied Effects")
	EEffectApplicationPolicy InstantEffectApplicationPolicy=EEffectApplicationPolicy::DoNotApply;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Applied Effects")
	TSubclassOf<UGameplayEffect> DurationGameplayEffectClass;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Applied Effects")
	EEffectApplicationPolicy DurationEffectApplicationPolicy=EEffectApplicationPolicy::DoNotApply;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Applied Effects")
	TSubclassOf<UGameplayEffect> InfiniteGameplayEffectClass;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Applied Effects")
	EEffectApplicationPolicy InfiniteEffectApplicationPolicy=EEffectApplicationPolicy::DoNotApply;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Applied Effects")
	EEffectRemovalPolicy InfiniteEffectRemovalPolicy=EEffectRemovalPolicy::DoNotRemove;
	
	
	UFUNCTION(BlueprintCallable)
	//效果应用器
	void ApplyEffectToActor(AActor* TargetActor,TSubclassOf<UGameplayEffect> GameplayEffectClass);

	UFUNCTION(BlueprintCallable)
	void OnOverlap(AActor* TargetActor);
	UFUNCTION(BlueprintCallable)
	void OnEndOverlap(AActor* TargetActor);

	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Applied Effects")
	bool bDestroyOnEffectApplication=false;

	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Applied Effects")
	bool bApplyEffectsToEnemies=false;

	//跟踪和管理应用的无限持续时间效果。
	UPROPERTY()
	TMap<FActiveGameplayEffectHandle,UAbilitySystemComponent*> ActiveEffectHandles;

	//物品级别
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="EffectActor Level")
	float ActorLevel=1.f;

	UPROPERTY(BlueprintReadWrite)
	FVector CalculatedLocation;

	//计算后的Actor旋转
	UPROPERTY(BlueprintReadWrite)
	FRotator CalculatedRotation;

	//Actor石是否帧更新旋转
	UPROPERTY(BlueprintReadWrite)
	bool bRotates=false;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	float RotationRate=45.f;

	//Actor是否更新位置
	UPROPERTY(BlueprintReadWrite)
	bool bSinusoidalMovement=false;

	//正弦值-1到1,调整更新移动范围
	UPROPERTY(BlueprintReadWrite)
	float SineAmplitude=1.f;

	//正弦运算，默认值为1秒一个循环
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	float SinePeriod=1.f;

	UFUNCTION(BlueprintCallable)
	void StartSinusoidalMovement();

	UFUNCTION(BlueprintCallable)
	void StartRotation();
private:
	UPROPERTY()
	float RunningTime=0.f;
	UPROPERTY()
	FVector InitialLocation;
	void ItemMovement(float DeltaTime);
	
	
	
};
