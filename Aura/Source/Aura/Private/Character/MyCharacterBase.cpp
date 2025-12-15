// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/MyCharacterBase.h"

#include "AuraGameplayTags.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Net/UnrealNetwork.h"
#include "Player/MyPlayerState.h"

// Sets default values
AMyCharacterBase::AMyCharacterBase()
{
	PrimaryActorTick.bCanEverTick = false;
	Weapon=CreateDefaultSubobject<USkeletalMeshComponent>("Weapon");
	Weapon->SetupAttachment(GetMesh(),FName("WeaponHandSocket"));
	Weapon->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	BurnDeBuffComponent=CreateDefaultSubobject<UDeBuffNiagaraComponent>("BurnDeBuffComponent");
	BurnDeBuffComponent->SetupAttachment(GetRootComponent());
	BurnDeBuffComponent->DeBuffTag=FAuraGameplayTags::Get().DeBuff_Burn;

	StunDeBuffComponent=CreateDefaultSubobject<UDeBuffNiagaraComponent>("StunDeBuffComponent");
	StunDeBuffComponent->SetupAttachment(GetRootComponent());
	StunDeBuffComponent->DeBuffTag=FAuraGameplayTags::Get().DeBuff_Stun;
}

// Called when the game starts or when spawned
void AMyCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	bIsDead=false;
}

// Called every frame
void AMyCharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AMyCharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

UAuraAbilitySystemComponent* AMyCharacterBase::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void AMyCharacterBase::InitialAbilityActorInfo()
{
	
}

void AMyCharacterBase::InitializeDefaultAttributes() const
{
	ApplyEffectToSelf(DefaultPrimaryAttribute,1.f);
	ApplyEffectToSelf(DefaultSecondaryAttributes,1.f);
	ApplyEffectToSelf(DefaultVitalAttributes,1.f);
	ApplyEffectToSelf(Test,1.f);
}

void AMyCharacterBase::ApplyEffectToSelf(TSubclassOf<UGameplayEffect> GameplayEffectClass, float Level) const
{
	if (UAuraAbilitySystemComponent* TargetASC=Cast<UAuraAbilitySystemComponent>(GetAbilitySystemComponent()))
	{
		FGameplayEffectContextHandle EffectContextHandle=TargetASC->MakeEffectContext();
		EffectContextHandle.AddSourceObject(this);
		FGameplayEffectSpecHandle EffectSpec=TargetASC->MakeOutgoingSpec(GameplayEffectClass,Level,EffectContextHandle);
		UAuraAttributeSet* AuraAS=Cast<UAuraAttributeSet>(AttributeSet);
		if (EffectSpec.IsValid())
		{
			TargetASC->ApplyGameplayEffectSpecToSelf(*EffectSpec.Data.Get());
		}
	}
}

int32 AMyCharacterBase::GetPlayerLevel_Implementation()
{
	return ICombatInterface::GetPlayerLevel_Implementation();
}


void AMyCharacterBase::AddCharacterAbilities() const
{
	UAuraAbilitySystemComponent* AuraASC=GetAbilitySystemComponent();
	if (!HasAuthority())return;
	AuraASC->AddCharacterAbilities(StartupAbilities);
	AuraASC->AddCharacterPassiveAbilities(StartupPassiveAbilities);
}


FVector AMyCharacterBase::GetCombatSocketLocation_Implementation(const FGameplayTag &MontageTag)
{
	if (MontageTag.MatchesTagExact(FAuraGameplayTags::Get().CombatSocket_Weapon))
	{
		return Weapon->GetSocketLocation(WeaponTioSocketName);
	}
	if (MontageTag.MatchesTagExact(FAuraGameplayTags::Get().CombatSocket_LeftHand))
	{
		return GetMesh()->GetSocketLocation(LeftHandSocketName);
	}
	if (MontageTag.MatchesTagExact(FAuraGameplayTags::Get().CombatSocket_RightHand))
	{
		return GetMesh()->GetSocketLocation(RightHandSocketName);
	}
	if (MontageTag.MatchesTagExact(FAuraGameplayTags::Get().CombatSocket_Tail))
	{
		return GetMesh()->GetSocketLocation(TailSocketName);
	}
	return FVector();
}

TArray<FTaggedMontage> AMyCharacterBase::GetAttackMontages_Implementation()
{
	return AttackMontages;
}


