// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/AuraAbilitySystemLibrary.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AuraAbilityTypes.h"
#include "AuraGameplayTags.h"
#include "Game/MyGameModeBase.h"
#include "kismet/GameplayStatics.h"
#include "Player/MyPlayerState.h"
#include "Character/AuraCharacter.h"
#include "UI/HUD/AuraHUD.h"

bool UAuraAbilitySystemLibrary::MakeWidgetControllerParams(const UObject* WorldObject,
	FWidgetControllerParams& OutWCParams,AAuraHUD*& OutAuraHUD)
{
	if (APlayerController* PC=UGameplayStatics::GetPlayerController(WorldObject,0))
	{
		OutAuraHUD=Cast<AAuraHUD>(PC->GetHUD());
		if (OutAuraHUD)
		{
			AMyPlayerState* PS=Cast<AMyPlayerState>(PC->GetPlayerState<AMyPlayerState>());
			UAuraAbilitySystemComponent* ASC=Cast<UAuraAbilitySystemComponent>(PS->GetAbilitySystemComponent());
			UAuraAttributeSet* AS=Cast<UAuraAttributeSet>(PS->GetAttributeSet());
			OutWCParams.PlayerController=PC;
			OutWCParams.PlayerState=PS;
			OutWCParams.AbilitySystemComponent=ASC;
			OutWCParams.AttributeSet=AS;
			return  true;
		}
	}
	return false;
}

UOverplayWidgetController* UAuraAbilitySystemLibrary::GetOverlayWidgetController(const UObject* WorldContextObject)
{
	FWidgetControllerParams WCParams;
	AAuraHUD* AuraHUD=nullptr;
	if (MakeWidgetControllerParams(WorldContextObject,WCParams,AuraHUD))
	{
		return AuraHUD->GetOverPlayWidgetController(WCParams);
	}
	return nullptr;
}

UAttributeMenuWidgetController* UAuraAbilitySystemLibrary::GetAttributeMenuController(const UObject* WorldContextObject)
{
	FWidgetControllerParams WCParams;
	AAuraHUD* AuraHUD=nullptr;
	if (MakeWidgetControllerParams(WorldContextObject,WCParams,AuraHUD))
	{
		return AuraHUD->GetAttributeMenuWidgetController(WCParams);
	}
	return nullptr;
}

USpellMenuWidgetController* UAuraAbilitySystemLibrary::GetSpellMenuController(const UObject* WorldContextObject)
{
	FWidgetControllerParams WCParams;
	AAuraHUD* AuraHUD=nullptr;
	if (MakeWidgetControllerParams(WorldContextObject,WCParams,AuraHUD))
	{
		return AuraHUD->GetSpellMenuWidgetController(WCParams);
	}
	return nullptr;
}

void UAuraAbilitySystemLibrary::InitializeDefaultAttributes(const UObject* WorldContextObject, ECharacterClass CharacterClass, float Level, UAbilitySystemComponent* ASC)
{
	AMyGameModeBase* AuraGameMode = Cast<AMyGameModeBase>(UGameplayStatics::GetGameMode(WorldContextObject));
	if (AuraGameMode == nullptr) return;

	//初始化对象(用于context)
	AActor* AvatarActor = ASC->GetAvatarActor();

	UCharacterClassInfo* CharacterClassInfo = AuraGameMode->CharacterClassInfo;
	FCharacterClassDefault ClassDefaultInfo = CharacterClassInfo->GetCharacterClassDefaultInfo(CharacterClass);

	FGameplayEffectContextHandle PrimaryAttributesContextHandle = ASC->MakeEffectContext();
	PrimaryAttributesContextHandle.AddSourceObject(AvatarActor);
	const FGameplayEffectSpecHandle PrimaryAttributesSpecHandle = ASC->MakeOutgoingSpec(
		ClassDefaultInfo.PrimaryAttributeClass, Level, PrimaryAttributesContextHandle);
	ASC->ApplyGameplayEffectSpecToSelf(*PrimaryAttributesSpecHandle.Data.Get());

	FGameplayEffectContextHandle SecondaryAttributesContextHandle = ASC->MakeEffectContext();
	SecondaryAttributesContextHandle.AddSourceObject(AvatarActor);
	const FGameplayEffectSpecHandle SecondaryAttributesSpecHandle = ASC->MakeOutgoingSpec(CharacterClassInfo->SecondaryAttributeClass, Level, SecondaryAttributesContextHandle);
	ASC->ApplyGameplayEffectSpecToSelf(*SecondaryAttributesSpecHandle.Data.Get());

	FGameplayEffectContextHandle VitalAttributesContextHandle = ASC->MakeEffectContext();
	VitalAttributesContextHandle.AddSourceObject(AvatarActor);
	const FGameplayEffectSpecHandle VitalAttributesSpecHandle = ASC->MakeOutgoingSpec(CharacterClassInfo->VitalAttributeClass, Level, VitalAttributesContextHandle);
	ASC->ApplyGameplayEffectSpecToSelf(*VitalAttributesSpecHandle.Data.Get());
	
}

