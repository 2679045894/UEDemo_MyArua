// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SphereComponent.h"
#include "GameFramework/PlayerStart.h"
#include "Interaction/SaveInterface.h"
#include "CheckPoint.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API ACheckPoint : public APlayerStart,public ISaveInterface
{
	GENERATED_BODY()
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TObjectPtr<UStaticMeshComponent> CheckPointMesh;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TObjectPtr<USphereComponent> SphereComponent;

	ACheckPoint(const FObjectInitializer& ObjectInitializer);
	virtual void BeginPlay() override;

	UFUNCTION()
	virtual void OnSphereComponentOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	//检查点激活后的处理
	UFUNCTION(BlueprintImplementableEvent,BlueprintCallable)
	void CheckPointReached(UMaterialInstanceDynamic* DynamicMaterialInstance);

	//碰撞后，检查点被激活时触发
	UFUNCTION()
	void HandleGlowEffect();

	UPROPERTY()
	bool bReached=false;

	virtual void LoadActor_Implementation() override;

	virtual bool ShouldLoadTransform_Implementation() override{return false;};
};
