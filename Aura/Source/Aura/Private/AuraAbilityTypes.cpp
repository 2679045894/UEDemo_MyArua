#include "AuraAbilityTypes.h"

//核心目的：提供多态深拷贝。
FGameplayEffectContext* FAuraGameplayContext::Duplicate() const
{
	FGameplayEffectContext* NewContext=new FGameplayEffectContext();
	*NewContext= *this;
	if (GetHitResult())
	{
		NewContext->AddHitResult(*GetHitResult(),true);
	}
	return NewContext;
}

//按需同步
bool FAuraGameplayContext::NetSerialize(FArchive& Ar, class UPackageMap* Map, bool& bOutSuccess)
{
	uint32 RepBits=0;
	//第一部分：发送前的检查（打包时的清单制作）
	if (Ar.IsSaving())
	{
		//如果要同步施法者并且施法者指针有效，就在清单第0位打勾。
		//游戏意义：记录是谁释放的这个技能（比如玩家A施放了火球术）
		if (bReplicateInstigator&&Instigator.IsValid())
		{
			RepBits|=1<<0;
		}
		//含义：如果要同步效果来源并且效果来源有效，就在清单第1位打勾。
		//游戏意义：记录是哪个具体物体造成效果（比如是火球术的投射物本身）
		if (bReplicateEffectCauser&&EffectCauser.IsValid())
		{
			RepBits|=1<<1;
		}
		//含义：如果技能原型有效，就在清单第2位打勾。
		//游戏意义：记录是哪个技能造成这个效果（比如是"火球术"技能而不是"寒冰箭"）
		if (AbilityCDO.IsValid())
		{
			RepBits|=1<<2;
		}
		//含义：如果要同步源对象并且源对象有效，就在清单第3位打勾。
		//游戏意义：记录技能的来源对象（比如是来自某个装备或buff）
		if (bReplicateSourceObject&&SourceObject.IsValid())
		{
			RepBits|=1<<3;
		}
		//含义：如果相关Actor数组不为空，就在清单第4位打勾。
		//游戏意义：记录其他相关的Actor（比如范围技能影响到的多个目标）
		if (Actors.Num()>0)
		{
			RepBits|=1<<4;
		}
		if (HitResult.IsValid())
		{
			RepBits|=1<<5;
		}
		//含义：如果有世界原点信息，就在清单第6位打勾。
		//游戏意义：记录效果产生的源头位置（比如爆炸的中心点）
		if (bHasWorldOrigin)
		{
			RepBits|=1<<6;
		}
		if (bIsBlockedHit)
		{
			RepBits|=1<<7;
		}
		if (bIsCriticalHit)
		{
			RepBits|=1<<8;
		}
		if (bIsSuccessfulDeBuff)
		{
			RepBits|=1<<9;
		}
		if (DeBuffDamage>0.f)
		{
			RepBits|=1<<10;
		}
		if (DeBuffDuration>0.f)
		{
			RepBits|=1<<11;
		}
		if (DeBuffFrequency>0.f)
		{
			RepBits|=1<<12;
		}
		if (DeBuffDamageType.IsValid())
		{
			RepBits|=1<<13;
		}
		if (!DeathImpulse.IsZero())
		{
			RepBits|=1<14;
		}
		if (!KnockbackForce.IsZero())
		{
			RepBits|=1<<15;
		}
	}
	
	//把刚才制作的14位清单发送给客户端（或者客户端接收这个清单）
	Ar.SerializeBits(&RepBits,16);
	if (RepBits&(1<<0))
	{
		Ar<<Instigator;
	}
	if (RepBits&(1<<1))
	{
		Ar<<EffectCauser;
	}
	if (RepBits&(1<<2))
	{
		Ar<<AbilityCDO;
	}
	if (RepBits&(1<<3))
	{
		Ar<<SourceObject;
	}
	if (RepBits&(1<<4))
	{
		SafeNetSerializeTArray_Default<31>(Ar,Actors);
	}
	//含义：如果清单第5位有勾，就同步命中结果。
	//特殊处理：客户端如果没有命中结果对象，要先创建一个空的，然后再填充数据。
	if (RepBits&(1<<5))
	{
		if (Ar.IsLoading())
		{
			if (!HitResult.IsValid())
			{
				HitResult=TSharedPtr<FHitResult>(new FHitResult());
			}
		}
		//NetSerialize用于处理复合数据类型
		HitResult->NetSerialize(Ar,Map,bOutSuccess);
	}
	if (RepBits&(1<<6))
	{
		Ar<<WorldOrigin;
		bHasWorldOrigin=true;
	}
	else
	{
		bHasWorldOrigin=false;
	}
	if (RepBits&(1<<7))
	{
		Ar<<bIsBlockedHit;
	}
	if (RepBits&(1<<8))
	{
		Ar<<bIsCriticalHit;
	}
	if (RepBits&(1<<9))
	{
		Ar<<bIsSuccessfulDeBuff;
	}
	if (RepBits&(1<<10))
	{
		Ar<<DeBuffDamage;
	}
	if (RepBits&(1<<11))
	{
		Ar<<DeBuffDuration;
	}
	if (RepBits&(1<<12))
	{
		Ar<<DeBuffFrequency;
	}
	if (RepBits & (1 << 13))
	{
		if (Ar.IsLoading())
		{
			if (!DeBuffDamageType.IsValid())
			{
				DeBuffDamageType = TSharedPtr<FGameplayTag>(new FGameplayTag());
			}
		}
		//NetSerialize用于处理复合数据类型
		DeBuffDamageType->NetSerialize(Ar, Map, bOutSuccess);
	}
	if (RepBits&(1<<14))
	{
		//NetSerialize用于处理复合数据类型
		DeathImpulse.NetSerialize(Ar, Map, bOutSuccess);
	}
	if (RepBits&(1<<15))
	{
		KnockbackForce.NetSerialize(Ar,Map,bOutSuccess);
	}
	
	//最后的后处理
	//客户端在收到所有数据后，要初始化一些内部组件引用。
	if (Ar.IsLoading())
	{
		AddInstigator(Instigator.Get(),EffectCauser.Get());
	}
	bOutSuccess = true;
	return true;
}