void UAuraAbilitySystemLibrary::GiveStartupAbilities(const UObject* WorldContext, UAbilitySystemComponent* ASC,ECharacterClass CharacterClass)
{
	UCharacterClassInfo* CharacterClassInfo=GetCharacterClassInfo(WorldContext);
	if (CharacterClassInfo==nullptr) return;
	AMyGameModeBase* AuraGameMode=Cast<AMyGameModeBase>(UGameplayStatics::GetGameMode(WorldContext));
	if (!AuraGameMode) return;
	for (TSubclassOf<UGameplayAbility> AbilityClass:GetCharacterClassInfo(WorldContext)->CommonAbilitiesClass)
	{
		//定义所有职业共有的基础能力
		FGameplayAbilitySpec AbilitySpec=FGameplayAbilitySpec(AbilityClass,1);
		ASC->GiveAbility(AbilitySpec);
	}
	for (TSubclassOf<UGameplayAbility> AbilityClass:CharacterClassInfo->GetCharacterClassDefaultInfo(CharacterClass).StartupAbilitiesClass)
	{
		if (ICombatInterface* CombatInterface=Cast<ICombatInterface>(ASC->GetAvatarActor()))
		{
			FGameplayAbilitySpec AbilitySpec=FGameplayAbilitySpec(AbilityClass,CombatInterface->Execute_GetPlayerLevel(ASC->GetAvatarActor()));
			ASC->GiveAbility(AbilitySpec);
		}
	}
}

UCharacterClassInfo* UAuraAbilitySystemLibrary::GetCharacterClassInfo(const UObject* WorldContext)
{
	//通过访问MyGameMode中的CharacterClassInfo
	if (AMyGameModeBase* AuraGameMode=Cast<AMyGameModeBase>(UGameplayStatics::GetGameMode(WorldContext)))
	{
		return AuraGameMode->CharacterClassInfo;
	}
	return nullptr;
}

UAbilityInfo* UAuraAbilitySystemLibrary::GetActiveAbilityInfo(const UObject* WorldContext)
{
	if (AMyGameModeBase* AuraGameMode=Cast<AMyGameModeBase>(UGameplayStatics::GetGameMode(WorldContext)))
	{
		return AuraGameMode->AbilityInfo;
	}
	return nullptr;
}

bool UAuraAbilitySystemLibrary::IsBlockedHit(const FGameplayEffectContextHandle& ContextHandle)
{
	if (const FAuraGameplayContext* AuraContext=static_cast<const FAuraGameplayContext*>(ContextHandle.Get()))
	{
		return AuraContext->IsBlockedHit();
	}
	return false;
}

bool UAuraAbilitySystemLibrary::IsCriticalHit(const FGameplayEffectContextHandle& ContextHandle)
{
	if (const FAuraGameplayContext* AuraContext=static_cast<const FAuraGameplayContext*>(ContextHandle.Get()))
	{
		return AuraContext->IsCriticalHit();
	}
	return false;
}

bool UAuraAbilitySystemLibrary::IsRadiaDamage(const FGameplayEffectContextHandle& ContextHandle)
{
	if (const FAuraGameplayContext* AuraGameplayContext=static_cast<const FAuraGameplayContext*>(ContextHandle.Get()))
	{
		return AuraGameplayContext->IsRadiaDamage();
	}
	return false;
}

