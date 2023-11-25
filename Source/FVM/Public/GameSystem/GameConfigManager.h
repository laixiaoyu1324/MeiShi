// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "GameSystem/Item/ItemStruct.h"
#include "GameConfigManager.generated.h"


//特殊音效选项
UENUM(BlueprintType)
enum class EConfig_Music_SpecialAudio : uint8 {
	EEAudio_1 UMETA(DisplayName = "Audio1"),
	EEAudio_2 UMETA(DisplayName = "Audio2")
};

//音乐音量
USTRUCT(BlueprintType)
struct FMusic {
	GENERATED_USTRUCT_BODY()
public:
	//音量
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float M_BGM_Value = 1.f;
	//音量
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float M_BGM_S_Value = 1.f;
	//音效名称
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString M_Operating_BGM = FString("Def_Click");
	//启动特殊音效
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool M_bEnableSpecialAudio = false;
	//特殊音效选择
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		EConfig_Music_SpecialAudio M_Music_SpecialAudio = EConfig_Music_SpecialAudio::EEAudio_1;
};

//游戏帧率
USTRUCT(BlueprintType)
struct FGameFPS {
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float M_FPS_Max = 60.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 M_State_Index = 2;
};

//游戏渲染自动曝光
USTRUCT(BlueprintType)
struct FGameRender {
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float M_Rander_Max = 0.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 M_State_Index = 0;
	//自定义
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool M_Custom = false;
};


//游戏主菜单设置
USTRUCT(BlueprintType)
struct FGameInGameSetting {
	GENERATED_USTRUCT_BODY()
public:
	//使用新的角色选择界面
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool M_bUseNewPlayerCreatingPanel = true;
	//使用低配模式启动
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool M_bUseLowModelInGame = false;
};

//游戏屏幕比例
USTRUCT(BlueprintType)
struct FGameScreenScale {
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float M_X = 1.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float M_Y = 1.f;
};

//游戏特效
USTRUCT(BlueprintType)
struct FGameEF {
	GENERATED_USTRUCT_BODY()
public:
	//开启特效
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool M_bEnable = false;
};

//游戏的界面框架配置
USTRUCT(BlueprintType)
struct FGameUI_MainFrameConfig {
	GENERATED_USTRUCT_BODY()
public:
	//开启或者关闭->活动界面（默认开启）
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool M_bGameActivityEnable = true;
	//关闭或者开启->存档列表（默认开启）
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool M_bGameDataListEnable = false;
};

UCLASS()
class FVM_API UGameConfigManager : public USaveGame
{
	GENERATED_BODY()
public:
	//上一次版本配置
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameConfig")
		FString M_LastConfigVersion;
	//配置版本
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameConfig")
		FString M_ConfigVersion;
	//角色所有保存的存档名称
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameConfig")
		TArray<FString> M_PlayerStructManagerName;
	//角色的性别
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameConfig")
		TArray<uint8> M_PlayerStructManagerSex;
	//角色准备环节选择的卡片数据
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameConfig")
		TArray<FItemCard> M_PlayerGamePrepareSelectCardData;
	//角色进入地图的名称
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameConfig")
		FString M_PlayerInGameMapName;
	//当前选择的角色
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameConfig")
		FString M_CurrentPlayerName = FString("");
	//音乐音效 声音大小
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameConfig")
		FMusic M_FMusic;
	//游戏帧率的最大值(默认600)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameConfig")
		FGameFPS M_GameFPS;
	//游戏特效
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameConfig")
		FGameEF M_GameEF;
	//游戏屏幕比例
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameConfig")
		FGameScreenScale M_GameScreenScale;
	//游戏UI主框架配置
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameConfig")
		FGameUI_MainFrameConfig M_FGameUI_MainFrameConfig;
	//游戏主菜单设置
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameConfig")
		FGameInGameSetting M_FGameInGameSetting;
public:
	//获取游戏配置管理器
	UFUNCTION(BlueprintCallable)
		static UGameConfigManager* GetGameConfigManager();
};
