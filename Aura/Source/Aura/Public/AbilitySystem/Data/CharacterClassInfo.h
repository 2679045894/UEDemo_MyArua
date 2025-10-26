// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffect.h"
#include "Engine/DataAsset.h"
#include "CharacterClassInfo.generated.h"

UENUM(BlueprintType)
enum class ECharacterClass : uint8
{
	Elementalist,
	Warrior,
	Ranger
};

USTRUCT(BlueprintType)
struct FCharacterClassDefault
{
	GENERATED_BODY()
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly,Category="Class Default")
	TSubclassOf<UGameplayEffect> PrimaryAttributeClass;
};

UCLASS()
class AURA_API UCharacterClassInfo : public UDataAsset
{
	GENERATED_BODY()
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly,Category="Class Default")
	TMap<ECharacterClass, FCharacterClassDefault> CharacterClassInformation;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly,Category="Class Default")
	TSubclassOf<UGameplayEffect> SecondaryAttributeClass;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly,Category="Class Default")
	TSubclassOf<UGameplayEffect> VitalAttributeClass;

	
	FCharacterClassDefault GetCharacterClassDefaultInfo(ECharacterClass CharacterClass);
};