bool UAuraAbilitySystemLibrary::IsSuccessfulDeBuff(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if(const FAuraGameplayContext* RPGEffectContext = static_cast<const FAuraGameplayContext*>(EffectContextHandle.Get()))
	{
		return RPGEffectContext->IsSuccessfulDeBuff();
	}
	return false;
}

float UAuraAbilitySystemLibrary::GetDeBuffDamage(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if(const FAuraGameplayContext* RPGEffectContext = static_cast<const FAuraGameplayContext*>(EffectContextHandle.Get()))
	{
		return RPGEffectContext->GetDeBuffDamage();
	}
	return 0.f;
}

float UAuraAbilitySystemLibrary::GetDeBuffDuration(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if(const FAuraGameplayContext* RPGEffectContext = static_cast<const FAuraGameplayContext*>(EffectContextHandle.Get()))
	{
		return RPGEffectContext->GetDeBuffDuration();
	}
	return 0.f;
}

float UAuraAbilitySystemLibrary::GetDeBuffFrequency(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if(const FAuraGameplayContext* RPGEffectContext = static_cast<const FAuraGameplayContext*>(EffectContextHandle.Get()))
	{
		return RPGEffectContext->GetDeBuffFrequency();
	}
	return 0.f;
}

FGameplayTag UAuraAbilitySystemLibrary::GetDeBuffDamageType(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if(const FAuraGameplayContext* RPGEffectContext = static_cast<const FAuraGameplayContext*>(EffectContextHandle.Get()))
	{
		//如果当前存在设置了伤害类型
		if(RPGEffectContext->GetDeBuffDamageTypeTag())
		{
			//解引用
			return *RPGEffectContext->GetDeBuffDamageTypeTag();
		}
	}
	return FGameplayTag();
}

FVector UAuraAbilitySystemLibrary::GetDeathImpulse(FGameplayEffectContextHandle& EffectContextHandle)
{
	if (FAuraGameplayContext* AuraContext=static_cast<FAuraGameplayContext*>(EffectContextHandle.Get()))
	{
		return AuraContext->GetDeathImpulse();
	}
	return FVector::ZeroVector;
}

FVector UAuraAbilitySystemLibrary::GetKnockbackForce(FGameplayEffectContextHandle& EffectContextHandle)
{
	if (FAuraGameplayContext* AuraGameplayContext=static_cast<FAuraGameplayContext*>(EffectContextHandle.Get()))
	{
		return AuraGameplayContext->GetKnockForce();
	}
	return FVector::ZeroVector;
}

float UAuraAbilitySystemLibrary::GetRadiaDamageInnerRadius(FGameplayEffectContextHandle& EffectContextHandle)
{
	if (FAuraGameplayContext* AuraGameplayContext=static_cast<FAuraGameplayContext*>(EffectContextHandle.Get()))
	{
		return AuraGameplayContext->GetRadiaDamageInnerRadius();
	}
	return 0.f;
}

float UAuraAbilitySystemLibrary::GetRadiaDamageOuterRadius(FGameplayEffectContextHandle& EffectContextHandle)
{
	if (FAuraGameplayContext* AuraGameplayContext=static_cast<FAuraGameplayContext*>(EffectContextHandle.Get()))
	{
		return AuraGameplayContext->GetRadiaDamageOuterRadius();
	}
	return 0.f;
}

FVector UAuraAbilitySystemLibrary::GetRadiaDamageOrigin(FGameplayEffectContextHandle& EffectContextHandle)
{
	if (FAuraGameplayContext* AuraGameplayContext=static_cast<FAuraGameplayContext*>(EffectContextHandle.Get()))
	{
		return AuraGameplayContext->GetRadiaDamageOrigin();
	}
	return FVector::ZeroVector;
}


void UAuraAbilitySystemLibrary::SetIsBlockedHit(FGameplayEffectContextHandle& ContextHandle, bool bInIsBlockedHit)
{
	if (FAuraGameplayContext* AuraContext=static_cast<FAuraGameplayContext*>(ContextHandle.Get()))
	{
		AuraContext->SetIsBlockedHit(bInIsBlockedHit);
	}
}

