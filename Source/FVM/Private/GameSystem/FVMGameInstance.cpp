// Fill out your copyright notice in the Description page of Project Settings.


#include "GameSystem/FVMGameInstance.h"
#include "GameSystem/PlayerStructManager.h"
#include "GameSystem/GameMapStructManager.h"
#include "GameSystem/GameConfigManager.h"
#include "GameSystem/MouseStructManager.h"

#include "GameSystem/Tools/GameSystemFunction.h"

#include "Game/UI/UI_MainFramework.h"

//地图数据-老鼠回合数据表
#include "Data/MapData/MapDataStruct.h"
//技能书数据表
#include "Data/CardSkillBookStruct.h"

#include "GameSystem/GlobalDatas.h"

#include <Components/AudioComponent.h>
#include <Kismet/GameplayStatics.h>
#include <Kismet/KismetSystemLibrary.h>

#include "GameFramework/GameUserSettings.h"

//UI 层 函数库
#include <Blueprint/WidgetLayoutLibrary.h>

//标题提示UI->争对Windows平台
#include "Game/UI/Tips/UI_ItemTitleTip.h"

//初始化静态游戏实例
UFVMGameInstance* UFVMGameInstance::M_StaticUFVMGameInstance = nullptr;
//是否显示游戏测试文字
bool UFVMGameInstance::M_bShowDebugText = false;

UUI_ItemTitleTip* const UFVMGameInstance::GetUI_ItemTitleTip()
{
	if (UGameplayStatics::GetPlatformName().Equals(TEXT("Windows")))
	{
		return this->M_Windows_ShowUI_ItemTitleTip;
	}

	return nullptr;
}

UFVMGameInstance::UFVMGameInstance()
{

}

void UFVMGameInstance::Init()
{
	Super::Init();

	//UGameplayStatics::GetPlayerCameraManager(this->GetWorld(), 0);

	UFVMGameInstance::M_StaticUFVMGameInstance = this;
	UE_LOG(LogTemp, Log, TEXT("加载静态UFVMGameInstance"));


	//预加载数据
	if (UFVMGameInstance::GetDebug())
		UE_LOG(LogTemp, Log, TEXT("加载数据表!"));
	UGlobalDatas::InitCardsDatasLoader();


	//加载游戏配置
	this->LoadSaveDataToGameConfigManager();
	//初始化角色存档
	this->InitPlayerStructManager();
	//初始化默认地图配置
	this->InitGameMapConfigManager();
	//初始化老鼠配置
	this->InitGameMouseStructManager();


	//初始化->争对Windows平台UI
	if (UGameplayStatics::GetPlatformName().Equals(TEXT("Windows")))
	{
		this->M_Windows_ShowUI_ItemTitleTip = CreateWidget<UUI_ItemTitleTip>(this->GetWorld(), LoadClass<UUI_ItemTitleTip>(0, TEXT("WidgetBlueprint'/Game/Resource/BP/Game/UI/UI_Tip/BPUI_ItemTitleTip.BPUI_ItemTitleTip_C'")));
	}
}

void UFVMGameInstance::Shutdown()
{
	Super::Shutdown();

	//卡片数据卸载
	if (UFVMGameInstance::GetDebug())
		UE_LOG(LogTemp, Log, TEXT("卸载数据表!"));
	UGlobalDatas::UnLoadCardsDatasLoader();

	//判断地图UI是否存在
	if (this->M_CurrentMap)
	{
		this->M_CurrentMap->RemoveFromParent();
	}

	//玩家的主要UI界面显示
	if (this->M_MainUIFrame)
	{
		this->M_MainUIFrame->RemoveFromParent();
	}

	//保存配置
	if (this->SaveGameConfigManager(this->M_GameConfigManager))
		UE_LOG(LogTemp, Warning, TEXT("GameDataConfig 数据保存成功"));
}

UGameConfigManager* UFVMGameInstance::CreateGameConfigManager()
{
	//判断文件是否存在
	if (this->IsFileExist("GameDataConfig"))
	{
		UE_LOG(LogTemp, Log, TEXT("加载 GameDataConfig"));
		//加载并且返回
		return Cast<UGameConfigManager>(UGameplayStatics::LoadGameFromSlot("GameDataConfig", 0));
	}

	//文件不存在创建SaveGame实例
	UGameConfigManager* _UGameConfigManager = Cast<UGameConfigManager>(UGameplayStatics::CreateSaveGameObject(LoadClass<UGameConfigManager>(0, TEXT("Class'/Script/FVM.GameConfigManager'"))));
	//保存
	this->SaveDataToGameConfigManager(_UGameConfigManager);
	//返回
	return _UGameConfigManager;
}

