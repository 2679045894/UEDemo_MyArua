// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilitys/AuraSummonAbility.h"

#include "Kismet/KismetSystemLibrary.h"

TArray<FVector> UAuraSummonAbility::GetSpawnLocations()
{
	const FVector Forward=GetAvatarActorFromActorInfo()->GetActorForwardVector();
	const FVector Location=GetAvatarActorFromActorInfo()->GetActorLocation();
	//将召唤的角度范围进行分段，在每一段里面生成一个召唤物
	const float DeltaSpread=SpawnSpread/(NumMinions-1);

	//以Forward方向为基准，绕Z轴(UpVector)旋转-SpawnSpread/2角度
	// 得到扇形左边界方向
	const FVector LeftOfSpread=Forward.RotateAngleAxis(-SpawnSpread/2.f,FVector::UpVector);
	UKismetSystemLibrary::DrawDebugArrow(GetAvatarActorFromActorInfo(),Location,Location+LeftOfSpread*MaxSpawnDistance,4.f,FLinearColor::Red,3.f);
	// 旋转+SpawnSpread/2角度，得到扇形右边界方向  
	const FVector RightOfSpread=Forward.RotateAngleAxis(SpawnSpread/2.f,FVector::UpVector);
	UKismetSystemLibrary::DrawDebugArrow(GetAvatarActorFromActorInfo(),Location,Location+RightOfSpread*MaxSpawnDistance,4.f,FLinearColor::Red,3.f);
	DrawDebugSphere(GetWorld(),Location+LeftOfSpread*MinSpawnDistance,15.f,12,FColor::Blue,false,3.0f);
	DrawDebugSphere(GetWorld(),Location+LeftOfSpread*MaxSpawnDistance,15.f,12,FColor::Blue,false,3.0f);
	DrawDebugSphere(GetWorld(),Location+RightOfSpread*MinSpawnDistance,15.f,12,FColor::Blue,false,3.0f);
	DrawDebugSphere(GetWorld(),Location+RightOfSpread*MaxSpawnDistance,15.f,12,FColor::Blue,false,3.0f);
	TArray<FVector> SpawnLocations;
	for (int32 i=0;i<NumMinions;i++)
	{
		const FVector Direction=LeftOfSpread.RotateAngleAxis(i*DeltaSpread,FVector::UpVector);
		FVector ChosenSpawnLocation=Location+Direction*FMath::FRandRange(MinSpawnDistance,MaxSpawnDistance);
		//射线检测，防止浮空
		FHitResult HitResult;
		GetWorld()->LineTraceSingleByChannel(HitResult,ChosenSpawnLocation+FVector(0,0,400.f),ChosenSpawnLocation+FVector(0,0,-400.f),ECollisionChannel::ECC_Visibility);
		if (HitResult.bBlockingHit)
		{
			ChosenSpawnLocation=HitResult.Location;
		}
		DrawDebugSphere(GetWorld(),ChosenSpawnLocation,15.f,12,FColor::Blue,false,3.0f);
		SpawnLocations.Add(ChosenSpawnLocation);
	}
	return SpawnLocations;
}

TSubclassOf<APawn> UAuraSummonAbility::GetRandomMinionClass()
{
	int32 Index=FMath::RandRange(0,NumMinions-1);
	return MinionClasses[Index];
}
