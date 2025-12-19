// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PointCollection.generated.h"

UCLASS()
class AURA_API APointCollection : public AActor
{
	GENERATED_BODY()
	
public:	
	APointCollection();
	/**
	 * 
	 * @param GroundLocation 地面位置
	 * @param NumPoint 需要返回的数量
	 * @param YawOverride 设置actor旋转的值
	 * @return 
	 */
	UFUNCTION(BlueprintPure)
	TArray<USceneComponent*> GetGroundPoints(const FVector& GroundLocation,int32 NumPoint,float YawOverride=0.f);

protected:
	virtual void BeginPlay() override;

	/**
	 * 初始化使用，减少重复代码，创建多个点并添加到组件下面
	 * @param Pt 创建的场景组件引用
	 * @param Name 场景组件名称
	 */
	void CreateSceneComponent(TObjectPtr<USceneComponent>& Pt,FName Name);

	//存储下面场景组件的数组
	UPROPERTY(BlueprintReadOnly,VisibleAnywhere)
	TArray<USceneComponent*> ImmutablePts;

	//场景的跟组件，作为检测位置的根
	UPROPERTY(BlueprintReadOnly,VisibleAnywhere)
	TObjectPtr<USceneComponent> Pt_0;

	UPROPERTY(BlueprintReadOnly,VisibleAnywhere)
	TObjectPtr<USceneComponent> Pt_1;

	UPROPERTY(BlueprintReadOnly,VisibleAnywhere)
	TObjectPtr<USceneComponent> Pt_2;

	UPROPERTY(BlueprintReadOnly,VisibleAnywhere)
	TObjectPtr<USceneComponent> Pt_3;

	UPROPERTY(BlueprintReadOnly,VisibleAnywhere)
	TObjectPtr<USceneComponent> Pt_4;

	UPROPERTY(BlueprintReadOnly,VisibleAnywhere)
	TObjectPtr<USceneComponent> Pt_5;

	UPROPERTY(BlueprintReadOnly,VisibleAnywhere)
	TObjectPtr<USceneComponent> Pt_6;

	UPROPERTY(BlueprintReadOnly,VisibleAnywhere)
	TObjectPtr<USceneComponent> Pt_7;

	UPROPERTY(BlueprintReadOnly,VisibleAnywhere)
	TObjectPtr<USceneComponent> Pt_8;

	UPROPERTY(BlueprintReadOnly,VisibleAnywhere)
	TObjectPtr<USceneComponent> Pt_9;

	UPROPERTY(BlueprintReadOnly,VisibleAnywhere)
	TObjectPtr<USceneComponent> Pt_10;
	
};
