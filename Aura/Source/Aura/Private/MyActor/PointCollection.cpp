// Fill out your copyright notice in the Description page of Project Settings.


#include "MyActor/PointCollection.h"

#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
APointCollection::APointCollection()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Pt_0=CreateDefaultSubobject<USceneComponent>(FName("Point_0"));
	ImmutablePts.Add(Pt_0);
	SetRootComponent(Pt_0);
	CreateSceneComponent(Pt_1,FName("Point_1"));
	CreateSceneComponent(Pt_2,FName("Point_2"));
	CreateSceneComponent(Pt_3,FName("Point_3"));
	CreateSceneComponent(Pt_4,FName("Point_4"));
	CreateSceneComponent(Pt_5,FName("Point_5"));
	CreateSceneComponent(Pt_6,FName("Point_6"));
	CreateSceneComponent(Pt_7,FName("Point_7"));
	CreateSceneComponent(Pt_8,FName("Point_8"));
	CreateSceneComponent(Pt_9,FName("Point_9"));
	CreateSceneComponent(Pt_10,FName("Point_10"));
}

/*TArray<USceneComponent*> APointCollection::GetGroundPoints(const FVector& GroundLocation, int32 NumPoint,
	float YawOverride)
{
	checkf(ImmutablePts.Num()>=NumPoint,TEXT("访问超出数组索引范围"));
	TArray<USceneComponent*> ArrayCopy;
	for (TObjectPtr<USceneComponent> Pt:ImmutablePts)
	{
		if (ArrayCopy.Num()>NumPoint)return ArrayCopy;
		if (Pt!=Pt_0)
		{
			//获取节点基于根组件的偏移
			FVector ToPoint=Pt->GetComponentLocation()-Pt_0->GetComponentLocation();
			//对偏移值进行垂直偏移
			ToPoint=ToPoint.RotateAngleAxis(YawOverride,FVector::UpVector);
			//设置偏移后的坐标
			Pt->SetWorldLocation(Pt_0->GetComponentLocation()+ToPoint);
		}
		//创建拾取坐标使用的起始点和最终位置
		const FVector RaisedLocation=FVector(Pt->GetComponentLocation().X,Pt->GetComponentLocation().Y,Pt->GetComponentLocation().Z+500.f);
		const FVector LowerLocation=FVector(Pt->GetComponentLocation().X,Pt->GetComponentLocation().Y,Pt->GetComponentLocation().Z-500.f);

		TArray<AActor*> IgnoreActors;
		UAuraAbilitySystemLibrary::GetLivePlayerWithinRadius(this,IgnoreActors,TArray<AActor*>(),1500.f,GetActorLocation());
		FHitResult Hit;
		FCollisionQueryParams CollisionParams;
		GetWorld()->LineTraceSingleByProfile(Hit,RaisedLocation,LowerLocation,FName("BlockAll"),CollisionParams);

		//通过结果修改节点的位置和朝向
		const FVector AdjustedLocation=FVector(Pt->GetComponentLocation().X,Pt->GetComponentLocation().Y,Hit.ImpactPoint.Z);
		Pt->SetWorldLocation(AdjustedLocation);
		Pt->SetWorldRotation(UKismetMathLibrary::MakeRotFromZ(Hit.ImpactPoint));

		ArrayCopy.Add(Pt);
	}
	return ArrayCopy;
}*/
TArray<USceneComponent*> APointCollection::GetGroundPoints(const FVector& GroundLocation, int32 NumPoints, float YawOverride)
{
	checkf(ImmutablePts.Num() >= NumPoints, TEXT("访问索引超过了数组的范围"));

	TArray<USceneComponent*> ArrayCopy;

	for(USceneComponent* Pt : ImmutablePts)
	{
		if(ArrayCopy.Num() >= NumPoints) return ArrayCopy;

		if(Pt != Pt_0)
		{
			FVector ToPoint = Pt->GetComponentLocation() - Pt_0->GetComponentLocation(); //获取到节点基于根组件世界坐标系下的偏移
			ToPoint = ToPoint.RotateAngleAxis(YawOverride, FVector::UpVector); //对偏移值进行垂直偏移
			Pt->SetWorldLocation(Pt_0->GetComponentLocation() + ToPoint); //设置偏移后的坐标
		}

		//创建拾取坐标使用的起始点和最终位置
		const FVector RaisedLocation = FVector(Pt->GetComponentLocation().X, Pt->GetComponentLocation().Y, Pt->GetComponentLocation().Z + 500.f);
		const FVector LoweredLocation = FVector(Pt->GetComponentLocation().X, Pt->GetComponentLocation().Y, Pt->GetComponentLocation().Z - 500.f);

		//获取到周围过滤的对象
		TArray<AActor*> IgnoreActors;
		UAuraAbilitySystemLibrary::GetLivePlayerWithinRadius(this, IgnoreActors, TArray<AActor*>(), 1500.f, GetActorLocation());

		//创建一条直线来拾取地面的坐标
		FHitResult HitResult;
		FCollisionQueryParams QueryParams;
		QueryParams.AddIgnoredActors(IgnoreActors);
		GetWorld()->LineTraceSingleByProfile(HitResult, RaisedLocation, LoweredLocation, FName("BlockAll"), QueryParams);

		//通过结果修改节点的位置和朝向
		const FVector AdjustedLocation = FVector(Pt->GetComponentLocation().X, Pt->GetComponentLocation().Y, HitResult.ImpactPoint.Z);
		Pt->SetWorldLocation(AdjustedLocation);
		Pt->SetWorldRotation(UKismetMathLibrary::MakeRotFromZ(HitResult.ImpactNormal));

		//添加到返回数组内
		ArrayCopy.Add(Pt);
	}

	return ArrayCopy;
}


// Called when the game starts or when spawned
void APointCollection::BeginPlay()
{
	Super::BeginPlay();
	
}

void APointCollection::CreateSceneComponent(TObjectPtr<USceneComponent>& Pt, FName Name)
{
	Pt=CreateDefaultSubobject<USceneComponent>(Name);
	Pt->SetupAttachment(GetRootComponent());
	ImmutablePts.Add(Pt);
}



