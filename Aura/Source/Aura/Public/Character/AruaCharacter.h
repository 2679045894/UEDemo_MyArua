// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/MyCharacterBase.h"
#include "AruaCharacter.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API AAruaCharacter : public AMyCharacterBase
{
	GENERATED_BODY()
public:
	AAruaCharacter();
	

	virtual void BeginPlay() override;
	
	//PossessedBy(针对服务器)：当Pawn被Controller占据（控制）时，执行服务器端的初始化设置。（因此针对玩家初始化要使用这个函数）
	virtual void PossessedBy(AController* NewController) override;
	//OnRep_PlayerState（针对客户端）它在客户端的 PlayerState 被复制更新时自动调用
	virtual void OnRep_PlayerState() override;
private:
	void InitialAbilityActorInfo();
};