bool UFVMGameInstance::SaveGameConfigManager(UGameConfigManager* const _ConfigInstance)
{
	return UGameplayStatics::SaveGameToSlot(_ConfigInstance, "GameDataConfig", 0);
}

void UFVMGameInstance::InitPlayerStructManager()
{
	//创建一个默认的角色结构管理实例对象并且保存
	if (!this->IsFileExist("DefPlayerStructManager"))
	{
		this->M_UPlayerStructManager = Cast<UPlayerStructManager>(UGameplayStatics::CreateSaveGameObject(LoadClass<UPlayerStructManager>(0, TEXT("Class'/Script/FVM.PlayerStructManager'"))));
		UGameplayStatics::SaveGameToSlot(this->M_UPlayerStructManager, "DefPlayerStructManager", 0);
		UE_LOG(LogTemp, Log, TEXT("保存默认角色结构管理器并加载"));
	}
	else {
		if (!this->M_GameConfigManager->M_CurrentPlayerName.Equals(""))
		{
			this->LoadPlayerStructConfig(this->M_GameConfigManager->M_CurrentPlayerName);
			UE_LOG(LogTemp, Log, TEXT("加载%s角色结构管理器"), *this->M_GameConfigManager->M_CurrentPlayerName);
		}
		else {
			//如果玩家没有建立任何存档则使用默认的存档
			this->M_UPlayerStructManager = Cast<UPlayerStructManager>(this->LoadSaveGame("DefPlayerStructManager"));
			UE_LOG(LogTemp, Log, TEXT("加载默认角色结构管理器"));
		}
	}
}

void UFVMGameInstance::InitGameMapConfigManager()
{
	//保存一个默认的地图管理(前提是默认配置不存在)
	if (!this->IsFileExist("DefGameMapConfig_GameMap"))
	{
		this->CreateGameMapStructManager("DefGameMapConfig");
		//初始化默认地图基本数据
		this->M_UGameMapStructManager->GameMapStruct.M_FirstRowMesheLocation = FVector2D(-135.f, 260.f);

		if (UGameplayStatics::SaveGameToSlot(this->M_UGameMapStructManager, "DefGameMapConfig_GameMap", 0))
		{
			UE_LOG(LogTemp, Log, TEXT("保存默认地图配置管理器"));
		}
	}
	else {
		//加载地图
		this->LoadGameMapStructManager("DefGameMapConfig");

		if (this->GetGameMapStructManager())
			UE_LOG(LogTemp, Log, TEXT("加载默认地图配置管理器"));
	}
}

void UFVMGameInstance::InitGameMouseStructManager()
{
	if (!this->IsFileExist("DefGameMouseStructConfig_MouseRound"))
	{
		this->CreateMouseStructManager(FString("DefGameMouseStructConfig"));

		if (UGameplayStatics::SaveGameToSlot(this->M_MouseStructManager, "DefGameMouseStructConfig_MouseRound", 0))
		{
			UE_LOG(LogTemp, Log, TEXT("保存默认老鼠配置管理器"));
		}
	}
	else {
		this->LoadMouseStructManager("DefGameMouseStructConfig");
		if (this->GetMouseStructManager())
			UE_LOG(LogTemp, Log, TEXT("加载默认老鼠配置管理器"));
	}
}

FString UFVMGameInstance::GetGameVersion()
{
	return UFVMGameInstance::GetFVMGameInstance()->GetGameConfigManager()->M_ConfigVersion;
}

UUI_MainFramework* UFVMGameInstance::GetMainFramework()
{
	return this->M_MainUIFrame;
}

void UFVMGameInstance::SetUpdateGame()
{
	UFVMGameInstance::GetFVMGameInstance()->M_bGameUpdate = true;
}

void UFVMGameInstance::SetMainFramework(UUI_MainFramework* _UUI_MainFramework)
{
	if (!this->M_MainUIFrame)
		this->M_MainUIFrame = _UUI_MainFramework;

	this->M_MainUIFrame->AddToViewport();
}

void UFVMGameInstance::SetGameDebug(bool _Value)
{
	UFVMGameInstance::M_bShowDebugText = _Value;
}