void UAuraAbilitySystemLibrary::SetIsCriticalHit(FGameplayEffectContextHandle& ContextHandle, bool bInIsCriticalHit)
{
	if (FAuraGameplayContext* AuraContext=static_cast<FAuraGameplayContext*>(ContextHandle.Get()))
	{
		AuraContext->SetIsCriticalHit(bInIsCriticalHit);
	}
}

void UAuraAbilitySystemLibrary::SetIsSuccessfulDeBuff(FGameplayEffectContextHandle& ContextHandle,
	bool bInIsSuccessfulDeBuff)
{
	if (FAuraGameplayContext* AuraContext=static_cast<FAuraGameplayContext*>(ContextHandle.Get()))
	{
		AuraContext->SetIsSuccessfulDeBuff(bInIsSuccessfulDeBuff);
	}
}

void UAuraAbilitySystemLibrary::SetDeBuff(FGameplayEffectContextHandle& ContextHandle, FGameplayTag& InDamageType,
	float InDamage, float InDuration, float InFrequency)
{
	if (FAuraGameplayContext* AuraGameplayContext=static_cast<FAuraGameplayContext*>(ContextHandle.Get()))
	{
		TSharedPtr<FGameplayTag> DamageTypeTag=MakeShared<FGameplayTag>(InDamageType);
		AuraGameplayContext->SetDeBuffDamageType(DamageTypeTag);
		AuraGameplayContext->SetDeBuffDamage(InDamage);
		AuraGameplayContext->SetDeBuffDuration(InDuration);
		AuraGameplayContext->SetDeBuffFrequency(InFrequency);
	}
}

void UAuraAbilitySystemLibrary::SetDeathImpulse(FGameplayEffectContextHandle& ContextHandle,
	FVector InDeathImpulse)
{
	if (FAuraGameplayContext* AuraContext=static_cast<FAuraGameplayContext*>(ContextHandle.Get()))
	{
		AuraContext->SetDeathImpulse(InDeathImpulse);
	}
}

void UAuraAbilitySystemLibrary::SetKnockbackForce(FGameplayEffectContextHandle& ContextHandle, FVector InKnockbackForce)
{
	if (FAuraGameplayContext* AuraContext=static_cast<FAuraGameplayContext*>(ContextHandle.Get()))
	{
		AuraContext->SetKnockForce(InKnockbackForce);
	}
}

void UAuraAbilitySystemLibrary::SetIsRadiaDamage(FGameplayEffectContextHandle& ContextHandle, bool InIsRadiaDamage)
{
	if (FAuraGameplayContext* AuraContext=static_cast<FAuraGameplayContext*>(ContextHandle.Get()))
	{
		AuraContext->SetIsRadiaDamage(InIsRadiaDamage);
	}
}

void UAuraAbilitySystemLibrary::SetRadiaDamageInnerRadius(FGameplayEffectContextHandle& ContextHandle,
	float InRadiaDamageInnerRadius)
{
	if (FAuraGameplayContext* AuraContext=static_cast<FAuraGameplayContext*>(ContextHandle.Get()))
	{
		AuraContext->SetRadiaDamageInnerRadius(InRadiaDamageInnerRadius);
	}
}

void UAuraAbilitySystemLibrary::SetRadiaDamageOuterRadius(FGameplayEffectContextHandle& ContextHandle,
	float InRadiaDamageOuterRadius)
{
	if (FAuraGameplayContext* AuraContext=static_cast<FAuraGameplayContext*>(ContextHandle.Get()))
	{
		AuraContext->SetRadiaDamageInnerRadius(InRadiaDamageOuterRadius);
	}
}

void UAuraAbilitySystemLibrary::SetRadiaDamageOrigin(FGameplayEffectContextHandle& ContextHandle,
	FVector InRadiaDamageOrigin)
{
	if (FAuraGameplayContext* AuraContext=static_cast<FAuraGameplayContext*>(ContextHandle.Get()))
	{
		AuraContext->SetRadiaDamageOrigin(InRadiaDamageOrigin);
	}
}

