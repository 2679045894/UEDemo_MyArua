// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/MyCharacter.h"

#include "GameFramework/CharacterMovementComponent.h"

AMyCharacter::AMyCharacter()
{
	//让角色面朝加速的方向
	GetCharacterMovement()->bOrientRotationToMovement=true;
	GEngine->AddOnScreenDebugMessage(-1,5.0f,FColor::Blue,FString::Printf(TEXT("name")));
	GetCharacterMovement()->RotationRate=FRotator(0.f,400.f,0.f);
	GetCharacterMovement()->bConstrainToPlane=true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	bUseControllerRotationPitch=false;
	bUseControllerRotationYaw=false;
	bUseControllerRotationRoll=false;
}