bool UFVMGameInstance::GetDebug()
{
	return UFVMGameInstance::M_bShowDebugText;
}

void UFVMGameInstance::SetGameVersion(FString _Version)
{
	UFVMGameInstance::GetFVMGameInstance()->GetGameConfigManager()->M_ConfigVersion = _Version;
}

void UFVMGameInstance::SetWorldMapWidget(UWidgetBase* _CurrentMap)
{
	if (_CurrentMap && this->M_CurrentMap)
		this->M_CurrentMap->RemoveFromParent();

	this->M_CurrentMap = _CurrentMap;

	if (this->M_CurrentMap)
		this->M_CurrentMap->AddToViewport();
}

bool UFVMGameInstance::GetUpdateGame()
{
	return UFVMGameInstance::GetFVMGameInstance()->M_bGameUpdate;
}

UWidgetBase* UFVMGameInstance::GetWorldMap()
{
	return this->M_CurrentMap;
}

void UFVMGameInstance::UpdateGameData()
{
	//获取存档
	UPlayerStructManager* PlayerData = UFVMGameInstance::GetFVMGameInstance()->GetPlayerStructManager();

	//初始化ID
	PlayerData->UpdateEquipID(PlayerData->M_PlayerBagGirdGroup.GetBagCount(0));

	//更新卡片
	for (auto& Item : PlayerData->M_PlayerItems_Card)
	{
		int32 _CardGrade = Item.M_CardGrade;
		if (UCardBaseStruct::SearchCardFromDataTable(Item.ItemName, Item))
		{
			//更新卡片等级
			Item.M_CardGrade = _CardGrade;
		}
	}

	//更新材料
	for (auto& Item : PlayerData->M_PlayerItems_Material)
	{
		int32 _ItemCound = Item.M_Count;
		UMaterialBaseStruct::SearchMaterailFromDataTable(Item.ItemName, Item, true, Item.M_MaterialType);
		//更新材料个数
		Item.M_Count = _ItemCound;
	}

	//更新装备
	for (auto& Item : PlayerData->M_PlayerItems_Equipment)
	{
		int32 _ItemCount = Item.M_IsOverlap ? Item.M_Count : 0;
		bool _Use = Item.M_Used;
		bool _WaitRemove = Item.bWaitRemove;
		int32 _ID = Item.M_ItemID;

		//搜索成功的更新
		if (UEquipmentBaseStruct::SearchEquipmentFromDataTable(Item.ItemName, Item))
		{
			//更新材料个数
			Item.M_Count = _ItemCount;
			//更新使用情况
			Item.M_Used = _Use;
			//是否等待删除
			Item.bWaitRemove = _WaitRemove;
			//赋予ID号
			Item.M_ItemID = _ID;

			if (UFVMGameInstance::GetDebug())
			{
				UGameSystemFunction::FVMLog(__FUNCTION__, FString(TEXT("装备道具：") + Item.ItemName + TEXT("ID：") + FString::FromInt(Item.M_ItemID)));
			}
		}
		else {
			//未查询的 标记(判断名称是否是：煮蛋器投手)【版本降低的可以忽略】
			if (UFVMGameInstance::GetDebug())
			{
				UGameSystemFunction::FVMLog(__FUNCTION__, FString(TEXT("当前装备道具-查询失败：") + Item.ItemName + TEXT("ID：") + FString::FromInt(_ID)));
			}

			//注销ID
			PlayerData->RemoveEquipmentFromPlayerBag(Item);
		}

	}

	//清理掉等待删除的物品
	UGameSystemFunction::ClearWaitingItemsForEquip(PlayerData->M_PlayerItems_Equipment);

	//技能书更新
	UFVMGameInstance::UpdatePlayerSkillBooksDataList();

	PlayerData->M_GameVersion = UFVMGameInstance::GetGameVersion();

	UFVMGameInstance::GetFVMGameInstance()->M_bGameUpdate = false;

	UGameSystemFunction::SaveCurrentPlayerData();
	UGameSystemFunction::LoadCurrentPlayerData();
}

bool UFVMGameInstance::UpdateConfigData()
{
	//版本相同则false
	if (UFVMGameInstance::GetFVMGameInstance()->GetGameConfigManager()->M_ConfigVersion.
		Equals(UFVMGameInstance::GetFVMGameInstance()->GetGameConfigManager()->M_LastConfigVersion))
	{
		return false;
	}

	UFVMGameInstance::GetFVMGameInstance()->GetGameConfigManager()->M_LastConfigVersion = UFVMGameInstance::GetFVMGameInstance()->GetGameConfigManager()->M_ConfigVersion;

	UGameSystemFunction::SaveCurrentGameConfigData();

	return true;
}