void UAuraAbilitySystemLibrary::GetLivePlayerWithinRadius(const UObject* WorldContextObject,
                                                          TArray<AActor*>& OutOverlappingActors, const TArray<AActor*>& ActorsToIgnore, float Radius,
                                                          const FVector& SphereOrigin)
{
	//创建一个碰撞查询配置
	FCollisionQueryParams SphereParams;
	//添加忽略对象
	SphereParams.AddIgnoredActors(ActorsToIgnore);
	TArray<FOverlapResult> OverlapResults;
	//LogAndReturnNull：当无法获取有效的 World 时，不会让游戏崩溃，而是优雅地返回 nullptr。
	if (UWorld *World=GEngine->GetWorldFromContextObject(WorldContextObject,EGetWorldErrorMode::LogAndReturnNull))
	{
		//执行球形范围检测
		World->OverlapMultiByObjectType(OverlapResults,//输出：存储检测结果
			SphereOrigin,
			FQuat::Identity,// 输入：旋转（无旋转）
			FCollisionObjectQueryParams::InitType::AllDynamicObjects,// 输入：检测对象类型
			FCollisionShape::MakeSphere(Radius),// 输入：碰撞形状
			SphereParams);//输入：额外参数
	}
	for (FOverlapResult& OverlapResult:OverlapResults)
	{
		//将检测到的未死亡的敌人或玩家添加到数组中
		const bool CombatInterface=OverlapResult.GetActor()->Implements<UCombatInterface>();
		if (CombatInterface&&!ICombatInterface::Execute_IsDead(OverlapResult.GetActor()))
		{
			//AddUnique：如果当前数组中不存在这个对象，才进行add
			OutOverlappingActors.AddUnique(OverlapResult.GetActor());
		}
	}

	
}

bool UAuraAbilitySystemLibrary::IsNotFriend(AActor* FirstActor, AActor* SecondActor)
{
	if (FirstActor->ActorHasTag(FName("Player")))
	{
		return !SecondActor->ActorHasTag(FName("Player"));
	}
	if (FirstActor->ActorHasTag(FName("Enemy")))
	{
		return !SecondActor->ActorHasTag(FName("Enemy"));
	}
	return false;
}

int32 UAuraAbilitySystemLibrary::GetXPRewardForClassAndLevel(const UObject* WorldContextObject,
	ECharacterClass CharacterClass, int32 CharacterLevel)
{
	if (UCharacterClassInfo* CharacterClassInfo=GetCharacterClassInfo(WorldContextObject))
	{
		FCharacterClassDefault CharacterClassDefault=CharacterClassInfo->GetCharacterClassDefaultInfo(CharacterClass);
		const float XPReward=CharacterClassDefault.XPReward.GetValueAtLevel(CharacterLevel);
		return static_cast<int32>(XPReward);
	}
	return 0;
}

FGameplayEffectContextHandle UAuraAbilitySystemLibrary::ApplyDamageEffect(const FDamageEffectParams& DamageEffectParams)
{
	FAuraGameplayTags GameplayTags=FAuraGameplayTags::Get();
	AActor* SourceAvatar=DamageEffectParams.SourceASC->GetAvatarActor();
	//创建GE的上下文句柄
	FGameplayEffectContextHandle ContextHandle=DamageEffectParams.SourceASC->MakeEffectContext();
	ContextHandle.AddSourceObject(SourceAvatar);
	
	//设置击退相关
	SetDeathImpulse(ContextHandle,DamageEffectParams.DeathImpulse);
	SetKnockbackForce(ContextHandle,DamageEffectParams.KnockbackForce);

	//设置范围相关
	SetIsRadiaDamage(ContextHandle,DamageEffectParams.bIsRadiaDamage);
	SetRadiaDamageInnerRadius(ContextHandle,DamageEffectParams.RadiaDamageInnerRadius);
	SetRadiaDamageOuterRadius(ContextHandle,DamageEffectParams.RadiaDamageOuterRadius);
	SetRadiaDamageOrigin(ContextHandle,DamageEffectParams.RadiaDamageOrigin);
	
	//根据句柄和类创建GE实例
	FGameplayEffectSpecHandle SpecHandle=DamageEffectParams.SourceASC->MakeOutgoingSpec(
		DamageEffectParams.DamageGameplayEffectClass,DamageEffectParams.AbilityLevel,ContextHandle);
	
	//通过标签设置GE使用的配置
	for (auto& Pair:DamageEffectParams.DamageTypes)
	{
		UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle,Pair.Key,Pair.Value);
	}
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle,GameplayTags.DeBuff_Chance,DamageEffectParams.DeBuffChance);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle,DamageEffectParams.DeBuffDamageType,DamageEffectParams.DeBuffDamage);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle,GameplayTags.DeBuff_Duration,DamageEffectParams.DeBuffDuration);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle,GameplayTags.DeBuff_Frequency,DamageEffectParams.DeBuffFrequency);
	//将GE应用给目标ASC
	//DamageEffectParams.TargetASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
	if (SpecHandle.IsValid())
	{
		if (FGameplayEffectSpec* Spec = SpecHandle.Data.Get())
		{
			if (IsValid(DamageEffectParams.TargetASC))
			{
				DamageEffectParams.TargetASC->ApplyGameplayEffectSpecToSelf(*Spec);
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("ApplyDamageEffect: TargetASC is invalid!"));
			}
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("ApplyDamageEffect: Spec is null!"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("ApplyDamageEffect: SpecHandle is invalid!"));
	}
	return ContextHandle;
}

