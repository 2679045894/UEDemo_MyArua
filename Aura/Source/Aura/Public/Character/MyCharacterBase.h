// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "AbilitySystem/AruaAttributeSet.h"
#include "MyCharacterBase.generated.h"

UCLASS()
class AURA_API AMyCharacterBase : public ACharacter,public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	AMyCharacterBase();

protected:

	virtual void BeginPlay() override;

public:	

	virtual void Tick(float DeltaTime) override;


	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//必须实现这个函数，因为调用了接口
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	//自定义的一个函数，用于外部访问私有变量（AttributeSet）
	UAttributeSet* GetAttributeSet() const {return AttributeSet;}

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TObjectPtr<USkeletalMeshComponent> Weapon;
protected:
	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;
	
	UPROPERTY()
	TObjectPtr<UAttributeSet> AttributeSet;
	
};
