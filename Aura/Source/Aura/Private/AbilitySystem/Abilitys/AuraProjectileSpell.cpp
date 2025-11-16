// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilitys/AuraProjectileSpell.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AuraGameplayTags.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "Interaction/CombatInterface.h"
#include "MyActor/AuraProjectile.h"

void UAuraProjectileSpell::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                           const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                           const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	
}

void UAuraProjectileSpell::SpawnProjectile(const FVector & ProjectileTargetLocation)
{
	//在服务器调用
	const bool bIsServer=GetAvatarActorFromActorInfo()->HasAuthority();
	if (!bIsServer)return;
	//自动转化调用Base中的GetCombatSocketLocation方法
		//const FVector SocketLocation=CombatInterface->GetCombatSocketLocation();
	const FVector SocketLocation = ICombatInterface::Execute_GetCombatSocketLocation(GetAvatarActorFromActorInfo(),FAuraGameplayTags::Get().Montage_Attack_Weapon);
	FRotator Rotation=(ProjectileTargetLocation-SocketLocation).Rotation();
	FTransform SpawnTransform;
	SpawnTransform.SetLocation(SocketLocation);
	SpawnTransform.SetRotation(Rotation.Quaternion());

	AAuraProjectile* Projectile = GetWorld()->SpawnActorDeferred<AAuraProjectile>(
			ProjectileClass,//生成对象类
			SpawnTransform,//生成位置
			GetOwningActorFromActorInfo(),  // 拥有者
			Cast<APawn>(GetAvatarActorFromActorInfo()),  // 引发者
			ESpawnActorCollisionHandlingMethod::AlwaysSpawn);//碰撞处理方式
	check(Projectile);

	//将当前的SpecHandle传递给生成的Projectile
	UAbilitySystemComponent* SourceASC=UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetAvatarActorFromActorInfo());
	//空上下文
	//FGameplayEffectSpecHandle SpecHandle=SourceASC->MakeOutgoingSpec(DamageEffectClass,GetAbilityLevel(),SourceASC->MakeEffectContext());

	//完善上下文内容 填充Context
	FGameplayEffectContextHandle EffectContextHandle=SourceASC->MakeEffectContext();
	EffectContextHandle.SetAbility(this);
	EffectContextHandle.AddSourceObject(Projectile);
	//将投射物Actor添加到效果上下文的Actor列表中
	TArray<TWeakObjectPtr<AActor>> Actors;
	Actors.Add(Projectile);
	EffectContextHandle.AddActors(Actors);
	FHitResult HitResult;
	HitResult.Location=ProjectileTargetLocation;
	EffectContextHandle.AddHitResult(HitResult);
	FGameplayEffectSpecHandle SpecHandle=SourceASC->MakeOutgoingSpec(DamageEffectClass,GetAbilityLevel(),EffectContextHandle);
	Projectile->DamageEffectSpecHandle=SpecHandle;
	
	//遍历每种伤害类型，为伤害类型标签绑定对应的伤害
	for (auto Pair:DamageTypes)
	{
		const float ScaleDamage=Pair.Value.GetValueAtLevel(GetAbilityLevel());
		UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle,Pair.Key,ScaleDamage);
	}
		
	Projectile->FinishSpawning(SpawnTransform);
}











