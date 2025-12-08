// Fill out your copyright notice in the Description page of Project Settings.


#include "MyActor/AuraProjectile.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "NiagaraFunctionLibrary.h"
#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "Aura/Aura.h"
#include "Components/AudioComponent.h"
#include "kismet/GameplayStatics.h"

// Sets default values
AAuraProjectile::AAuraProjectile()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates=true;
	SphereComponent=CreateDefaultSubobject<USphereComponent>(FName("SphereComponent"));
	SetRootComponent(SphereComponent);
	SphereComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	SphereComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	SphereComponent->SetCollisionResponseToChannel(ECC_WorldDynamic,ECR_Overlap);
	SphereComponent->SetCollisionResponseToChannel(ECC_WorldStatic,ECR_Overlap);
	SphereComponent->SetCollisionResponseToChannel(ECC_Pawn,ECR_Overlap);
	SphereComponent->SetCollisionObjectType(ECC_Projectile);

	ProjectileMovementComponent=CreateDefaultSubobject<UProjectileMovementComponent>(FName("ProjectileMovementComponent"));
	ProjectileMovementComponent->InitialSpeed=450.f;
	ProjectileMovementComponent->MaxSpeed=550.f;
	ProjectileMovementComponent->ProjectileGravityScale=0.f;
}


void AAuraProjectile::BeginPlay()
{
	Super::BeginPlay();
    
	if (SphereComponent)
	{
		SphereComponent->OnComponentBeginOverlap.AddDynamic(this,&AAuraProjectile::OnSphereOverlap);
	}
    
	SetLifeSpan(LifeSpan);
    
	/*// 只在服务器创建音效组件，客户端跳过
	if (HasAuthority()) // 或者使用 GetNetMode() != NM_Client
	{
		if (LoopingSound && GetRootComponent())
		{
			LoopingSoundComponent = UGameplayStatics::SpawnSoundAttached(LoopingSound, GetRootComponent());
			if (LoopingSoundComponent)
			{
				UE_LOG(LogTemp, Log, TEXT("Server successfully created LoopingSoundComponent"));
			}
		}
	}
	else
	{
		// 客户端明确设置为nullptr
		LoopingSoundComponent = nullptr;
		UE_LOG(LogTemp, Verbose, TEXT("Client skipping LoopingSoundComponent creation"));
	}*/
}


void AAuraProjectile::Destroyed()
{
	if (!bHit&&!HasAuthority())
	{
		OnHit();
	}
	Super::Destroyed();
}

void AAuraProjectile::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                      UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AActor* SourceAvatarActor=DamageEffectParams.SourceASC->GetAvatarActor();
	if (SourceAvatarActor==OtherActor)return;
	if (!UAuraAbilitySystemLibrary::IsNotFriend(SourceAvatarActor,OtherActor))return;
	if (!bHit)OnHit();
	if (HasAuthority())
	{
		if (UAbilitySystemComponent* TargetASC=UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OtherActor))
		{
			const FVector DeathImpulse=GetActorForwardVector()*DamageEffectParams.DeathImpulseMagnitude;
			DamageEffectParams.DeathImpulse=DeathImpulse;
			/*if (FMath::RandRange(1,100)<DamageEffectParams.KnockbackChance)
			{
				FVector KnockbackDirection=GetActorForwardVector().RotateAngleAxis(30.f,GetActorRightVector());
				KnockbackDirection.Normalize();
				DamageEffectParams.KnockbackForce=KnockbackDirection*DamageEffectParams.KnockbackForceMagnitude;
				// 检查Z轴分量（判断向上还是向下）
				if (KnockbackDirection.Z > 0.1f)
				{
					UE_LOG(LogTemp, Warning, TEXT("✅ 方向: 向上 (Z=%.3f)"), KnockbackDirection.Z);
				}
				else if (KnockbackDirection.Z < -0.1f)
				{
					UE_LOG(LogTemp, Warning, TEXT("⚠️  方向: 向下 (Z=%.3f)"), KnockbackDirection.Z);
				}
				else
				{
					UE_LOG(LogTemp, Warning, TEXT("➡️  方向: 水平 (Z=%.3f)"), KnockbackDirection.Z);
				}
			}*/
			if (FMath::RandRange(1,100) < DamageEffectParams.KnockbackChance)
			{
				// 获取火球击中时的方向（速度方向）
				FVector ProjectileVelocity = GetVelocity();
				if (ProjectileVelocity.SizeSquared() > 0.1f)
				{
					// 1. 使用速度的反方向作为基础击退方向（与飞来方向相反）
					FVector BackwardDirection = -ProjectileVelocity.GetSafeNormal();
        
					// 2. 确保击退方向主要是水平向后，但略微向上
					// 清空大部分垂直分量，重新添加向上的分量
					FVector HorizontalBackward = FVector(BackwardDirection.X, BackwardDirection.Y, 0.0f);
					HorizontalBackward.Normalize();
        
					// 3. 混合：70%水平向后 + 30%向上
					FVector Upward = FVector(0, 0, 1.0f);
					FVector KnockbackDirection = (HorizontalBackward * 0.7f + Upward * 0.3f).GetSafeNormal();
        
					KnockbackDirection.Normalize();
					DamageEffectParams.KnockbackForce = KnockbackDirection * DamageEffectParams.KnockbackForceMagnitude;
        
					UE_LOG(LogTemp, Warning, TEXT("火球速度方向: %s"), *ProjectileVelocity.GetSafeNormal().ToString());
					UE_LOG(LogTemp, Warning, TEXT("最终击飞方向: %s (Z=%.3f)"), *KnockbackDirection.ToString(), KnockbackDirection.Z);
				}
			}
			DamageEffectParams.TargetASC=TargetASC;
			UAuraAbilitySystemLibrary::ApplyDamageEffect(DamageEffectParams);
		}
		Destroy();
	}
	//客户端设置销毁状态为Ture
	else
	{
		bHit=true;
	}
}

void AAuraProjectile::OnHit()
{
	//命中时播放特效和声音
	UGameplayStatics::PlaySoundAtLocation(this,ImpactSound,GetActorLocation());
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(this,ImpactEffect,GetActorLocation());
	bHit=true;
}



