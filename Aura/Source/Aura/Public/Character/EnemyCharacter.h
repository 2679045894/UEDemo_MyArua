// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Data/CharacterClassInfo.h"
#include "AI/AuraAIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Character/MyCharacterBase.h"
#include "Components/WidgetComponent.h"
#include "Interaction/EnemyInterface.h"
#include "UI/WidgeController/OverplayWidgetController.h"
#include "EnemyCharacter.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API AEnemyCharacter : public AMyCharacterBase,public IEnemyInterface
{
	GENERATED_BODY()

	virtual void BeginPlay() override;
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TObjectPtr<USoundBase> HurtSound;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TObjectPtr<USoundBase> DeadSound;
	AEnemyCharacter();
	virtual void HighlightActor() override;
	virtual void UnHighlightActor() override;
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Highlighting")
	bool bHighlight;

	virtual void InitialAbilityActorInfo() override;

	virtual void InitializeDefaultAttributes() const override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Level")
	int32 Level=1;
	
	UPROPERTY(BlueprintAssignable)
	FOnAttributeChangedSignature OnHealthChange;
	UPROPERTY(BlueprintAssignable)
	FOnAttributeChangedSignature OnMaxHealthChange;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UWidgetComponent> HealthBar;
	
	UPROPERTY(BlueprintReadOnly)
	bool bHitReacting=false;

	//委托绑定函数
	UFUNCTION()
	void HitReactTagChanged(const FGameplayTag GameplayTag,int32 NewCount);
	//当前敌人的受击动画蒙太奇
	UPROPERTY(EditDefaultsOnly,Category="Combat")
	TObjectPtr<UAnimMontage> HitReactMontage;
	//重写战斗接口的获取蒙太奇函数
	virtual UAnimMontage* GetHitReactMontage_Implementation() override;

	float LifeSpan=5.f;

	virtual void Die(const FVector& DeathImpulse) override;

	UFUNCTION(BlueprintCallable)
	void SetLevel(int32 InLevel);


	UPROPERTY(EditAnywhere, BlueprintReadOnly,Category="AI")
	TObjectPtr<UBehaviorTree> BehaviorTree;
	UPROPERTY()
	TObjectPtr<AAuraAIController> AuraAIController;

	UFUNCTION()
	virtual void PossessedBy(AController* NewController) override;

	UPROPERTY()
	TObjectPtr<AActor> TargetActor;

	virtual void SetCombatTarget_Implementation(AActor* InCombatTarget) override;

	virtual AActor* GetCombatTarget_Implementation() override;

	UFUNCTION(BlueprintCallable)
	virtual int32 GetPlayerLevel_Implementation() override;

	UFUNCTION(BlueprintImplementableEvent)
	void DropLoot();

	virtual void PlayDeadSound_Implementation() override;

	virtual void PlayHurtSound_Implementation() override;
};