UGameMapStructManager* UFVMGameInstance::LoadGameMapStructManager(const FString& _FileName)
{
	UDataTable* GameMapData = LoadObject<UDataTable>(0, TEXT("DataTable'/Game/Resource/BP/Data/MapData/GameMapData.GameMapData'"));
	TArray<FGameMapData*> M_GameMapData;
	GameMapData->GetAllRows("GameMapList", M_GameMapData);

	for (auto Datas : M_GameMapData)
	{
		//从数据表寻找指定的地图名称
		if ((*Datas).M_FLevelConfig.LevelName.Equals(_FileName))
		{
			//创建新的地图结构并且将数据表的数据赋予给新创建的地图结构
			this->M_UGameMapStructManager = this->CreateGameMapStructManager(_FileName);
			this->M_UGameMapStructManager->GameMapStruct = (*Datas).M_FGameMapStruct;
			this->M_UGameMapStructManager->LevelConfig = (*Datas).M_FLevelConfig;

			//创建新的老鼠结构并且把数据表的数据赋予给新的老鼠结构
			this->M_MouseStructManager = this->CreateMouseStructManager(_FileName);

			//设置老鼠配置
			this->M_MouseStructManager->SetMouseConfig((*Datas).M_FMouseConfig);
			//设置行数量
			this->M_MouseStructManager->SetRowCount(this->M_UGameMapStructManager->GameMapStruct.M_Meshe.Num());

			//保存地图数据
			UGameplayStatics::SaveGameToSlot(this->M_MouseStructManager, FString(_FileName + FString("_MouseRound")), 0);

			//返回数据
			return this->M_UGameMapStructManager;
		}
	}

	if (UFVMGameInstance::GetDebug())
		UE_LOG(LogTemp, Log, TEXT("地图配置管理器加载失败!"));

	return nullptr;
}

UGameMapStructManager* UFVMGameInstance::CreateGameMapStructManager(const FString& _FileName)
{
	this->M_UGameMapStructManager = Cast<UGameMapStructManager>(UGameplayStatics::CreateSaveGameObject(LoadClass<UGameMapStructManager>(0, TEXT("Class'/Script/FVM.GameMapStructManager'"))));
	return this->M_UGameMapStructManager;
}

UMouseStructManager* UFVMGameInstance::CreateMouseStructManager(const FString& _FileName)
{
	this->M_MouseStructManager = Cast<UMouseStructManager>(UGameplayStatics::CreateSaveGameObject(LoadClass<UMouseStructManager>(0, TEXT("Class'/Script/FVM.MouseStructManager'"))));
	UGameplayStatics::SaveGameToSlot(this->M_MouseStructManager, *(_FileName + FString(TEXT("_MouseRound"))), 0);
	return this->M_MouseStructManager;
}

UMouseStructManager* UFVMGameInstance::LoadMouseStructManager(const FString& _FileName)
{
	//加载了配置之后自动将原来的配置指向新的配置
	UMouseStructManager* _NewLoad = Cast<UMouseStructManager>(this->LoadSaveGame(_FileName + FString(TEXT("_MouseRound"))));

	if (_NewLoad)
	{
		this->M_MouseStructManager = _NewLoad;
		return _NewLoad;
	}

	if (UFVMGameInstance::GetDebug())
		UE_LOG(LogTemp, Log, TEXT("老鼠配置管理器加载失败!"));

	return nullptr;
}

bool UFVMGameInstance::LoadPlayerStructConfig(const FString& _FileName)
{
	UPlayerStructManager* PlayerInstance = Cast<UPlayerStructManager>(LoadSaveGame(_FileName + FString("P")));

	if (PlayerInstance)
	{
		this->M_UPlayerStructManager = PlayerInstance;
		return true;
	}

	return false;
}

UPlayerStructManager* UFVMGameInstance::LoadTempPlayerStructConfig(const FString& _FileName)
{
	return  Cast<UPlayerStructManager>(UFVMGameInstance::GetFVMGameInstance()->LoadSaveGame(_FileName + FString("P")));
}

UGameConfigManager* UFVMGameInstance::GetGameConfigManager()
{
	return this->M_GameConfigManager;
}

