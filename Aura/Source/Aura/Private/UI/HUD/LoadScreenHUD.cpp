// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HUD/LoadScreenHUD.h"

void ALoadScreenHUD::BeginPlay()
{
	Super::BeginPlay();
	LoadScreenViewModel=NewObject<UMVVM_LoadScreen>(this,LoadScreenViewModelClass);
	LoadScreenViewModel->InitializeLoadSlot();

	LoadScreenWidget=CreateWidget<ULoadScreenWidget>(GetWorld(),LoadScreenWidgetClass);
	LoadScreenWidget->AddToViewport();

	FInputModeUIOnly InputMode;
	APlayerController* PlayerController=GetOwningPlayerController();
	//为当前输入模式（InputMode）设置一个需要获得焦点的 Slate Widget
	//因为 UE 的输入模式（FInputModeUIOnly等）仅支持绑定 Slate 类型的 Widget，所以必须通过TakeWidget()完成类型转换，否则无法传入SetWidgetToFocus方法。
	InputMode.SetWidgetToFocus(LoadScreenWidget->TakeWidget());
	//设置鼠标相对于视口（Viewport）的锁定行为，明确指定 “不锁定鼠标到视口内”，让鼠标可以自由移出游戏窗口
	InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	PlayerController->SetInputMode(InputMode);

	PlayerController->SetShowMouseCursor(true);
	LoadScreenWidget->BlueprintInitializeWidget();
}
