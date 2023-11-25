// Fill out your copyright notice in the Description page of Project Settings.


#include "GameStart/VS/GameMapInstance.h"

#include "GameSystem/GameMapStructManager.h"

#include "GameStart/VS/Components/MesheControllComponent.h"
#include "GameStart/VS/Components/VSManagerComponent.h"
#include "GameStart/VS/Components/CardManagerComponent.h"
#include "GameStart/VS/Components/MouseManagerComponent.h"
#include "GameStart/VS/Components/ResourceManagerComponent.h"

#include "GameStart/VS/PlayerCameraPosition.h"

#include "GameStart/VS/MapMeshe.h"
#include "GameStart/VS/PlayerView.h"

#include "GameStart/Flipbook/GameActor/GamePlayer.h"

#include <Paper2D/Classes/PaperSprite.h>
#include <Paper2D/Classes/PaperSpriteActor.h>
#include <Paper2D/Classes/PaperSpriteComponent.h>

#include "GameSystem/FVMGameInstance.h"

#include "UI/WidgetBase.h"

#include <Kismet/GameplayStatics.h>
#include <Sound/SoundBase.h>

UCardManagerComponent* AGameMapInstance::GetCardManagerComponent_Static()
{
	return AGameMapInstance::GetGameMapInstance()->M_CardManagerComponent;
}

UVSManagerComponent* AGameMapInstance::GetVSManagerComponent_Static()
{
	return AGameMapInstance::GetGameMapInstance()->M_VSManagerComponent;
}

UMouseManagerComponent* AGameMapInstance::GetMouseManagerComponent_Static()
{
	return AGameMapInstance::GetGameMapInstance()->M_MouseManagerComponent;
}

UResourceManagerComponent* AGameMapInstance::GetResourceManagerComponent_Static()
{
	return AGameMapInstance::GetGameMapInstance()->M_ResourceManagerComponent;
}

// Sets default values
AGameMapInstance::AGameMapInstance()
{
	PrimaryActorTick.bCanEverTick = true;

	//网格控制管理
	//this->M_MesheControllComponent = this->CreateDefaultSubobject<UMesheControllComponent>("LUMesheControllComponent");
	//VS管理
	//this->M_VSManagerComponent = this->CreateDefaultSubobject<UVSManagerComponent>("LUVSManagerComponent");
	//卡片管理
	//this->M_CardManagerComponent = this->CreateDefaultSubobject<UCardManagerComponent>("LUCardManagerComponent");;
	//老鼠管理
	//this->M_MouseManagerComponent = this->CreateDefaultSubobject<UMouseManagerComponent>("LUMouseManagerComponent");;
	//资源管理
	//this->M_ResourceManagerComponent = this->CreateDefaultSubobject<UResourceManagerComponent>("LUResourceManagerComponent");
}

//初始化静态变量
AGameMapInstance* AGameMapInstance::M_AGameMapInstance = nullptr;

AGameMapInstance* AGameMapInstance::GetGameMapInstance()
{
	return AGameMapInstance::M_AGameMapInstance;
}

void AGameMapInstance::CreatePaperSpriteActorToWorld(APaperSpriteActor* _Actor, UPaperSprite* _PSprite, const FTransform& _ActorTranform)
{
	if (!_Actor)
		return;

	APaperSpriteActor* LAMapBackground = this->GetWorld()->SpawnActor<APaperSpriteActor>(_Actor->GetClass(), _ActorTranform);
	LAMapBackground->GetRootComponent()->SetMobility(EComponentMobility::Movable);
	LAMapBackground->GetRenderComponent()->SetSprite(_PSprite);
}

UMesheControllComponent* AGameMapInstance::GetMesheControllComponent()
{
	return this->M_MesheControllComponent;
}

void AGameMapInstance::InitGameMapStructManager(UGameMapStructManager* _UGameMapStructManager)
{
	this->M_UGameMapStructManager = _UGameMapStructManager;
}

UGameMapStructManager* AGameMapInstance::GetGameMapStructManager()
{
	return this->M_UGameMapStructManager;
}

AGamePlayer* AGameMapInstance::GetPlayer()
{
	return this->M_PlayerInstance;
}

void AGameMapInstance::PlaySound(const FSoftObjectPath& _MusicName)
{
	//如果正在播放，那么判断正在播放的音频是否一样如果一样则不改变
	if (_MusicName.ToString().Equals(this->M_CurrentPlaySoundName))
		return;

	UFVMGameInstance::GetFVMGameInstance()->PlayBGM(_MusicName);

	this->M_CurrentPlaySoundName = _MusicName.ToString();
}

bool AGameMapInstance::GetGameOver()
{
	return this->IsInGame;
}

bool AGameMapInstance::GetGamePause()
{
	return this->IsInPause;
}

bool AGameMapInstance::GetIsPlayPlayer()
{
	return this->M_bIsPlayPlayer;
}

void AGameMapInstance::SetGameOver(bool _Value)
{
	this->IsInGame = _Value;
}

void AGameMapInstance::SetGamePause(bool _Value)
{
	//如果角色已经放置了，才可以自由设置
	if (this->M_bIsPlayPlayer)
		this->IsInPause = _Value;
	else
		this->IsInPause = true;
}