UGameMapStructManager* UFVMGameInstance::GetGameMapStructManager()
{
	return this->M_UGameMapStructManager;
}

UPlayerStructManager* UFVMGameInstance::GetPlayerStructManager()
{
	return this->M_UPlayerStructManager;
}

UGameConfigManager* UFVMGameInstance::LoadSaveDataToGameConfigManager()
{
	//获取游戏配置文件
	this->M_GameConfigManager = this->CreateGameConfigManager();

	//设置游戏FPS配置
	if (UGameUserSettings* LUserSetting = GEngine->GetGameUserSettings())
	{
		LUserSetting->SetFrameRateLimit(this->M_GameConfigManager->M_GameFPS.M_FPS_Max);
		LUserSetting->ApplySettings(true);
	}

	return this->M_GameConfigManager;
}

void UFVMGameInstance::SaveDataToGameConfigManager(UGameConfigManager* _UGameConfigManager)
{
	if (_UGameConfigManager)
	{
		//保存数据
		if (this->SaveGameConfigManager(_UGameConfigManager))
		{
			if (UFVMGameInstance::GetDebug())
				UE_LOG(LogTemp, Warning, TEXT("GameDataConfig 数据保存成功"));
		}
	}
	else if (this->M_GameConfigManager)
	{
		//保存数据
		if (this->SaveGameConfigManager(this->M_GameConfigManager))
		{
			if (UFVMGameInstance::GetDebug())
				UE_LOG(LogTemp, Warning, TEXT("GameDataConfig 数据保存成功"));
		}
	}
}

void UFVMGameInstance::CreateBaseTipWidget(const FString& _ShowTitle, bool _bShow)
{
	UUI_ItemTitleTip* const UITitle = UFVMGameInstance::GetFVMGameInstance()->GetUI_ItemTitleTip();
	if (IsValid(UITitle))
	{
		if (_bShow)
		{
			//设置标题
			UITitle->SetShow(true, _ShowTitle);
			//获取鼠标位置
			FVector2D LPosition = UWidgetLayoutLibrary::GetMousePositionOnViewport(UFVMGameInstance::GetFVMGameInstance()->GetWorld());
			//设置缩放
			LPosition *= UWidgetLayoutLibrary::GetViewportScale(UFVMGameInstance::GetFVMGameInstance()->GetWorld());
			//设置最终位置
			UITitle->SetPositionInViewport(LPosition, true);
		}
		else {
			UITitle->SetShow(false, TEXT(""));
		}
	}
}

bool UFVMGameInstance::GetGameConfig_GameEFEnabled()
{
	return UFVMGameInstance::GetFVMGameInstance()->GetGameConfigManager()->M_GameEF.M_bEnable;
}

void UFVMGameInstance::CreatePlayerStructConfig(const FString& _PlayerDataName)
{
	this->M_UPlayerStructManager = Cast<UPlayerStructManager>(UGameplayStatics::CreateSaveGameObject(LoadClass<UPlayerStructManager>(0, TEXT("Class'/Script/FVM.PlayerStructManager'"))));
	this->SavePlayerStructConfig(_PlayerDataName);
}

void UFVMGameInstance::SavePlayerStructConfig(const FString& _PlayerDataName)
{
	UGameplayStatics::SaveGameToSlot(this->M_UPlayerStructManager, _PlayerDataName + FString("P"), 0);
}

void UFVMGameInstance::DeletePlayerStructConfig(const FString& _PlayerDataName)
{
	UGameplayStatics::DeleteGameInSlot(_PlayerDataName + FString("P"), 0);
}

