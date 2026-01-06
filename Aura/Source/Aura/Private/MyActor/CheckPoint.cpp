// Fill out your copyright notice in the Description page of Project Settings.


#include "MyActor/CheckPoint.h"

#include "Game/MyGameModeBase.h"
#include "Interaction/PlayerInterface.h"
#include "Kismet/GameplayStatics.h"

ACheckPoint::ACheckPoint(const FObjectInitializer& ObjectInitializer): Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick=false;
	CheckPointMesh=CreateDefaultSubobject<UStaticMeshComponent>(FName("CheckPointMesh"));
	CheckPointMesh->SetupAttachment(GetRootComponent());
	SphereComponent=CreateDefaultSubobject<USphereComponent>(FName("SphereComponent"));
	SphereComponent->SetupAttachment(GetRootComponent());

	CheckPointMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	CheckPointMesh->SetCollisionResponseToAllChannels(ECR_Block);
	
	SphereComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	SphereComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	SphereComponent->SetCollisionResponseToChannel(ECC_Pawn,ECR_Overlap);
}

void ACheckPoint::BeginPlay()
{
	Super::BeginPlay();
	SphereComponent->OnComponentBeginOverlap.AddDynamic(this,&ACheckPoint::OnSphereComponentOverlap);
}

void ACheckPoint::OnSphereComponentOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->Implements<UPlayerInterface>())
	{
		bReached=true;
		if (AMyGameModeBase* AuraGameMode=Cast<AMyGameModeBase>(UGameplayStatics::GetGameMode(this)))
		{
			//保存场景状态
			AuraGameMode->SaveWorldState(GetWorld(),FString(""));
		}
		//修改存档当前的碰撞点
		IPlayerInterface::Execute_SaveProgress(OtherActor,PlayerStartTag);
		HandleGlowEffect();
	}
}

void ACheckPoint::HandleGlowEffect()
{
	//取消碰撞检查
	CheckPointMesh->SetCollisionResponseToAllChannels(ECR_Ignore);
	SphereComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	UMaterialInstanceDynamic* DynamicMaterial=UMaterialInstanceDynamic::Create(CheckPointMesh->GetMaterial(0),this);
	CheckPointMesh->SetMaterial(0, DynamicMaterial);
	CheckPointReached(DynamicMaterial);
}

void ACheckPoint::LoadActor_Implementation()
{
	if (bReached)
	{
		HandleGlowEffect();
	}
}
