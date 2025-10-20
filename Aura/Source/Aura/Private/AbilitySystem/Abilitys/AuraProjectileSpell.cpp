// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilitys/AuraProjectileSpell.h"

#include "Interaction/CombatInterface.h"
#include "MyActor/AuraProjectile.h"

void UAuraProjectileSpell::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                           const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                           const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	
}

void UAuraProjectileSpell::SpawnProjectile()
{
	GEngine->AddOnScreenDebugMessage(-1,5.f,FColor::Black,TEXT("发射"));
	//在服务器调用
	const bool bIsServer=GetAvatarActorFromActorInfo()->HasAuthority();
	if (!bIsServer)return;
	
	if (ICombatInterface* CombatInterface=Cast<ICombatInterface>(GetAvatarActorFromActorInfo()))
	{
		//自动转化调用Base中的GetCombatSocketLocation方法
		const FVector SocketLocation=CombatInterface->GetCombatSocketLocation();
		FTransform SpawnTransform;
		SpawnTransform.SetLocation(SocketLocation);

		AAuraProjectile* Projectile = GetWorld()->SpawnActorDeferred<AAuraProjectile>(
			ProjectileClass,//生成对象类
			SpawnTransform,//生成位置
			GetOwningActorFromActorInfo(),  // 拥有者
			Cast<APawn>(GetOwningActorFromActorInfo()),  // 引发者
			ESpawnActorCollisionHandlingMethod::AlwaysSpawn);//碰撞处理方式
		check(Projectile);
		Projectile->FinishSpawning(SpawnTransform);
	}
}