void UFVMGameInstance::UpdatePlayerSkillBooksDataList()
{
	//获取存档
	UPlayerStructManager* PlayerData = UFVMGameInstance::GetFVMGameInstance()->GetPlayerStructManager();

	//加载技能书数据表(缺少多少项添加多少项)
	TArray<FSkillBookData_Data*> Datas;
	UGameSystemFunction::GetDataTableRows<FSkillBookData_Data>(TEXT("DataTable'/Game/Resource/BP/Data/CardSkillBookData/CardSkillBookData.CardSkillBookData'"), Datas);

	//判断背包中的技能书个数是否匹配数据表个数
	if (PlayerData->M_SkillBookDatas.Num() != Datas.Num())
	{
		if (UFVMGameInstance::GetDebug())
			UE_LOG(LogTemp, Warning, TEXT("技能书数量不匹配->更新技能书"));

		int32 Items = Datas.Num();
		for (int32 i = PlayerData->M_SkillBookDatas.Num(); i < Items; i++)
		{
			PlayerData->M_SkillBookDatas.Emplace(Datas[i]->M_FSkillBookData);
		}

		//更新数据
		int32 LNums = PlayerData->M_SkillBookDatas.Num();
		for (int32 i = 0; i < LNums; i++)
		{
			//保存等级和当前经验值
			int32 Level = PlayerData->M_SkillBookDatas[i].M_LV;
			int32 CurrentEx = PlayerData->M_SkillBookDatas[i].M_CurrentEx;
			int32 CurrentTotalEx = PlayerData->M_SkillBookDatas[i].M_CurrentTotalEx;
			//重新更新数据
			PlayerData->M_SkillBookDatas[i] = Datas[i]->M_FSkillBookData;
			//重新赋予数据值
			PlayerData->M_SkillBookDatas[i].M_LV = Level;
			PlayerData->M_SkillBookDatas[i].M_CurrentEx = CurrentEx;
			PlayerData->M_SkillBookDatas[i].M_CurrentTotalEx = CurrentTotalEx;
		}

		if (UFVMGameInstance::GetDebug() && PlayerData->M_SkillBookDatas.Num() == Datas.Num())
			UE_LOG(LogTemp, Warning, TEXT("技能书更新完成"));


		//更新存档
		UGameSystemFunction::SaveCurrentPlayerData();
		UGameSystemFunction::LoadCurrentPlayerData();
	}

}

void UFVMGameInstance::PlayBGM(const FSoftObjectPath& _MusicName, bool _Loop)
{
	//如果组件存在那么先停止播放
	if (this->M_UUAudioComponent)
		this->M_UUAudioComponent->Stop();

	if (_MusicName.ToString().Equals(""))
		return;

	USoundBase* Sound = LoadObject<USoundBase>(0, *_MusicName.ToString());

	if (!Sound)
		return;

	Cast<USoundWave>(Sound)->bLooping = _Loop;
	this->M_UUAudioComponent = UGameplayStatics::SpawnSound2D(this, Sound, this->M_GameConfigManager->M_FMusic.M_BGM_Value, 1.f, 0.0f, 0, false, false);
	this->UpdateBGMVolume();
}

void UFVMGameInstance::PlayBGM_S(const FString& _MusicName, const FString& _MusicPathName)
{
	if (!this->M_GameConfigManager)
		return;

	if (_MusicName.Equals(""))
		return;

	const FString& LSoundPath = FString("SoundWave'/Game/Resource/BP/Audio/") + _MusicPathName + ("/") + _MusicName + FString(".") + _MusicName + FString("'");

	USoundBase* Sound = LoadObject<USoundBase>(0, *LSoundPath);

	if (!Sound)
		return;

	Cast<USoundWave>(Sound)->bLooping = false;

	UGameplayStatics::PlaySound2D(this, Sound, this->M_GameConfigManager->M_FMusic.M_BGM_S_Value);
}

void UFVMGameInstance::UpdateBGMVolume()
{
	//将BMG本身的Volume * 指定Volume大小   [  0 - 1 * BGM(size)  ]
	if (this->M_UUAudioComponent && this->M_GameConfigManager)
		this->M_UUAudioComponent->SetVolumeMultiplier(this->M_GameConfigManager->M_FMusic.M_BGM_Value);
}

UMouseStructManager* UFVMGameInstance::GetMouseStructManager()
{
	return this->M_MouseStructManager;
}

UAudioComponent* UFVMGameInstance::GetAudioComponent()
{
	return this->M_UUAudioComponent;
}

UPlayerStructManager* UFVMGameInstance::GetPlayerStructManager_Static()
{
	if (IsValid(UFVMGameInstance::GetFVMGameInstance()))
		return UFVMGameInstance::GetFVMGameInstance()->GetPlayerStructManager();
	return nullptr;
}

void UFVMGameInstance::PrintGameMapStruct(UGameMapStructManager* _UGameMapStructManager)
{
}