/**
 * 
 * @param Forward 施术者的向前向量
 * @param Axis 基准轴
 * @param Spread 总角度
 * @param NumRotators 均分数量
 * @return 
 */
TArray<FRotator> UAuraAbilitySystemLibrary::EvenlySpacedRotators(const FVector& Forward, const FVector& Axis,
                                                                 float Spread, int32 NumRotators)
{
	TArray<FRotator> Rotators;
	const FVector LeftOfSpread=Forward.RotateAngleAxis(-Spread/2.f,Axis);
	if (NumRotators>1)
	{
		const float DeltaSpread = Spread / NumRotators;
		for (int32 i=0; i<NumRotators; i++)
		{
			const FVector Direction=LeftOfSpread.RotateAngleAxis(DeltaSpread*(i+.5f),Axis);
			Rotators.Add(Direction.Rotation());
		}
	}
	else
	{
		Rotators.Add(Forward.Rotation());
	}
	return Rotators;
}

TArray<FVector> UAuraAbilitySystemLibrary::EvenlySpacedVectors(const FVector& Forward, const FVector& Axis,
	float Spread, int32 NumVectors)
{
	TArray<FVector> Vectors;
	const FVector LeftOfSpread=Forward.RotateAngleAxis(-Spread/2,Axis);
	if (NumVectors>1)
	{
		const float DeltaSpread=Spread/NumVectors;
		for (int32 i=0; i<NumVectors; i++)
		{
			const FVector Direction=LeftOfSpread.RotateAngleAxis(DeltaSpread*(i+.5f),Axis);
			Vectors.Add(Direction);
		}
	}
	else
	{
		Vectors.Add(Forward);
	}
	return Vectors;
}

void UAuraAbilitySystemLibrary::GetClosestTargets(TArray<AActor*>& Actors, TArray<AActor*>& OutActors, int32 MaxTargets,
	FVector& Origin)
{
	if (Actors.Num()<MaxTargets)
	{
		OutActors=Actors;
		return;
	}
	TArray<AActor*> ActorsToCheck=Actors;
	int32 TempCount=0;
	
	while (TempCount<MaxTargets)
	{
		if (ActorsToCheck.Num()==0)break;
		double ClosestTargetDistance=TNumericLimits<double>::Max();
		AActor* ClosestTarget=nullptr;
		for (AActor* Actor:ActorsToCheck)
		{
			double Distance=(Actor->GetActorLocation()-Origin).Length();
			if (Distance<ClosestTargetDistance)
			{
				ClosestTargetDistance=Distance;
				ClosestTarget=Actor;
			}
		}
		ActorsToCheck.Remove(ClosestTarget);
		OutActors.AddUnique(ClosestTarget);
		++TempCount;
	}
}

void UAuraAbilitySystemLibrary::SetIsRadiaDamageEffectParams(FDamageEffectParams& DamageEffectParams,
	bool bIsRadiaDamage, float InRadiaDamageInnerRadius, float InRadiaDamageOuterRadius,const FVector& InOrigin)
{
	DamageEffectParams.bIsRadiaDamage=bIsRadiaDamage;
	DamageEffectParams.RadiaDamageInnerRadius=InRadiaDamageInnerRadius;
	DamageEffectParams.RadiaDamageOuterRadius=InRadiaDamageOuterRadius;
	DamageEffectParams.RadiaDamageOrigin=InOrigin;
}

