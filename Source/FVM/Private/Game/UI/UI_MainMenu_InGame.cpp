// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/UI/UI_MainMenu_InGame.h"
#include "Game/UI/UI_MainMenu_PlayerData.h"

#include "GameSystem/PlayerStructManager.h"
#include "GameSystem/GameConfigManager.h"

#include "GameSystem/FVMGameInstance.h"
#include "GameSystem/Tools/GameSystemFunction.h"

#include <PlatformFeatures.h>
#include <SaveGameSystem.h>

#include <Components/ScrollBox.h>
#include <Components/VerticalBox.h>
#include <Components/EditableTextBox.h>

void UUI_MainMenu_InGame::InitPlayerData()
{
	if (!UFVMGameInstance::GetFVMGameInstance())
		return;

	UGameConfigManager* Config = UFVMGameInstance::GetFVMGameInstance()->GetGameConfigManager();
	if (!Config)
		return;

	//加载对应的界面
	UClass* ObjInstance = LoadClass<UUI_MainMenu_PlayerData>(0, TEXT("WidgetBlueprint'/Game/Resource/BP/Game/InGame/UI_InGame_PlayerData.UI_InGame_PlayerData_C'"));
	UUI_MainMenu_PlayerData* Data = nullptr;



	//当前选中的索引
	int32 _SelectIndex = 0;

	//加载数据
	for (int32 Index = 0; Index < Config->M_PlayerStructManagerName.Num(); Index++)
	{
		Data = CreateWidget<UUI_MainMenu_PlayerData>(this, ObjInstance);
		Data->M_PlayerName = FText::FromString(Config->M_PlayerStructManagerName[Index]);
		//获取等级
		Data->SetImage(UFVMGameInstance::LoadTempPlayerStructConfig(Config->M_PlayerStructManagerName[Index])->GetGradeImagePath());
		Data->M_UI_MainMenu_InGame = this;
		Data->SetIndex(Index);

		//锁定上一次选中的角色的存档
		if (Config->M_CurrentPlayerName.Equals(Data->M_PlayerName.ToString()))
		{
			_SelectIndex = Index;
		}

		if (Config->M_PlayerStructManagerSex[Index] == 0)
			Data->SetTextPink();

		this->M_PlayerSelectBgs.Emplace(Data->GetSelectBoxBg());

		this->M_UVerticalBox_PlayerDataList->AddChild(Data);
	}

	UGameSystemFunction::WidgetSelect(this->GetPlayerSelectBgArrays(), ESlateVisibility::SelfHitTestInvisible, _SelectIndex);

	//初始化玩家当前选择的存档名称
	this->UpdatePlayerTextShow();
}

bool UUI_MainMenu_InGame::Initialize()
{
	if (!Super::Initialize())
		return false;

	//初始化
	this->M_UVerticalBox_PlayerDataList = this->GetWidgetComponent<UVerticalBox>(this, "PlayerDataList");
	this->M_UEditableTextBox_PlayerInput = this->GetWidgetComponent<UEditableTextBox>(this, "Input_Player_Name");

	//初始化角色数据
	//this->InitPlayerData(); 

	//this->M_CurrentDes = FPaths::ProjectSavedDir() + TEXT("SaveGame/");

	return true;
}

void UUI_MainMenu_InGame::AddList(UClass* _Obj, const FString& __PlayerName, uint8 _Sex)
{
	UUI_MainMenu_PlayerData* Data = CreateWidget<UUI_MainMenu_PlayerData>(this, _Obj);
	Data->M_PlayerName = FText::FromString(__PlayerName);
	Data->SetImage(UFVMGameInstance::LoadTempPlayerStructConfig(__PlayerName)->GetGradeImagePath());
	Data->M_UI_MainMenu_InGame = this;
	Data->SetIndex(this->M_PlayerSelectBgs.Num());

	if (_Sex == 0)
		Data->SetTextPink();

	this->M_PlayerSelectBgs.Emplace(Data->GetSelectBoxBg());
	UGameSystemFunction::WidgetSelect(this->M_PlayerSelectBgs, ESlateVisibility::SelfHitTestInvisible, this->M_PlayerSelectBgs.Num() - 1);
	this->M_UVerticalBox_PlayerDataList->AddChild(Data);
}

