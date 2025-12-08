// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/EnemyCharacter.h"

#include "AuraGameplayTags.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "AI/AuraAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/CharacterMovementComponent.h"


void AEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();
	GetCharacterMovement()->MaxWalkSpeed=BaseWalkSpeed;
	//敌人初始化，Owner和Avatar都是自己
	InitialAbilityActorInfo();
	if (HasAuthority())
	{
		UAuraAbilitySystemLibrary::GiveStartupAbilities(this,AbilitySystemComponent,CharacterClass);
	}
	//GetUserWidgetObject()：获取组件所指向的控件
	if (UAuraUserWidget* AuraUserWidget=Cast<UAuraUserWidget>(HealthBar->GetUserWidgetObject()))
	{
		AuraUserWidget->SetWidgetController(this);
	}
	if (UAuraAttributeSet* AuraAttributeSet = Cast<UAuraAttributeSet>(GetAttributeSet()))
	{
		// 绑定委托函数
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuraAttributeSet->GetHealthAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data)
			{
				OnHealthChange.Broadcast(Data.NewValue);
			});
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuraAttributeSet->GetMaxHealthAttribute()).AddLambda(
			[this, AuraAttributeSet](const FOnAttributeChangeData& Data) // 显式捕获AuraAttributeSet
			{
				OnMaxHealthChange.Broadcast(AuraAttributeSet->GetMaxHealth());
			});
    
		// 初始化血条
		OnHealthChange.Broadcast(AuraAttributeSet->GetHealth());
		OnMaxHealthChange.Broadcast(AuraAttributeSet->GetMaxHealth());
	}

	FAuraGameplayTags AuraGameplayTags=FAuraGameplayTags::Get();
	AbilitySystemComponent->RegisterGameplayTagEvent(FAuraGameplayTags::Get().Effect_HitReact,EGameplayTagEventType::NewOrRemoved).AddUObject(
			this,
			&AEnemyCharacter::HitReactTagChanged);
}

AEnemyCharacter::AEnemyCharacter()
{
	GetMesh()->SetCollisionResponseToChannel(ECC_Visibility,ECR_Block);

	AbilitySystemComponent=CreateDefaultSubobject<UAuraAbilitySystemComponent>("AbilitySystemComponent");
	//启用游戏能力系统组件（GAS）的网络复制功能。
	AbilitySystemComponent->SetIsReplicated(true);
	AttributeSet=CreateDefaultSubobject<UAuraAttributeSet>("AttributeSet");
	//设置复制模式
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);
	HealthBar=CreateDefaultSubobject<UWidgetComponent>(TEXT("HealthBar"));
	HealthBar->SetupAttachment(RootComponent);
}

void AEnemyCharacter::HighlightActor()
{
	bHighlight = true;
	// 启用网格体的自定义深度渲染
	GetMesh()->SetRenderCustomDepth(true);
	//// 设置模板值为250
	GetMesh()->SetCustomDepthStencilValue(250);
	Weapon->SetRenderCustomDepth(true);
	Weapon->SetCustomDepthStencilValue(250);
}

void AEnemyCharacter::UnHighlightActor()
{
	bHighlight = false;
	GetMesh()->SetRenderCustomDepth(false);
	Weapon->SetRenderCustomDepth(false);
	//GetMesh()->SetCustomDepthStencilValue(250);
}

void AEnemyCharacter::InitialAbilityActorInfo()
{
	// 初始化基础 Ability Actor Info
	AbilitySystemComponent->InitAbilityActorInfo(this, this);
    Cast<UAuraAbilitySystemComponent>(AbilitySystemComponent)->AbilityActorInfoSet();
	InitializeDefaultAttributes();
	OnASCRegistered.Broadcast(AbilitySystemComponent);
}

void AEnemyCharacter::InitializeDefaultAttributes() const
{
	UAuraAbilitySystemLibrary::InitializeDefaultAttributes(this,CharacterClass,Level,AbilitySystemComponent);
}


void AEnemyCharacter::HitReactTagChanged(const FGameplayTag GameplayTag, int32 NewCount)
{
	//NewCount是自动更新的，应用Effect的时候+1
	bHitReacting=NewCount>0;
	GetCharacterMovement()->MaxWalkSpeed=bHitReacting?0.f:BaseWalkSpeed;
	if (AuraAIController&&AuraAIController->GetBlackboardComponent())
	{
		AuraAIController->GetBlackboardComponent()->SetValueAsBool("HitReacting",bHitReacting);
	}
}

UAnimMontage* AEnemyCharacter::GetHitReactMontage_Implementation()
{
	return HitReactMontage;
}

void AEnemyCharacter::Die(const FVector& DeathImpulse)
{
	SetLifeSpan(LifeSpan);
	Super::Die(DeathImpulse);
	
}

void AEnemyCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	if (!HasAuthority())return;
	//确保只在服务器中初始化AI行为
	AuraAIController=Cast<AAuraAIController>(NewController);
	AuraAIController->GetBlackboardComponent()->InitializeBlackboard(*BehaviorTree->BlackboardAsset);
	AuraAIController->GetBlackboardComponent()->SetValueAsBool(FName("HitReacting"),false);
	AuraAIController->GetBlackboardComponent()->SetValueAsBool(FName("RangeAttack"),CharacterClass!=ECharacterClass::Warrior);
	AuraAIController->RunBehaviorTree(BehaviorTree);
	
}

void AEnemyCharacter::SetCombatTarget_Implementation(AActor* InCombatTarget)
{
	TargetActor=InCombatTarget;
}

AActor* AEnemyCharacter::GetCombatTarget_Implementation()
{
	return TargetActor;
}

int32 AEnemyCharacter::GetPlayerLevel_Implementation()
{
	return Level;
}
