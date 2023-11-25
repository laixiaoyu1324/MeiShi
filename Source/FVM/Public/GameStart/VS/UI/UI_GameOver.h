// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetBase.h"
#include "UI_GameOver.generated.h"

/**
 *
 */

class UUniformGridPanel;
class UTextBlock;
class UImage;


//掉落物-物品
UCLASS()
class FVM_API UUI_LevelItem : public UWidgetBase
{
	GENERATED_BODY()

public:

	virtual bool Initialize() override;

	void Init(const FSoftObjectPath& _path, const int32& Count);

private:

	//物品的头像
	UPROPERTY()
		UImage* ItemHead = nullptr;

	//物品的数量文本
	UPROPERTY()
		UTextBlock* ItemCountText = nullptr;

};

class UResourceManagerComponent;

UCLASS()
class FVM_API UUI_GameOver : public UWidgetBase
{
	GENERATED_BODY()

	friend class UResourceManagerComponent;

public:

	virtual bool Initialize() override;

public:

	//显示界面1的数据
	UFUNCTION()
		void ShowOver1();

	//显示界面2的数据
	UFUNCTION()
		void ShowOver2(bool bWin);

	//显示界面3的数据
	UFUNCTION()
		void ShowOver3();

private://界面1

	//金币提示
	UPROPERTY()
		UTextBlock* CoinText = nullptr;

	//获取的物品奖励
	UPROPERTY()
		UUniformGridPanel* LevelItemsPanel = nullptr;

private://界面2

	//分数提示
	UPROPERTY()
		UTextBlock* ScoreText = nullptr;
	//分数等级提示
	UPROPERTY()
		UTextBlock* ScoreLevelText = nullptr;
	//时间提示
	UPROPERTY()
		UTextBlock* TimeText = nullptr;
	//卡片损坏提示
	UPROPERTY()
		UTextBlock* BadCardCountText = nullptr;
	//经验提示
	UPROPERTY()
		UTextBlock* ExText = nullptr;
	//角色等级图片
	UPROPERTY()
		UImage* PlayerGradeHead = nullptr;
};
