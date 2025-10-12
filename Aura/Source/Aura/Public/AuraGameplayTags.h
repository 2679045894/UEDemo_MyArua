// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

struct FAuraGameplayTags
{
public:
	
	//单例访问方法，返回全局唯一实例的引用
	static const FAuraGameplayTags& Get(){return GameplayTags;}

	//静态初始化方法
	static void InitializeNativeGameplayTags();

private:
	//静态单例实例，整个程序生命周期内存在
	static FAuraGameplayTags GameplayTags;

protected:
};
