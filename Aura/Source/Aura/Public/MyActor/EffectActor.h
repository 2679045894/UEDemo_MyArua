// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffect.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Actor.h"
#include "EffectActor.generated.h"

UCLASS()
class AURA_API AEffectActor : public AActor
{
	GENERATED_BODY()
	
public:	
	AEffectActor();

protected:
	virtual void BeginPlay() override;

public:
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="SceneComponents")
	TObjectPtr<USceneComponent> SceneComponent;
	UPROPERTY(EditAnywhere,Blueprintable,Category="Applied Effects")
	TSubclassOf<UGameplayEffect> InstantGameplayEffectClass;
	UFUNCTION(BlueprintCallable)
	//效果应用器
	void ApplyEffectToActor(AActor* TargetActor,TSubclassOf<UGameplayEffect> GameplayEffectClass);
};
