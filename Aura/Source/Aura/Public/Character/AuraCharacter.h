// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraComponent.h"
#include "Character/MyCharacterBase.h"
#include "GameFramework/SpringArmComponent.h"
#include "Interaction/PlayerInterface.h"
#include  "NiagaraComponent.h"
#include "AuraCharacter.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API AAuraCharacter : public AMyCharacterBase,public IPlayerInterface
{
	GENERATED_BODY()
public:
	AAuraCharacter();
	
	virtual void BeginPlay() override;
	
	//PossessedBy(针对服务器)：当Pawn被Controller占据（控制）时，执行服务器端的初始化设置。（因此针对玩家初始化要使用这个函数）
	virtual void PossessedBy(AController* NewController) override;
	//OnRep_PlayerState（针对客户端）它在客户端的 PlayerState 被复制更新时自动调用
	virtual void OnRep_PlayerState() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UNiagaraComponent> LevelUpNiagaraComponent;

	
private:
	virtual void InitialAbilityActorInfo()override;
	virtual int32 GetXP_Implementation() override;
	virtual int32 FindLevelForXP_Implementation(int32 InXP) const override;
	virtual int32 GetAttributePointReward_Implementation(int32 InLevel) const override;
	virtual int32 GetSpellPointReward_Implementation(int32 InLevel) const override;
	virtual void AddToXP_Implementation(int32 InXP) override;
	virtual void AddToPlayerLevel_Implementation(int32 InPlayerLevel) override;
	virtual void AddToAttributePoints_Implementation(int32 InAttributePoints) override;
	virtual void AddToSpellPoints_Implementation(int32 InSpellPoints) override;
	virtual void LevelUp_Implementation() override;
	virtual int32 GetPlayerLevel_Implementation() override;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UCameraComponent> TopDownCameraComponent;
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USpringArmComponent> CameraBoom;

	UFUNCTION(NetMulticast, Reliable)
	void MulticastLevelUpParticles()const;

	virtual  int32 GetAttributePoints_Implementation() const override;
};
