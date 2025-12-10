// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilitys/AuraFireBolt.h"

#include "AuraGameplayTags.h"
#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "Kismet/KismetSystemLibrary.h"

void UAuraFireBolt::SpawnProjectiles(const FVector& ProjectileTargetLocation,FName SocketName,const FGameplayTag& SocketTag,
                                     bool bOverridePitch, float PitchOverride,AActor* HomingTarget)
{
	bool bIsServe=GetAvatarActorFromActorInfo()->HasAuthority();
	if (!bIsServe)return;
	if (GetAvatarActorFromActorInfo()->Implements<UCombatInterface>())
	{
		NumProjectiles=FMath::Max(MaxNumProjectiles,GetAbilityLevel());
		//获取释放位置
		FVector SocketLocation=ICombatInterface::Execute_GetCombatSocketLocation(GetAvatarActorFromActorInfo(),SocketTag);
		FRotator Rotation=(ProjectileTargetLocation-SocketLocation).Rotation();
		//覆写发射角度
		if (bOverridePitch)Rotation.Pitch=PitchOverride;
		const FVector Forward=Rotation.Vector();
		TArray<FRotator> Rotations=UAuraAbilitySystemLibrary::EvenlySpacedRotators(Forward,FVector::UpVector,ProjectileSpread,NumProjectiles);
		for (FRotator& Rot : Rotations)
		{
			FTransform SpawnTransform;
			SpawnTransform.SetLocation(SocketLocation);
			SpawnTransform.SetRotation(Rot.Quaternion());
			AAuraProjectile* Projectile=GetWorld()->SpawnActorDeferred<AAuraProjectile>(
				ProjectileClass,
				SpawnTransform,
				GetOwningActorFromActorInfo(),
				Cast<APawn>(GetAvatarActorFromActorInfo()),
				ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
			Projectile->DamageEffectParams=MakeDamageEffectParamsFromClassDefault();
				
			UKismetSystemLibrary::DrawDebugArrow(GetAvatarActorFromActorInfo(), SocketLocation, SocketLocation + Rot.Vector() * 100.f, 5, FLinearColor::Green, 120, 5);

			if (HomingTarget&&HomingTarget->Implements<UCombatInterface>())
			{
				//直接将该目标的根组件设为追踪目标：
				Projectile->ProjectileMovementComponent->HomingTargetComponent=HomingTarget->GetRootComponent();
			}
			else
			{
				//创建一个临时的 USceneComponent 对象
				Projectile->HomingTargetSceneComponent=NewObject<USceneComponent>(UClass::StaticClass());
				//将该组件的位置设置为指定的目标位置 
				Projectile->HomingTargetSceneComponent->SetWorldLocation(ProjectileTargetLocation);
				//将这个临时组件设为追踪目标
				Projectile->ProjectileMovementComponent->HomingTargetComponent=Projectile->HomingTargetSceneComponent;
			}
			Projectile->ProjectileMovementComponent->HomingAccelerationMagnitude=FMath::RandRange(HomingAccelerationMin,HomingAccelerationMax);
			Projectile->ProjectileMovementComponent->bIsHomingProjectile=bLaunchHomingProjectiles;

			Projectile->FinishSpawning(SpawnTransform);
		}
	}
}

float UAuraFireBolt::GetManaCost(float InLevel) const
{
	float ManaCost = 0.f;
	if (UGameplayEffect* CostEffect=GetCostGameplayEffect())
	{
		for (FGameplayModifierInfo Info:CostEffect->Modifiers)
		{
			if (Info.Attribute==UAuraAttributeSet::GetManaAttribute())
			{
				Info.ModifierMagnitude.GetStaticMagnitudeIfPossible(InLevel,ManaCost);
				break;
			}
		}
	}
	return ManaCost;
}

float UAuraFireBolt::GetCooldown(float InLevel) const
{
	float Cooldown = 0.f;
	if (UGameplayEffect* CooldownEffect=GetCostGameplayEffect())
	{
		CooldownEffect->DurationMagnitude.GetStaticMagnitudeIfPossible(InLevel,Cooldown);
	}
	return Cooldown;
}

FString UAuraFireBolt::GetDescription(int32 Level)
{
	return GetDescriptionAtLevel(Level,L"火球术");
}

FString UAuraFireBolt::GetNextLevelDescription(int32 Level)
{
	return GetDescriptionAtLevel(Level+1,L"下一等级");
}

FString UAuraFireBolt::GetDescriptionAtLevel(int32 Level, const FString& Title)
{
	float Cooldown = GetCooldown(Level);
	float ManaCost=GetManaCost(Level);
	int32 Damage=GetDamageByDamageType(Level,FAuraGameplayTags::Get().Damage_Fire);

	return FString::Printf(TEXT(
		"<Title>%s</>\n"
		"等级:<Level>%i</>\n"
		"技能冷却<Cooldown>%.1f</>\n"
		"蓝量消耗<ManaCost>%.1f</>\n"

		"<Default>发射 %i 颗火球，在发生撞击时产生爆炸，并造成</> <Damage>%i</> <Default>点火焰伤害，并有一定几率燃烧。</>"),
		*Title,
		Level,
		Cooldown,
		ManaCost,
		FMath::Min(Level,NumProjectiles),
		Damage
		);
}
