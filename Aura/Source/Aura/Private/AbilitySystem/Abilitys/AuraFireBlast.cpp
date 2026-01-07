// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilitys/AuraFireBlast.h"

#include "AuraGameplayTags.h"
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
	float Cooldown=GetCooldown(Level);
	float ManaCost=FMath::Abs(GetManaCost(Level));
	float Damage=GetDamageByDamageType(Level,FAuraGameplayTags::Get().Damage_Fire);
	return FString::Printf(TEXT(
	"<Title>%s</>\n"
	"等级:<Level>%i</>\n"
	"技能冷却:<Cooldown>%.1f</>\n"
	"蓝量消耗:<ManaCost>%.1f</>\n"
    
	"<Default>以自身为中心释放</><NumProjectiles>%i</><Default>颗火球环绕飞行，火球会向外发射并在最远距离后返回，对路径上的敌人造成</><Damage>%f</><Default>点火焰伤害，返回时造成二次伤害并有几率点燃敌人。</>"),
	*Title,
	Level,
	Cooldown,
	ManaCost,
	FMath::Min(Level, NumProjectiles),
	Damage
	);
}
