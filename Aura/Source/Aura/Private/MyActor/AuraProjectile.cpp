// Fill out your copyright notice in the Description page of Project Settings.


#include "MyActor/AuraProjectile.h"
#include "NiagaraFunctionLibrary.h"
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

	ProjectileMovementComponent=CreateDefaultSubobject<UProjectileMovementComponent>(FName("ProjectileMovementComponent"));
	ProjectileMovementComponent->InitialSpeed=450.f;
	ProjectileMovementComponent->MaxSpeed=550.f;
	ProjectileMovementComponent->ProjectileGravityScale=0.f;
	
	
}


void AAuraProjectile::BeginPlay()
{
	Super::BeginPlay();
	SphereComponent->OnComponentBeginOverlap.AddDynamic(this,&AAuraProjectile::OnSphereOverlap);
	//设置自动销毁时间
	SetLifeSpan(LifeSpan);
	//将飞行过程中的循环音效添加到组件上
	LoopingSoundComponent=UGameplayStatics::SpawnSoundAttached(LoopingSound,GetRootComponent());
}

void AAuraProjectile::Destroyed()
{
	//客户端未命中
	if (!bHit&&!HasAuthority())
	{
		//播放爆炸特效和声音
		UGameplayStatics::PlaySoundAtLocation(this,ImpactSound,GetActorLocation());
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(this,ImpactEffect,GetActorLocation());
		LoopingSoundComponent->Stop();
	}
	Super::Destroyed();
}

void AAuraProjectile::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                      UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	GEngine->AddOnScreenDebugMessage(-1,5.f,FColor::Black,TEXT("触发"));
	//命中时播放特效和声音
	UGameplayStatics::PlaySoundAtLocation(this,ImpactSound,GetActorLocation());
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(this,ImpactEffect,GetActorLocation());
	LoopingSoundComponent->Stop();
	//服务器执行销毁函数
	if (HasAuthority())
	{
		Destroy();
	}
	//客户端设置销毁状态为Ture
	else
	{
		bHit=true;
	}
}