void UAuraAbilitySystemLibrary::SetKnockbackDirection(FDamageEffectParams& DamageEffectParams,
	FVector KnockbackDirection, float Magnitude)
{
	KnockbackDirection.Normalize();
	if (Magnitude==0)
	{
		DamageEffectParams.KnockbackForce=KnockbackDirection*DamageEffectParams.KnockbackForceMagnitude;
	}
	else
	{
		DamageEffectParams.KnockbackForce=KnockbackDirection*Magnitude;
	}
}

void UAuraAbilitySystemLibrary::SetDeathImpulseDirection(FDamageEffectParams& DamageEffectParams,
	FVector ImpulseDirection, float Magnitude)
{
	ImpulseDirection.Normalize();
	if (Magnitude==0)
	{
		DamageEffectParams.DeathImpulse=ImpulseDirection*DamageEffectParams.DeathImpulseMagnitude;
	}
	else
	{
		DamageEffectParams.DeathImpulse=ImpulseDirection*Magnitude;
	}
}

void UAuraAbilitySystemLibrary::SetEffectParamsTargetASC(FDamageEffectParams& DamageEffectParams,
	UAbilitySystemComponent* InASC)
{
	DamageEffectParams.TargetASC=InASC;
}

void UAuraAbilitySystemLibrary::InitializeDefaultAttributesFromSaveData(const UObject* WorldContextObject,
	UAbilitySystemComponent* InASC, ULoadScreenSaveGame* InSaveObject)
{
	UCharacterClassInfo* CharacterClassInfo=GetCharacterClassInfo(WorldContextObject);
	FAuraGameplayTags AuraGameplayTags=FAuraGameplayTags::Get();
	AActor* Avatar=InASC->GetAvatarActor();
	//应用基础属性
	FGameplayEffectContextHandle ContextHandle=InASC->MakeEffectContext();
	ContextHandle.AddSourceObject(Avatar);
	FGameplayEffectSpecHandle PrimaryEffectSpecHandle=InASC->MakeOutgoingSpec(CharacterClassInfo->PrimaryAttributes_SetByCaller,1,ContextHandle);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(PrimaryEffectSpecHandle,AuraGameplayTags.Attributes_Primary_Strength,InSaveObject->Strength);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(PrimaryEffectSpecHandle,AuraGameplayTags.Attributes_Primary_Intelligence,InSaveObject->Intelligence);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(PrimaryEffectSpecHandle,AuraGameplayTags.Attributes_Primary_Resilience,InSaveObject->Resilience);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(PrimaryEffectSpecHandle,AuraGameplayTags.Attributes_Primary_Vigor,InSaveObject->Vigor);
	InASC->ApplyGameplayEffectSpecToSelf(*PrimaryEffectSpecHandle.Data.Get());
	
	if (Avatar->Implements<UPlayerInterface>())
	{
		//应用次级属性
		FGameplayEffectContextHandle SecondaryContextHandle=InASC->MakeEffectContext();
		SecondaryContextHandle.AddSourceObject(Avatar);
		FGameplayEffectSpecHandle SecondaryEffectSpecHandle=InASC->MakeOutgoingSpec(IPlayerInterface::Execute_GetSecondaryAttributes(Avatar),1,SecondaryContextHandle);
		InASC->ApplyGameplayEffectSpecToSelf(*SecondaryEffectSpecHandle.Data.Get());
		//应用关键属性
		FGameplayEffectContextHandle VitalContextHandle=InASC->MakeEffectContext();
		VitalContextHandle.AddSourceObject(Avatar);
		FGameplayEffectSpecHandle VitalEffectSpecHandle=InASC->MakeOutgoingSpec(IPlayerInterface::Execute_GetVitalAttributes(Avatar),1,VitalContextHandle);
		InASC->ApplyGameplayEffectSpecToSelf(*VitalEffectSpecHandle.Data.Get());
	}
}

	