void AMyCharacterBase::Die(const FVector& DeathImpulse)
{
	// 将武器组件从角色身上分离，但保持武器在当前世界位置不变
	Weapon->DetachFromComponent(FDetachmentTransformRules(EDetachmentRule::KeepWorld,true));
	// 在网络上多播死亡处理函数，让所有客户端都执行相同的死亡效果
	MulticastHandleDeath(DeathImpulse);
	Dissolve();
	bIsDead=true;
	OnDeath.Broadcast(this);
}

void AMyCharacterBase::Dissolve()
{
	//1.角色模型溶解
	if (IsValid(DissolveMaterialInstance))
	{
		// 创建动态材质实例
		UMaterialInstanceDynamic* DynamicMatInst=UMaterialInstanceDynamic::Create(DissolveMaterialInstance,this);
		// 应用到角色网格的第一个材质槽
		GetMesh()->SetMaterial(0,DynamicMatInst);
		// 启动溶解动画时间轴（调用事件）
		StartDissolveTimeline(DynamicMatInst);
	}
	//2.武器模型溶解
	if (IsValid(WeaponDissolveMaterialInstance))
	{
		UMaterialInstanceDynamic* WeaponDynamicMatInst=UMaterialInstanceDynamic::Create(WeaponDissolveMaterialInstance,this);
		Weapon->SetMaterial(0,WeaponDynamicMatInst);
		StartWeaponDissolveTimeline(WeaponDynamicMatInst);
	}
}

bool AMyCharacterBase::IsDead_Implementation()
{
	return bIsDead;
}

AActor* AMyCharacterBase::GetAvatar_Implementation()
{
	return this;
}

UNiagaraSystem* AMyCharacterBase::GetBloodEffect_Implementation()
{
	return BloodEffect;
}

FTaggedMontage AMyCharacterBase::GetTaggedMontageByTag_Implementation(const FGameplayTag& Tag)
{
	for (FTaggedMontage TaggedMontage:AttackMontages)
	{
		if (TaggedMontage.MontageTag.MatchesTagExact(Tag))
		{
			return TaggedMontage;
		}
	}
	return FTaggedMontage();
}

int32 AMyCharacterBase::GetMinionCount_Implementation()
{
	return MinionsCount;
}

void AMyCharacterBase::IncrementMinionCount_Implementation(const int32 Amount)
{
	MinionsCount+=Amount;
}

ECharacterClass AMyCharacterBase::GetCharacterClass_Implementation()
{
	return CharacterClass;
}

FOnASCRegistered& AMyCharacterBase::GetOnASCRegistered()
{
	return OnASCRegistered;
}

FOnDeath& AMyCharacterBase::GetOnDeathDelegate()
{
	return OnDeath;
}

USkeletalMeshComponent* AMyCharacterBase::GetWeapon_Implementation()
{
	return Weapon;
}

void AMyCharacterBase::DeBuffRegisterChanged()
{
	GetAbilitySystemComponent()->RegisterGameplayTagEvent(FAuraGameplayTags::Get().DeBuff_Stun,EGameplayTagEventType::NewOrRemoved).AddUObject(this,&AMyCharacterBase::StunTagChanged);
}

void AMyCharacterBase::StunTagChanged(FGameplayTag CallbackTag, int32 NewCount)
{
	bIsStunned=NewCount>0;
	GetCharacterMovement()->MaxWalkSpeed=bIsStunned?0:BaseWalkSpeed;
}

void AMyCharacterBase::OnRep_Stunned()
{
	
}

void AMyCharacterBase::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AMyCharacterBase,bIsStunned);
}

void AMyCharacterBase::SetIsBeingShock_Implementation(bool bInShock)
{
	IsBeingShock=bInShock;
}

bool AMyCharacterBase::GetIsBeingShock_Implementation()
{
	return IsBeingShock;
}

//死亡物理化处理方式
void AMyCharacterBase::MulticastHandleDeath_Implementation(const FVector& DeathImpulse)
{
	// 让武器开始物理模拟（会受重力、碰撞等物理影响）
	Weapon->SetSimulatePhysics(true);
	// 为武器启用重力效果，使其会自然下落
	Weapon->SetEnableGravity(true);
	// 设置武器的碰撞类型为仅物理碰撞，只参与物理模拟的碰撞
	Weapon->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);

	GetMesh()->SetSimulatePhysics(true);
	GetMesh()->SetEnableGravity(true);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	GetMesh()->SetCollisionResponseToChannel(ECC_WorldStatic,ECR_Block);
	//NAME_None就是应用根骨骼
	//GetMesh()->AddImpulse(DeathImpulse,NAME_None, true);
	GetMesh()->AddImpulse(DeathImpulse);

	// 禁用角色胶囊体的碰撞，避免死亡后胶囊体还阻挡其他角色
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}