void UFVMGameInstance::PrintGameMouseStruct(UMouseStructManager* _UMouseStructManager)
{
	/*
	if (_UMouseStructManager && UFVMGameInstance::GetDebug())
	{
		UE_LOG(LogTemp, Warning, TEXT("-----------------------------------UMouseStructManager-----------------------------------"));
		UE_LOG(LogTemp, Warning, TEXT("一共%d个回合"), _UMouseStructManager->M_MouseRound.Num());

		int32 Index = 0;
		//遍历所有回合
		for (auto& Round : _UMouseStructManager->M_MouseRound)
		{
			Index++;
			UE_LOG(LogTemp, Error, TEXT("-"));
			UE_LOG(LogTemp, Warning, TEXT("-----------------------------------第%d回合-----------------------------------"), Index);
			UE_LOG(LogTemp, Warning, TEXT("1.当所有老鼠被消灭之后自动进行下一回合:%s"), Round.M_IsEnableMouseCountZero ? TEXT("开启") : TEXT("关闭"));
			UE_LOG(LogTemp, Warning, TEXT("2.当前回合老鼠细节个数(用于加载的老鼠):%d"), Round.M_MouseLineSetting.Num());
			UE_LOG(LogTemp, Warning, TEXT("3.当前回合一共有多少只老鼠:%d"), Round.M_MouseMax);
			UE_LOG(LogTemp, Warning, TEXT("4.每次生成的老鼠(最小:%d个,最大%d个)"), Round.M_MouseMinCount, Round.M_MouseMaxCount);
			UE_LOG(LogTemp, Warning, TEXT("5.自动进入下一个回合的时间:%.2f秒,(前提是你关闭了第一项)"), Round.M_NextRoundTime);
			UE_LOG(LogTemp, Warning, TEXT("6.(%.2f秒)之后开始初始化当前回合(BeginPlay)"), Round.M_RoundBeginDelay);
			UE_LOG(LogTemp, Warning, TEXT("7.(%.2f秒)之后开始执行当前回合结束(End)"), Round.M_RoundEndDelay);
			UE_LOG(LogTemp, Warning, TEXT("8.回合一开始可以生成老鼠的最大值:%d"), Round.M_RoundBeginSpawnMaxCount);
			UE_LOG(LogTemp, Warning, TEXT("-----------------------------------第%d回合-----------------------------------"), Index);
			UE_LOG(LogTemp, Error, TEXT("-"));
		}

		UE_LOG(LogTemp, Warning, TEXT("-----------------------------------UMouseStructManager-----------------------------------"));
	}
	*/
}

UFVMGameInstance* UFVMGameInstance::GetFVMGameInstance()
{
	return UFVMGameInstance::M_StaticUFVMGameInstance;
}

void UFVMGameInstance::CreateMainFramePanel()
{
	if (UFVMGameInstance::GetFVMGameInstance()->GetMainFramework())
	{
		UFVMGameInstance::GetFVMGameInstance()->GetMainFramework()->RemoveFromParent();

		UFVMGameInstance::GetFVMGameInstance()->GetMainFramework()->AddToViewport();
	}
	else {
		UFVMGameInstance::M_StaticUFVMGameInstance->SetMainFramework(CreateWidget<UUI_MainFramework>(UFVMGameInstance::GetFVMGameInstance()->GetWorld(), LoadClass<UUI_MainFramework>(0, TEXT("WidgetBlueprint'/Game/Resource/BP/Game/UI/BPUI_MainFrame.BPUI_MainFrame_C'"))));
	}
}


void UFVMGameInstance::PlayBGM_Static(const FSoftObjectPath& _MusicName, bool _Loop)
{
	if (UFVMGameInstance::GetFVMGameInstance())
	{
		UFVMGameInstance::GetFVMGameInstance()->PlayBGM(_MusicName, _Loop);
	}
}

void UFVMGameInstance::PlayBGM_S_Static(const FString& _MusicName, const FString& _MusicPath)
{
	if (UFVMGameInstance::GetFVMGameInstance())
	{
		UFVMGameInstance::GetFVMGameInstance()->PlayBGM_S(_MusicName, _MusicPath);
	}
}

USaveGame* UFVMGameInstance::LoadSaveGame(const FString& _FileName)
{
	return UGameplayStatics::LoadGameFromSlot(_FileName, 0);
}

void UFVMGameInstance::SaveGame(USaveGame* SaveGameObject, const FString& SlotName)
{
	UGameplayStatics::SaveGameToSlot(SaveGameObject, SlotName, 0);
}

bool UFVMGameInstance::IsFileExist(const FString& _FileName)
{
	return UGameplayStatics::DoesSaveGameExist(_FileName, 0);
}
