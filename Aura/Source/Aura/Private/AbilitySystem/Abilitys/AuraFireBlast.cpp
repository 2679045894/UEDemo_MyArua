// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilitys/AuraFireBlast.h"

#include "AbilitySystem/AuraAbilitySystemLibrary.h"

TArray<AAuraFireBall*> UAuraFireBlast::SpawnFireBall()
{
	const FVector Forward=GetAvatarActorFromActorInfo()->GetActorForwardVector();
	const FVector Location=GetAvatarActorFromActorInfo()->GetActorLocation();
	TArray<AAuraFireBall*> FireBalls;
	TArray<FRotator> Rotators=UAuraAbilitySystemLibrary::EvenlySpacedRotators(Forward,FVector::UpVector,360.f,NumFireBalls);
	for (FRotator Rotator:Rotators)
	{
		FTransform SpawnTransform;
		SpawnTransform.SetLocation(Location);
		SpawnTransform.SetRotation(Rotator.Quaternion());
		AAuraFireBall* FireBall=GetWorld()->SpawnActorDeferred<AAuraFireBall>(
			FireBallClass,
			SpawnTransform,
			GetOwningActorFromActorInfo(),
			CurrentActorInfo->PlayerController->GetPawn(),
			ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
		FireBall->ReturnToActor=GetAvatarActorFromActorInfo();
		FireBall->DamageEffectParams=MakeDamageEffectParamsFromClassDefault();
		FireBalls.Add(FireBall);

		//在配置完成火球配置后，调用FinishSpawning将火球正式添加到场景中
		FireBall->FinishSpawning(SpawnTransform);
		FireBall->ExplodeEffectParams=MakeDamageEffectParamsFromClassDefault();
	}
	return FireBalls;
}

FString UAuraFireBlast::GetDescription(int32 Level)
{
	return GetDescriptionAtLevel(Level,AbilityName);
}

FString UAuraFireBlast::GetNextLevelDescription(int32 Level)
{
	return GetDescriptionAtLevel(Level+1,TEXT("下一等级"));
}

FString UAuraFireBlast::GetDescriptionAtLevel(int32 Level, const FString& Title)
{
	return Super::GetDescriptionAtLevel(Level, Title);
}
