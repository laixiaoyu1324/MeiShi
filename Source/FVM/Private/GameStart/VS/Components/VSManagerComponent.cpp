// Fill out your copyright notice in the Description page of Project Settings.


#include "GameStart/VS/Components/VSManagerComponent.h"
#include "GameStart/VS/Components/MouseManagerComponent.h"

#include "GameStart/VS/UI/UI_VSCountDown.h"
#include "GameStart/VS/UI/UI_GameOver.h"

#include "GameStart/VS/GameMapInstance.h"

#include "GameSystem/FVMGameInstance.h"
#include "GameSystem/PlayerStructManager.h"
#include "GameSystem/GameMapStructManager.h"
#include "GameSystem/Tools/GameSystemFunction.h"

#include "GameSystem/MouseStructManager.h"

#include "Math/UnrealMathUtility.h"

#include "Game/UI/Tips/UI_BottomTip.h"

#include <Kismet/KismetSystemLibrary.h>

// Sets default values for this component's properties
UVSManagerComponent::UVSManagerComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UVSManagerComponent::BeginPlay()
{
	Super::BeginPlay();


	// ...
	//UKismetSystemLibrary::ExecuteConsoleCommand(this->GetWorld(), FString(TEXT("viewmode unlit")));
	//UKismetSystemLibrary::ExecuteConsoleCommand(this->GetWorld(), FString(TEXT("stat fps")));

}


// Called every frame
void UVSManagerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);


	if (AGameMapInstance::GetGameMapInstance()->GetGamePause())
		return;


	if (AGameMapInstance::GetMouseManagerComponent_Static()->IsMouseSpawnEnd())
	{
		if (!AGameMapInstance::GetMouseManagerComponent_Static()->IsMouseExist())
		{
			this->SetComponentTickEnabled(false);

			//游戏结束
			this->GameWin();
		}
	}
}



void UVSManagerComponent::SetGamePause()
{
	AGameMapInstance::GetGameMapInstance()->SetGamePause(true);
}

void UVSManagerComponent::SetGameContinue()
{
	AGameMapInstance::GetGameMapInstance()->SetGamePause(false);
}

void UVSManagerComponent::GameStart()
{

	//创建按钮
	this->M_UIBottomTip = CreateWidget<UUI_BottomTip>(
		this->GetWorld(),
		LoadClass<UUI_BottomTip>(0, TEXT("WidgetBlueprint'/Game/Resource/BP/Game/UI/UI_Tip/BPUI_BottomTip.BPUI_BottomTip_C'"))
		);



	//创建倒计时
	if (IsValid(AGameMapInstance::GetGameMapInstance()->GetGameMapStructManager()))
	{
		if (AGameMapInstance::GetGameMapInstance()->GetGameMapStructManager()->LevelConfig.LevelTimeWidthLevelConfig.bEnable)
		{
			this->UI_VSCountDown = CreateWidget<UUI_VSCountDown>(
				this->GetWorld(),
				LoadClass<UUI_VSCountDown>(0, TEXT("WidgetBlueprint'/Game/Resource/BP/GameStart/VS/UI_Player/Tips/BPUI_Timer.BPUI_Timer_C'"))
				);

			//开始倒计时
			this->UI_VSCountDown->Enable(
				AGameMapInstance::GetGameMapInstance()->GetGameMapStructManager()->LevelConfig.LevelTimeWidthLevelConfig.MaxTime
			);

			//添加到屏幕
			this->UI_VSCountDown->AddToViewport();
		}
	}
}

void UVSManagerComponent::GameOver()
{
	//暂停游戏
	this->SetGamePause();

	//关闭计时
	if (IsValid(this->UI_VSCountDown))
	{
		this->UI_VSCountDown->DisEnable();
	}

	//结算界面
	this->M_UI_GameOver = CreateWidget<UUI_GameOver>(this->GetWorld(), LoadClass<UUI_GameOver>(0, TEXT("WidgetBlueprint'/Game/Resource/BP/GameStart/VS/UI_Player/UI_GameOver.UI_GameOver_C'")));
	this->M_UI_GameOver->AddToViewport();
	this->M_UI_GameOver->ShowOver1();
	this->M_UI_GameOver->ShowOver2(M_Win);
	this->M_UI_GameOver->ShowOver3();

	//播放音乐
	if (M_Win)
	{
		UFVMGameInstance::PlayBGM_Static(FString(TEXT("SoundWave'/Game/Resource/BP/Audio/GameBG/Win.Win'")), false);
	}
	else {
		UFVMGameInstance::PlayBGM_Static(FString(TEXT("SoundWave'/Game/Resource/BP/Audio/GameBG/Failed.Failed'")), false);
	}
}

void UVSManagerComponent::ShowText(const FString& _Title)
{
	if (IsValid(this->M_UIBottomTip))
	{
		if (this->M_UIBottomTip->IsInViewport())
			this->M_UIBottomTip->RemoveFromViewport();

		this->M_UIBottomTip->AddToViewport(1);
		this->M_UIBottomTip->SetTitleAndPlay(_Title);
	}
}

void UVSManagerComponent::GameWin()
{
	AGameMapInstance::GetGameMapInstance()->M_VSManagerComponent->M_Win = true;

	AGameMapInstance::GetGameMapInstance()->GameOver();
}

void UVSManagerComponent::GameFailed()
{
	if (AGameMapInstance::GetGameMapInstance()->M_VSManagerComponent->M_bUseKey)
	{
		AGameMapInstance::GetGameMapInstance()->M_VSManagerComponent->M_Win = true;
	}
	else {
		AGameMapInstance::GetGameMapInstance()->M_VSManagerComponent->M_Win = false;
	}

	AGameMapInstance::GetGameMapInstance()->GameOver();
}