void AGameMapInstance::SetGameStartNow()
{
	if (IsValid(this->M_UI_PlayPlayerTipsWidget))
		this->M_UI_PlayPlayerTipsWidget->RemoveFromParent();

	this->M_bIsPlayPlayer = true;

	this->SetGamePause(false);
}


void AGameMapInstance::BeginPlay()
{
	Super::BeginPlay();
	//初始化静态变量
	this->InitStaticGameMapInstance();
	//初始化背景
	this->InitGameMapBackground();

	//初始化游戏
	if (!this->M_bIsPlayPlayer)
	{
		//如果玩家没有放置->显示界面---提示玩家放置角色
		this->M_UI_PlayPlayerTipsWidget = CreateWidget<UWidgetBase>(this->GetWorld(), LoadClass<UWidgetBase>(0, TEXT("WidgetBlueprint'/Game/Resource/BP/GameStart/VS/UI_Player/Tips/BPUI_PlayerPlayTips.BPUI_PlayerPlayTips_C'")));
		this->M_UI_PlayPlayerTipsWidget->AddToViewport(0);

		this->SetGamePause(false);
	}
}

void AGameMapInstance::InitGameMapBackground()
{
	//创建背景
	this->M_AMapBackground = NewObject<APaperSpriteActor>(this->GetWorld(), "M_AMapBackground");

	this->CreatePaperSpriteActorToWorld(this->M_AMapBackground, this->M_MapSprite.M_MapBackgroundSprite,
		this->GetActorTransform());

	//设置摄像机位置
	//FVector Location = FVector::ZeroVector;
	//Location.Z = this->GetActorLocation().Z;

	//APlayerView::SetPlayerPositionAndFOV(Location,90.f);
}

// Called every frame
void AGameMapInstance::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AGameMapInstance::GameStart()
{
	this->IsInGame = true;

	if (IsValid(this->M_VSManagerComponent))
	{
		this->M_VSManagerComponent->GameStart();
		this->M_CardManagerComponent->GameStart();
		this->M_MouseManagerComponent->GameStart();
		this->M_ResourceManagerComponent->GameStart();
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("请重新启动项目!!!"));
	}

	//音乐初始化
	if (this->M_UGameMapStructManager)
		this->PlaySound(this->M_UGameMapStructManager->LevelConfig.LevelDefMusic);

}

void AGameMapInstance::GameOver()
{
	FInputModeGameAndUI GameUI;
	GameUI.SetHideCursorDuringCapture(false);
	GameUI.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	UGameplayStatics::GetPlayerController(this->GetWorld(), 0)->SetInputMode(GameUI);


	this->IsInGame = false;

	//优先结算资源管理器
	if (this->M_ResourceManagerComponent)
	{
		this->M_ResourceManagerComponent->GameOver();
	}

	if (this->M_VSManagerComponent)
	{
		this->M_VSManagerComponent->GameOver();
	}

	if (this->M_CardManagerComponent)
	{
		this->M_CardManagerComponent->GameOver();
	}

	if (this->M_MouseManagerComponent)
	{
		this->M_MouseManagerComponent->GameOver();
	}


}

void AGameMapInstance::QuitGame()
{
	FInputModeGameAndUI GameUI;
	GameUI.SetHideCursorDuringCapture(false);
	GameUI.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	UGameplayStatics::GetPlayerController(this->GetWorld(), 0)->SetInputMode(GameUI);

	this->IsInGame = false;
	this->M_CardManagerComponent->GameOver();
	this->M_MouseManagerComponent->GameOver();
	this->M_ResourceManagerComponent->GameOver();
}

void AGameMapInstance::SpawnPlayerToMeshe(AMapMeshe* _MapMeshe, UUI_MapMeshe* _UI_MapMeshe)
{
	if (!IsValid(_MapMeshe))
		return;

	this->M_PlayerInstance = this->GetWorld()->SpawnActor<AGamePlayer>(LoadClass<AGamePlayer>(0, TEXT("Blueprint'/Game/Resource/BP/GameStart/Item/Player/MyGamePlayer.MyGamePlayer_C'")));
	this->M_PlayerInstance->SetActorLocation(_MapMeshe->GetActorLocation() + FVector(0.f, 0.f, -5.f));
	this->M_PlayerInstance->SetActorRotation(FRotator(0.f, 90.f, 0.f));
	//初始化网格
	this->M_PlayerInstance->InitMeshe(_UI_MapMeshe, _MapMeshe);
	//初始化武器
	this->M_PlayerInstance->InitPlayerWeapon();
	//初始化排序
	this->M_PlayerInstance->SetPlayerTranslucency(_UI_MapMeshe);

	this->SetGameStartNow();
}

bool AGameMapInstance::InitStaticGameMapInstance()
{
	//判断当前变量是否有效
	AGameMapInstance::M_AGameMapInstance = this;
	return true;
}

bool FVM_VS_GamePause()
{
	if (IsValid(AGameMapInstance::GetGameMapInstance()))
	{
		return  AGameMapInstance::GetGameMapInstance()->GetGamePause();
	}

	return true;
}

bool FVM_VS_GameOver()
{
	if (IsValid(AGameMapInstance::GetGameMapInstance()))
	{
		return  AGameMapInstance::GetGameMapInstance()->GetGameOver();
	}

	return true;
}
