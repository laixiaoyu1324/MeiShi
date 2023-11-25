// Fill out your copyright notice in the Description page of Project Settings.


#include "GameStart/VS/UI/UI_GameOver.h"
#include "GameStart/VS/Components/ResourceManagerComponent.h"
#include "GameSystem/GameMapStructManager.h"
#include "GameSystem/FVMGameInstance.h"
#include "GameSystem/Tools/GameSystemFunction.h"
#include <Components/TextBlock.h>
#include <Components/UniformGridPanel.h>
#include <Components/Image.h>

bool UUI_GameOver::Initialize()
{
	Super::Initialize();

	this->CoinText = this->GetWidgetComponent<UTextBlock>(this, TEXT("CoinText"));
	this->LevelItemsPanel = this->GetWidgetComponent<UUniformGridPanel>(this, TEXT("UniformGridPanel_65"));

	this->ScoreText = this->GetWidgetComponent<UTextBlock>(this, TEXT("ScoreText"));
	this->ScoreLevelText = this->GetWidgetComponent<UTextBlock>(this, TEXT("ScoreAText"));
	this->TimeText = this->GetWidgetComponent<UTextBlock>(this, TEXT("TimeText"));
	this->BadCardCountText = this->GetWidgetComponent<UTextBlock>(this, TEXT("BadCardCountText"));
	this->ExText = this->GetWidgetComponent<UTextBlock>(this, TEXT("ExText"));
	this->PlayerGradeHead = this->GetWidgetComponent<UImage>(this, TEXT("GradeHead"));


	return true;
}

void UUI_GameOver::ShowOver1()
{
	const TMap<FString, int32>& CurResource = UResourceManagerComponent::GetResource();
	//获得关卡配置
	const FLevelConfig& LevelConfig = UFVMGameInstance::GetFVMGameInstance()->GetGameMapStructManager()->LevelConfig;


	TMap<FString, int32> Coins;
	Coins.Emplace(TEXT("金币"), 0);
	Coins.Emplace(TEXT("礼券"), 1);
	Coins.Emplace(TEXT("点券"), 2);
	Coins.Emplace(TEXT("威威"), 3);

	int32 i = 0;
	for (const auto& Item : CurResource)
	{
		const FSoftObjectPath* CurPath = LevelConfig.LevelItems.Find(Item.Key);
		if (CurPath)
		{
			UUI_LevelItem* LevelItem = CreateWidget<UUI_LevelItem>(this, LoadClass<UUI_LevelItem>(nullptr,
				TEXT("WidgetBlueprint'/Game/Resource/BP/GameStart/VS/UI_Player/BPUI_LevelItem.BPUI_LevelItem_C'")));

			LevelItem->Init(*CurPath, Item.Value);

			this->LevelItemsPanel->AddChildToUniformGrid(LevelItem, i / 16, i % 16);
		}
		else {
			if (Item.Key.Equals(TEXT("金币")))
			{
				CoinText->SetText(FText::FromString(FString::FromInt(Item.Value)));
				continue;
			}
			else
			{
				if (UFVMGameInstance::GetDebug())
				{
					UGameSystemFunction::FVMLog(__FUNCTION__, Item.Key + TEXT("掉落物不存在，请检查关卡配置!!"));
				}
			}
		}


		//发送道具[材料]
		if (!UGameSystemFunction::SendMaterialToPlayerBag(Item.Key, Item.Value, false))
		{
			//发送装备
			if (!UGameSystemFunction::SendEquipmentToPlayerBag(Item.Key, Item.Value, false))
			{
				//发送货币
				const int32* CoinIndex = Coins.Find(Item.Key);
				if (CoinIndex)
				{
					UFVMGameInstance::GetPlayerStructManager_Static()->AddCoin(Item.Value, *CoinIndex);
				}
				else {
					if (UFVMGameInstance::GetDebug())
					{
						UGameSystemFunction::FVMLog(__FUNCTION__, Item.Key + TEXT("掉落物不存在，请检查关卡配置!!"));
					}
				}
			}
		}

		i++;
	}

	UGameSystemFunction::SaveCurrentPlayerData();
}

void UUI_GameOver::ShowOver2(bool bWin)
{
	UResourceManagerComponent* Manager = UResourceManagerComponent::GetResourceManagerComponent();
	this->ScoreText->SetText(FText::FromString(FString::FromInt(Manager->CurScore)));
	this->TimeText->SetText(FText::FromString(FString::FromInt(Manager->CurLevelTime)));
	this->BadCardCountText->SetText(FText::FromString(FString::FromInt(Manager->CurBadCardCount)));
	this->ExText->SetText(FText::FromString(FString::FromInt(Manager->CurEx * (bWin ? 1.1f : 0.5f)  )));

	//添加角色经验值
	UFVMGameInstance::GetPlayerStructManager_Static()->AddPlayerEx(Manager->CurEx * (bWin ? 1.1f : 0.5f)  );
	//设置角色等级
	FSoftObjectPath PlayerGradePath = UGameSystemFunction::GetPlayerGradeImagePath();
	this->PlayerGradeHead->SetBrushFromTexture(Cast<UTexture2D>(PlayerGradePath.TryLoad()));

	//设置关卡等级
	FPlayerLevelGrade NewLevel;
	NewLevel.Init(Manager->CurLevelTime, Manager->CurScore, Manager->CurBadCardCount);
	//初始化并初始化地图评级
	this->ScoreLevelText->SetText(FText::FromString(NewLevel.InitScoreLevel(Manager->CurBadCardCount + (bWin ? 0 : 60)  )));

	//添加地图评级
	UPlayerStructManager::AddPlayerLevelScore(
		UFVMGameInstance::GetFVMGameInstance()->GetGameMapStructManager()->LevelConfig.LevelName
		, NewLevel);

	UGameSystemFunction::SaveCurrentPlayerData();
}

void UUI_GameOver::ShowOver3()
{

}

bool UUI_LevelItem::Initialize()
{
	Super::Initialize();

	this->ItemHead = this->GetWidgetComponent<UImage>(this, TEXT("Head"));
	this->ItemCountText = this->GetWidgetComponent<UTextBlock>(this, TEXT("TextBlock_45"));

	return true;
}

void UUI_LevelItem::Init(const FSoftObjectPath& _path, const int32& Count)
{
	this->ItemHead->SetBrushFromTexture(Cast<UTexture2D>(_path.TryLoad()));
	this->ItemCountText->SetText(FText::FromString(TEXT("x") + FString::FromInt(Count)));
}
