// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/AuraAttributeSet.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Net/UnrealNetwork.h"
#include "GameplayEffectExtension.h" // 添加这个包含
#include "GameplayEffectTypes.h"
#include "GameFramework/Character.h"

UAuraAttributeSet::UAuraAttributeSet()
{
	//GEngine->AddOnScreenDebugMessage(-1,5.f,FColor::Black,TEXT("调用构造"));
	InitHealth(50.f);
	InitMaxHealth(100.f);
	InitMana(50.f);
	InitMaxMana(50.f);
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red,FString::Printf(TEXT("%f"),this->GetHealth()));
}


void UAuraAttributeSet::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	//函数中定义哪些属性需要在网络中进行同步复制。
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	//doRep_lifetime——notify
	//COND_None：无条件复制,
	//REPNOTIFY_Always：无论属性值是否实际发生变化，每次收到复制数据时都会调用 OnRep_Health 函数
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet,Health,COND_None,REPNOTIFY_Always);

	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet,MaxHealth,COND_None,REPNOTIFY_Always);

	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet,Mana,COND_None,REPNOTIFY_Always);

	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet,MaxMana,COND_None,REPNOTIFY_Always);

	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet,Strength,COND_None,REPNOTIFY_Always);

	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet,Intelligence,COND_None,REPNOTIFY_Always);

	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet,Resilience,COND_None,REPNOTIFY_Always);

	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet,Vigor,COND_None,REPNOTIFY_Always);
}

//变量(Health)更新时调用这个函数
void UAuraAttributeSet::OnRep_Health(const FGameplayAttributeData& OldHeath) const
{
	//这个函数必须包含这个宏
	//作用：处理属性预测（Prediction）的回滚
	//	   管理属性修饰符的重新应用
	//	   确保GAS内部状态的一致性
	// 通常只触发事件、更新UI等，不直接修改属性
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet,Health,OldHeath)
}

void UAuraAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet,MaxHealth,OldMaxHealth);
}

void UAuraAttributeSet::OnRep_Mana(const FGameplayAttributeData& OldMana) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet,Mana,OldMana);
}

void UAuraAttributeSet::OnRep_MaxMana(const FGameplayAttributeData& OldMana) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet,MaxMana,OldMana);
}

void UAuraAttributeSet::OnRep_Strength(const FGameplayAttributeData& OldStrength) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet,Strength,OldStrength);
}

void UAuraAttributeSet::OnRep_Intelligence(const FGameplayAttributeData& OldIntelligence) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet,Intelligence,OldIntelligence);
}

void UAuraAttributeSet::OnRep_Resilience(const FGameplayAttributeData& OldResilience) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet,Resilience,OldResilience);
}

void UAuraAttributeSet::OnRep_Vigor(const FGameplayAttributeData& OldVigor) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet,Vigor,OldVigor);
}

/*void UAruaAttributeSet::InitAttributeSet()
{
	InitHealth(70.f);
	InitMaxHealth(100.f);
	InitMana(50.f);
	InitMaxMana(50.f);
}*/
void UAuraAttributeSet::InitAttributeSet()
{
	GEngine->AddOnScreenDebugMessage(5, 10.f, FColor::Blue, 
		FString::Printf(TEXT("进入 InitAttributeSet - 当前Health: %f"), GetHealth()));
    
	InitHealth(70.f);
	InitMaxHealth(100.f);
	InitMana(50.f);
	InitMaxMana(100.f);
	GEngine->AddOnScreenDebugMessage(6, 10.f, FColor::Purple, 
		FString::Printf(TEXT("离开 InitAttributeSet - 新Health: %f"), GetHealth()));
}

void UAuraAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);
	if (Attribute==GetHealthAttribute())
	{
		NewValue=FMath::Clamp(NewValue,0,GetMaxHealth());
	}
	if (Attribute==GetManaAttribute())
	{
		NewValue=FMath::Clamp(NewValue,0,GetMaxMana());
	}
}

void UAuraAttributeSet::PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);
	FEffectProperties Props;
	SetEffectProperties(Data,Props);
}

void UAuraAttributeSet::SetEffectProperties(const FGameplayEffectModCallbackData& Data, FEffectProperties& Props) const
{
    Props.EffectContextHandle = Data.EffectSpec.GetContext();
    
    // ========== SOURCE 逻辑 ==========

	//从EffectContextHandle获取ASC
    Props.SourceASC = Props.EffectContextHandle.GetOriginalInstigatorAbilitySystemComponent();
    
    if (IsValid(Props.SourceASC) && Props.SourceASC->AbilityActorInfo.IsValid())
    {
        Props.SourceAvatarActor = Props.SourceASC->GetAvatarActor();
        
        if (IsValid(Props.SourceAvatarActor))
        {
            // 方法1：直接从AbilityActorInfo获取Controller
            Props.SourceController = Props.SourceASC->AbilityActorInfo->PlayerController.Get();
            
            // 方法2：如果方法1失败，从Pawn获取Controller（备用方案）
            if (!Props.SourceController)
            {
                if (APawn* SourcePawn = Cast<APawn>(Props.SourceAvatarActor))
                {
                    Props.SourceController = SourcePawn->GetController();
                }
            }
            
            // 获取SourceCharacter
            if (Props.SourceController)
            {
                Props.SourceCharacter = Cast<ACharacter>(Props.SourceController->GetPawn());
            }
            
            // 备用方案：直接从AvatarActor转换
            if (!Props.SourceCharacter)
            {
                Props.SourceCharacter = Cast<ACharacter>(Props.SourceAvatarActor);
            }
        }
    }
    
    // ========== TARGET 逻辑 ==========
    // 应该添加与Source类似的安全检查
    if (Data.Target.AbilityActorInfo.IsValid())
    {
        Props.TargetAvatarActor = Data.Target.GetAvatarActor();
        Props.TargetASC = &Data.Target; // 直接使用Data.Target，它就是Target的ASC
        
        if (IsValid(Props.TargetAvatarActor))
        {
            // 与方法1类似的Controller获取逻辑
            Props.TargetController = Data.Target.AbilityActorInfo->PlayerController.Get();
            
            // 备用方案
            if (!Props.TargetController)
            {
                if (APawn* TargetPawn = Cast<APawn>(Props.TargetAvatarActor))
                {
                    Props.TargetController = TargetPawn->GetController();
                }
            }
            
            // 获取TargetCharacter
            if (Props.TargetController)
            {
                Props.TargetCharacter = Cast<ACharacter>(Props.TargetController->GetPawn());
            }
            
            // 备用方案
            if (!Props.TargetCharacter)
            {
                Props.TargetCharacter = Cast<ACharacter>(Props.TargetAvatarActor);
            }
        }
    }
}