bool UUI_MainMenu_InGame::CreateNewPlayerData()
{
	// UE_LOG(LogTemp, Error, TEXT("A"));

	const FString PlayerName = this->M_UEditableTextBox_PlayerInput->GetText().ToString();

	//输入的文字是空的
	if (PlayerName.Equals(""))
	{
		UWidgetBase::CreateTipWidget(TEXT("不能是空的哦"), FVector(0.f, 1.f, 1.f));
		return false;
	}

	if (PlayerName.Len() > 8)
	{
		UWidgetBase::CreateTipWidget(TEXT("名字长度不能超过8个哦"), FVector(0.f, 1.f, 1.f));
		return false;
	}

	//获取游戏数据
	if (!UFVMGameInstance::GetFVMGameInstance())
		return false;

	UGameConfigManager* Config = UFVMGameInstance::GetFVMGameInstance()->GetGameConfigManager();

	const FString& _PlayerName = this->M_UEditableTextBox_PlayerInput->GetText().ToString();

	//从存档列表查询是否存在
	if (Config->M_PlayerStructManagerName.Find(_PlayerName) != -1)
		return false;

	//加载对应的界面
	UClass* ObjInstance = LoadClass<UUI_MainMenu_PlayerData>(0, TEXT("WidgetBlueprint'/Game/Resource/BP/Game/InGame/UI_InGame_PlayerData.UI_InGame_PlayerData_C'"));

	//从本地查询是否存在文件
	if (UFVMGameInstance::GetFVMGameInstance()->IsFileExist(_PlayerName + FString("P")))
	{
		//如果已经存在了，直接加载到列表
		Config->M_PlayerStructManagerName.Emplace(_PlayerName);
		Config->M_CurrentPlayerName = _PlayerName;
		UFVMGameInstance::GetFVMGameInstance()->LoadPlayerStructConfig(_PlayerName);
		//性别加载
		Config->M_PlayerStructManagerSex.Emplace(UFVMGameInstance::GetFVMGameInstance()->GetPlayerStructManager()->M_PlayerSex);

		this->AddList(ObjInstance, _PlayerName, UFVMGameInstance::GetFVMGameInstance()->GetPlayerStructManager()->M_PlayerSex);
	}
	else {
		//添加角色
		Config->M_PlayerStructManagerName.Emplace(_PlayerName);
		Config->M_PlayerStructManagerSex.Emplace(this->M_PlayerSex);
		Config->M_CurrentPlayerName = _PlayerName;

		UFVMGameInstance::GetFVMGameInstance()->CreatePlayerStructConfig(_PlayerName);

		//将当前创建的新的角色存档就行修改和保存
		UFVMGameInstance::GetFVMGameInstance()->GetPlayerStructManager()->M_PlayerName = _PlayerName;
		UFVMGameInstance::GetFVMGameInstance()->GetPlayerStructManager()->M_PlayerSex = this->M_PlayerSex;
		UFVMGameInstance::GetFVMGameInstance()->GetPlayerStructManager()->M_GameVersion = UFVMGameInstance::GetGameVersion();
		UFVMGameInstance::GetFVMGameInstance()->GetPlayerStructManager()->UpdateEquipID(150);
		UFVMGameInstance::GetFVMGameInstance()->SavePlayerStructConfig(_PlayerName);

		//发送初始卡片到背包
		UGameSystemFunction::SendCardToPlayerBag(FString(TEXT("小笼包")), 0);
		UGameSystemFunction::SendCardToPlayerBag(FString(TEXT("小火炉")), 0);
		UGameSystemFunction::SendCardToPlayerBag(FString(TEXT("木盘子")), 0);
		UGameSystemFunction::SendCardToPlayerBag(FString(TEXT("鲈鱼")), 0);
		UGameSystemFunction::SendCardToPlayerBag(FString(TEXT("酒杯灯")), 0);
		UGameSystemFunction::SendCardToPlayerBag(FString(TEXT("咖啡杯")), 0);
		
		//发送礼盒
		UGameSystemFunction::SendEquipmentToPlayerBag(FString(TEXT("十三香礼包")), 1);
		UGameSystemFunction::SendEquipmentToPlayerBag(FString(TEXT("菜鸟饭盒")), 1);
		UGameSystemFunction::SendEquipmentToPlayerBag(FString(TEXT("龙虾礼盒")), 10);

		this->AddList(ObjInstance, _PlayerName, this->M_PlayerSex);
	}

	this->UpdatePlayerTextShow();
	return true;
}

void UUI_MainMenu_InGame::UpdatePlayerTextShow()
{
	if (!UFVMGameInstance::GetFVMGameInstance())
		return;

	UGameConfigManager* Config = UFVMGameInstance::GetFVMGameInstance()->GetGameConfigManager();

	//初始化玩家当前选择的存档名称
	this->M_CurrentPlayerDataName = FText::FromString(Config->M_CurrentPlayerName);
}

bool UUI_MainMenu_InGame::InspectPlayerData()
{
	if (!UFVMGameInstance::GetFVMGameInstance())
		return false;

	UGameConfigManager* Config = UFVMGameInstance::GetFVMGameInstance()->GetGameConfigManager();
	if (!Config)
		return false;

	//先加载选择的角色存档
	if (!UFVMGameInstance::GetFVMGameInstance()->LoadPlayerStructConfig(this->M_CurrentPlayerDataName.ToString()))
	{
		//如果没有加载成功则加载默认存档
		Config->M_CurrentPlayerName = FString("");
		UFVMGameInstance::GetFVMGameInstance()->InitPlayerStructManager();
		return false;
	}

	UPlayerStructManager* PlayerData = UFVMGameInstance::GetFVMGameInstance()->GetPlayerStructManager();

	//进行版本匹配
	if (!PlayerData->M_GameVersion.Equals(UFVMGameInstance::GetGameVersion()))
	{

		//设置游戏更新
		UFVMGameInstance::SetUpdateGame();

		//创建弹窗
		UUserWidget* Widget = CreateWidget<UUserWidget>(this, LoadClass<UUserWidget>(0, TEXT("WidgetBlueprint'/Game/Resource/BP/Game/UI/UI_Tip/BPUI_UpdateGameVersion.BPUI_UpdateGameVersion_C'")));
		if (Widget)
			Widget->AddToViewport();
	}

	//发送补偿邮件
	UGameSystemFunction::SendVersionCompensate();

	return true;
}

TArray<UWidget*>& UUI_MainMenu_InGame::GetPlayerSelectBgArrays()
{
	return this->M_PlayerSelectBgs;
}
