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
	//客户端未命中
	if (!bHit&&!HasAuthority())
	{
		//播放爆炸特效和声音
		UGameplayStatics::PlaySoundAtLocation(this,ImpactSound,GetActorLocation());
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(this,ImpactEffect,GetActorLocation());
		/*if (LoopingSoundComponent)
		{
			LoopingSoundComponent->Stop();
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("找不到"));
		}*/
		bHit=true;
	}
	Super::Destroyed();
}

void AAuraProjectile::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                      UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (GetInstigator()==nullptr)
	{
		return;
	}
	if (!OtherActor || !OtherActor->IsValidLowLevel())
	{
		return;
	}
	if (GetInstigator()==OtherActor)return;
	if (!UAuraAbilitySystemLibrary::IsNotFriend(GetInstigator(),OtherActor))return;

	//命中时播放特效和声音
	UGameplayStatics::PlaySoundAtLocation(this,ImpactSound,GetActorLocation());
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(this,ImpactEffect,GetActorLocation());
	/*if (LoopingSoundComponent)
	{
		LoopingSoundComponent->Stop();
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("找不到"));
	}*/
	//服务器执行销毁函数
	if (HasAuthority())
	{
		if (UAbilitySystemComponent* TargetASC=UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OtherActor))
		{
			TargetASC->ApplyGameplayEffectSpecToSelf(*DamageEffectSpecHandle.Data.Get());
		}
		Destroy();
	}
	//客户端设置销毁状态为Ture
	else
	{
		bHit=true;
	}
}



