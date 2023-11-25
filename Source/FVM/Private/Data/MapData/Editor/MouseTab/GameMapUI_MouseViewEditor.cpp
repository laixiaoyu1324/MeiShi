// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除


#include "Data/MapData/Editor/MouseTab/GameMapUI_MouseViewEditor.h"
#include "Data/MapData/Editor/MouseTab/MouseViewEditorRoundNodeGrid.h"
#include "Data/MapData/Editor/MouseTab/MouseViewSelectMousePanel.h"
#include "Data/MapData/Editor/MouseTab/GameMapUI_MouseTab.h"

#include "Data/MapData/Editor/FVMEditUI_GameMapEdit.h"

#include <Components/TextBlock.h>
#include <Components/Button.h>
#include <Components/VerticalBox.h>
#include <Components/EditableTextBox.h>
#include <Components/Button.h>
#include <Components/Image.h>
#include <Components/UniformGridPanel.h>
#include <Components/HorizontalBox.h>

bool UGameMapUI_MouseViewEditor::Initialize()
{
	if (!Super::Initialize())
		return false;

	this->MapBg = Cast<UImage>(this->GetWidgetFromName(FName(TEXT("CurMapBg"))));
	this->RoundNodeGridPanel = Cast<UUniformGridPanel>(this->GetWidgetFromName(FName(TEXT("GridPanel"))));

	this->Text_TotalRoundCount = Cast<UTextBlock>(this->GetWidgetFromName(FName(TEXT("TotalRoundCount"))));
	this->Text_CurRoundIndex = Cast<UTextBlock>(this->GetWidgetFromName(FName(TEXT("CurRoundIndex"))));
	this->Text_TotalRoundNodeCount = Cast<UTextBlock>(this->GetWidgetFromName(FName(TEXT("TotalRoundNodeCount"))));
	this->Text_CurRoundNodeIndex = Cast<UTextBlock>(this->GetWidgetFromName(FName(TEXT("CurRoundNodeIndex"))));

	this->LevelItemsGridPanel = Cast<UUniformGridPanel>(this->GetWidgetFromName(FName(TEXT("UniformGridPanel_94"))));
	this->CurShowLevelItemListPanel = Cast<UHorizontalBox>(this->GetWidgetFromName(FName(TEXT("HorizontalBox_164"))));

	return true;
}

void UGameMapUI_MouseViewEditor::InitMouseViewEditor(UFVMEditUI_GameMapEdit* Class, UGameMapUI_MouseTab* OwnerClass)
{
	this->FVMEditUI_GameMapEdit = Class;
	this->GameMapUI_MouseTab = OwnerClass;

	this->MouseViewSelectMousePanel = Cast<UMouseViewSelectMousePanel>(this->GetWidgetFromName(FName(TEXT("UMouseViewSelectMousePanel"))));

	//初始化格子节点
	this->InitRoundNodeGrid();
	//初始化老鼠选择项
	this->MouseViewSelectMousePanel->InitMouseViewSelectMousePanel();
	//初始化掉落物选项
	this->InitLevelItems();
}

UFVMEditUI_GameMapEdit* UGameMapUI_MouseViewEditor::GetEditor()
{
	return this->FVMEditUI_GameMapEdit;
}

UGameMapUI_MouseTab* UGameMapUI_MouseViewEditor::GetMouseTab()
{
	return this->GameMapUI_MouseTab;
}

void UGameMapUI_MouseViewEditor::InitRoundNodeGrid()
{
	//得到地图配置
	const FGameMapStruct& CurGameMapData = this->FVMEditUI_GameMapEdit->GetCurEditData().M_FGameMapStruct;

	//得到行，得到列
	for (int32 Row = 0; Row < CurGameMapData.M_Meshe.Num(); Row++)
	{
		for (int32 Col = 0; Col < 20; Col++)
		{
			UMouseViewEditorRoundNodeGrid* CurGrid = CreateWidget<UMouseViewEditorRoundNodeGrid>(this,
				LoadClass<UMouseViewEditorRoundNodeGrid>(
					nullptr,
					TEXT("WidgetBlueprint'/Game/Resource/BP/Data/MapData/MouseConfig/BP_MouseEditorGrid.BP_MouseEditorGrid_C'")
					));

			//初始化数据
			CurGrid->InitEditorRoundNodeGrid(Row, Col, this);

			this->RoundNodeGridPanel->AddChildToUniformGrid(CurGrid, Row, Col);
		}
	}

	//更新总波数
	this->TotalRound = this->GameMapUI_MouseTab->GetConfigRef().CurRoundConfig.Num();
	this->CurRoundIndex = 0;
	this->TotalRoundNode = 0;
	this->CurRoundNodeIndex = 0;

	//初始化老鼠视图
	if (this->TotalRound == 0)
	{
		this->DisEnableView();
	}
	else {
		//更新节点
		if (this->CurRoundIndex < this->GameMapUI_MouseTab->GetConfigRef().CurRoundConfig.Num())
		{
			this->TotalRoundNode = this->TotalRoundNode = this->GameMapUI_MouseTab->GetConfigRef().CurRoundConfig[this->CurRoundIndex].CurNode.Num();

			if (this->TotalRoundNode == 0)
			{
				this->DisEnableView();
			}
			else {
				//检查回合节点
				this->CheckTimeNodeCount();
				//更新【编辑器视图】
				this->UpdateView();
			}
		}
	}

	this->OnInit();
}

void UGameMapUI_MouseViewEditor::UpdateText()
{
	if (IsValid(this->Text_TotalRoundCount))
	{
		this->Text_TotalRoundCount->SetText(FText::FromString(FString::FromInt(this->TotalRound)));
	}

	if (IsValid(this->Text_CurRoundIndex))
	{
		this->Text_CurRoundIndex->SetText(FText::FromString(FString::FromInt(this->CurRoundIndex + 1)));
	}

	if (IsValid(this->Text_TotalRoundNodeCount))
	{
		this->Text_TotalRoundNodeCount->SetText(FText::FromString(FString::FromInt(this->TotalRoundNode)));
	}

	if (IsValid(this->Text_CurRoundNodeIndex))
	{
		this->Text_CurRoundNodeIndex->SetText(FText::FromString(FString::FromInt(this->CurRoundNodeIndex + 1)));
	}
}

void UGameMapUI_MouseViewEditor::CheckTimeNodeCount()
{
	if (this->TotalRound == 0 || this->TotalRoundNode == 0)
	{
		return;
	}

	//当前回合节点【10】在个中的其中一个
	FTimeNodeWidthRound& CurTimeNode = this->GameMapUI_MouseTab->GetConfigRef().
		CurRoundConfig[
			this->CurRoundIndex
		].CurNode[
			this->CurRoundNodeIndex
		];

			//当前回合节点中的最小子节点个数是否有20个
			if (CurTimeNode.CurNode.Num() < 20)
			{
				const FGameMapStruct& CurGameMapData = this->FVMEditUI_GameMapEdit->GetCurEditData().M_FGameMapStruct;

				//差多少补多少
				int32 Result = 20 - CurTimeNode.CurNode.Num();
				for (int32 i = 0; i < Result; i++)
				{
					FTimeNode CurLocalTimeNode;

					TArray<FMouseConfigNode> CurMouseNodes;
					CurMouseNodes.SetNum(CurGameMapData.M_Meshe.Num());

					CurLocalTimeNode.CurMouseNode = CurMouseNodes;

					CurTimeNode.CurNode.Emplace(CurLocalTimeNode);
				}

				//更新数据表视图
				this->GameMapUI_MouseTab->UpdateView();
			}
}

void UGameMapUI_MouseViewEditor::UpdateView()
{
	if (this->TotalRound == 0 || this->TotalRoundNode == 0)
	{
		this->DisEnableView();

		return;
	}
	else {
		this->EnableView();
	}

	for (auto ViewGrid : this->RoundNodeGridPanel->GetAllChildren())
	{
		Cast<UMouseViewEditorRoundNodeGrid>(ViewGrid)->UpdateView();
	}
}

void UGameMapUI_MouseViewEditor::EnableView()
{
	for (auto ViewGrid : this->RoundNodeGridPanel->GetAllChildren())
	{
		Cast<UMouseViewEditorRoundNodeGrid>(ViewGrid)->Enable();
	}
}

void UGameMapUI_MouseViewEditor::DisEnableView()
{
	for (auto ViewGrid : this->RoundNodeGridPanel->GetAllChildren())
	{
		Cast<UMouseViewEditorRoundNodeGrid>(ViewGrid)->DisEnable();
	}
}

void UGameMapUI_MouseViewEditor::Node_AddNewCurRound()
{
	bool Result = this->TotalRound == 0 ? true : false;

	FRondWidthMouseConfig NewConfig;
	this->GameMapUI_MouseTab->GetConfigRef().CurRoundConfig.Emplace(NewConfig);
	this->TotalRound = this->GameMapUI_MouseTab->GetConfigRef().CurRoundConfig.Num();
	this->GameMapUI_MouseTab->UpdateView();

	if (Result)
	{
		//开启视图
		this->EnableView();
		//更新【编辑器视图】
		this->UpdateView();
		this->UpdateLevelItems();
	}
}

void UGameMapUI_MouseViewEditor::Node_RemoveCurRound()
{
	//移除波
	this->Remove(this->GameMapUI_MouseTab->GetConfigRef().CurRoundConfig, this->CurRoundIndex);

	this->GameMapUI_MouseTab->UpdateView();

	this->TotalRound = this->GameMapUI_MouseTab->GetConfigRef().CurRoundConfig.Num();

	//如果当前回合索引小于总回合数
	if (this->CurRoundIndex < this->GameMapUI_MouseTab->GetConfigRef().CurRoundConfig.Num())
	{
		//更新最小子节点
		this->TotalRoundNode = this->TotalRoundNode = this->GameMapUI_MouseTab->GetConfigRef().CurRoundConfig[this->CurRoundIndex].CurNode.Num();
		this->CurRoundNodeIndex = 0;

		this->UpdateView();
		this->UpdateLevelItems();
	}
	else
	{
		//关闭老鼠视图
		this->TotalRoundNode = 0;
		this->CurRoundNodeIndex = 0;
		this->TotalRound = 0;
		this->CurRoundIndex = 0;
		this->DisEnableView();
	}
}

void UGameMapUI_MouseViewEditor::LastCurRound()
{
	//返回上一波
	this->Last(this->CurRoundIndex);

	//更新最小子节点
	if (this->CurRoundIndex < this->GameMapUI_MouseTab->GetConfigRef().CurRoundConfig.Num())
	{
		this->TotalRoundNode = this->TotalRoundNode = this->GameMapUI_MouseTab->GetConfigRef().CurRoundConfig[this->CurRoundIndex].CurNode.Num();
		this->CurRoundNodeIndex = 0;
		//检查子节点
		this->CheckTimeNodeCount();
		//更新【编辑器视图】
		this->UpdateView();
		this->UpdateLevelItems();
	}

	this->OnInit();

	this->OnSelectMouseNodeIsNull();
}

void UGameMapUI_MouseViewEditor::NextCurRound()
{
	this->InNext(this->GameMapUI_MouseTab->GetConfigRef().CurRoundConfig.Num(), this->CurRoundIndex);

	//更新节点
	if (this->CurRoundIndex < this->GameMapUI_MouseTab->GetConfigRef().CurRoundConfig.Num())
	{
		this->TotalRoundNode = this->TotalRoundNode = this->GameMapUI_MouseTab->GetConfigRef().CurRoundConfig[this->CurRoundIndex].CurNode.Num();
		this->CurRoundNodeIndex = 0;
		//检查子节点
		this->CheckTimeNodeCount();
		//更新【编辑器视图】
		this->UpdateView();
		this->UpdateLevelItems();
	}

	this->OnInit();

	this->OnSelectMouseNodeIsNull();
}

void UGameMapUI_MouseViewEditor::Node_AddNewRoundNode()
{
	if (this->CurRoundIndex >= this->GameMapUI_MouseTab->GetConfigRef().CurRoundConfig.Num())
	{
		return;
	}

	if (this->GameMapUI_MouseTab->GetConfigRef().CurRoundConfig[this->CurRoundIndex].CurNode.Num() == 10)
	{
		return;
	}

	//得到地图配置
	const FGameMapStruct& CurGameMapData = this->FVMEditUI_GameMapEdit->GetCurEditData().M_FGameMapStruct;

	FTimeNodeWidthRound NewRoundNode;
	for (int32 Row = 0; Row < 20; Row++)
	{
		FTimeNode CurTimeNode;
		for (int32 Col = 0; Col < CurGameMapData.M_Meshe.Num(); Col++)
		{
			FMouseConfigNode CurNewMouseNode;
			CurNewMouseNode.CurMouseName = FString();
			CurTimeNode.CurMouseNode.Emplace(CurNewMouseNode);
		}
		NewRoundNode.CurNode.Emplace(CurTimeNode);
	}

	this->GameMapUI_MouseTab->GetConfigRef().CurRoundConfig[this->CurRoundIndex].CurNode.Emplace(NewRoundNode);

	this->TotalRoundNode = this->GameMapUI_MouseTab->GetConfigRef().CurRoundConfig[this->CurRoundIndex].CurNode.Num();

	this->GameMapUI_MouseTab->UpdateView();

	if (this->CurRoundNodeIndex == 0)
	{
		//更新【编辑器视图】
		this->EnableView();
		this->UpdateView();
		this->UpdateLevelItems();
	}
}

void UGameMapUI_MouseViewEditor::Node_RemoveCurRoundNode()
{
	if (this->CurRoundIndex >= this->GameMapUI_MouseTab->GetConfigRef().CurRoundConfig.Num())
	{
		return;
	}

	//移除回合节点
	this->Remove(this->GameMapUI_MouseTab->GetConfigRef().CurRoundConfig[this->CurRoundIndex].CurNode, this->CurRoundNodeIndex);

	this->GameMapUI_MouseTab->UpdateView();

	this->TotalRoundNode = this->GameMapUI_MouseTab->GetConfigRef().CurRoundConfig[this->CurRoundIndex].CurNode.Num();

	//更新【编辑器视图】
	if (this->TotalRoundNode == 0)
	{
		this->DisEnableView();
	}
	else {
		this->EnableView();
		this->UpdateView();
		this->UpdateLevelItems();
	}
}

void UGameMapUI_MouseViewEditor::LastCurRoundNode()
{
	if (this->CurRoundIndex >= this->GameMapUI_MouseTab->GetConfigRef().CurRoundConfig.Num())
	{
		return;
	}

	this->Last(this->CurRoundNodeIndex);

	//检查最小子节点
	this->CheckTimeNodeCount();
	//更新【编辑器视图】
	this->UpdateView();
	this->UpdateLevelItems();

	this->OnSelectMouseNodeIsNull();
}

void UGameMapUI_MouseViewEditor::NextCurRoundNode()
{
	if (this->CurRoundIndex >= this->GameMapUI_MouseTab->GetConfigRef().CurRoundConfig.Num())
	{
		return;
	}

	this->InNext(this->GameMapUI_MouseTab->GetConfigRef().CurRoundConfig[this->CurRoundIndex].CurNode.Num(), this->CurRoundNodeIndex);

	//检查最小子节点
	this->CheckTimeNodeCount();
	//更新【编辑器视图】
	this->UpdateView();
	this->UpdateLevelItems();

	this->OnSelectMouseNodeIsNull();
}

void UGameMapUI_MouseViewEditor::SetAllMouseSpawnFinishAutoNext(bool Enable)
{
	if (this->TotalRound != 0)
	{
		FRondWidthMouseConfig& LRoundConfig = this->GameMapUI_MouseTab->GetConfigRef().CurRoundConfig[this->CurRoundIndex];
		LRoundConfig.AllMouseSpawnFinishAutoNext = Enable;
		this->GameMapUI_MouseTab->UpdateView();
	}
}

bool UGameMapUI_MouseViewEditor::GetAllMouseSpawnFinishAutoNext()
{
	if (this->TotalRound != 0)
	{
		FRondWidthMouseConfig& LRoundConfig = this->GameMapUI_MouseTab->GetConfigRef().CurRoundConfig[this->CurRoundIndex];
		return LRoundConfig.AllMouseSpawnFinishAutoNext;
	}

	return false;
}

void UGameMapUI_MouseViewEditor::SetInNextRoundTime(float InNdexTime)
{
	if (this->TotalRound != 0)
	{
		FRondWidthMouseConfig& LRoundConfig = this->GameMapUI_MouseTab->GetConfigRef().CurRoundConfig[this->CurRoundIndex];
		LRoundConfig.InNextRoundTime = InNdexTime;
		this->GameMapUI_MouseTab->UpdateView();
	}
}

float UGameMapUI_MouseViewEditor::GetInNextRoundTime()
{
	if (this->TotalRound != 0)
	{
		FRondWidthMouseConfig& LRoundConfig = this->GameMapUI_MouseTab->GetConfigRef().CurRoundConfig[this->CurRoundIndex];
		return LRoundConfig.InNextRoundTime;
	}

	return 0.f;
}

void UGameMapUI_MouseViewEditor::SetCurRoundKey(FRoundKey Key)
{
	if (this->TotalRound != 0)
	{
		FRondWidthMouseConfig& LRoundConfig = this->GameMapUI_MouseTab->GetConfigRef().CurRoundConfig[this->CurRoundIndex];
		LRoundConfig.RoundKey = Key;
		this->GameMapUI_MouseTab->UpdateView();
	}
}

FRoundKey UGameMapUI_MouseViewEditor::GetCurRoundKey()
{
	if (this->TotalRound != 0)
	{
		FRondWidthMouseConfig& LRoundConfig = this->GameMapUI_MouseTab->GetConfigRef().CurRoundConfig[this->CurRoundIndex];
		return LRoundConfig.RoundKey;
	}

	return FRoundKey();
}

void UGameMapUI_MouseViewEditor::SelectRoundNodeWithMouseNode(UMouseViewEditorRoundNodeGrid* CurMouseViewEditorRoundNodeGrid)
{
	this->MouseViewEditorRoundNodeGrid = CurMouseViewEditorRoundNodeGrid;
	this->OnSelectMouseNode(this->MouseViewEditorRoundNodeGrid->MouseConfigNode);
}

void UGameMapUI_MouseViewEditor::UpdateCurRoundNodeWidthMouseNode(FMouseConfigNode NewNode)
{
	if (IsValid(this->MouseViewEditorRoundNodeGrid))
	{
		//写入数据
		{
			//获取老鼠配置
			FMouseConfig& CurMouseConfig = this->GetGameMapUIMouseTab()->GetConfigRef();

			//覆写当前节点的老鼠
			CurMouseConfig.CurRoundConfig[
				this->GetCurRoundIndex() //回合
			].CurNode[
				this->GetCurRoundNodeIndex()//回合节点
			].CurNode[
				this->MouseViewEditorRoundNodeGrid->ColIndex //通过列得到时间节点【x轴】
			].CurMouseNode[
				this->MouseViewEditorRoundNodeGrid->RowIndex//通过行得到老鼠节点【Y轴】
			] = NewNode;
		}

		this->MouseViewEditorRoundNodeGrid->MouseConfigNode = NewNode;

		//覆盖数据
		this->GameMapUI_MouseTab->UpdateView();

		//显示内容
		this->MouseViewEditorRoundNodeGrid->UpdateView();
	}
}

void UGameMapUI_MouseViewEditor::EnableMouseNodeRemove()
{
	if (this->bEnableMouseNodeRemove)
	{
		this->bEnableMouseNodeRemove = false;

		this->OnMouseNodeRemoveClose();
	}
	else {
		this->bEnableMouseNodeRemove = true;

		this->OnMouseNodeRemoveOpen();
	}
}

void UGameMapUI_MouseViewEditor::InitLevelItems()
{
	//获取掉落物配置
	const FGameMapData& Data = this->FVMEditUI_GameMapEdit->GetCurEditData();

	//得到全部的

	int32 i = 0;
	for (const auto& CurItem : Data.M_FLevelConfig.LevelItems)
	{
		UGameMapUI_MouseViewLevelItem* CurNewItem =
			CreateWidget<UGameMapUI_MouseViewLevelItem>(this,
				LoadClass<UGameMapUI_MouseViewLevelItem>(0,
					TEXT("WidgetBlueprint'/Game/Resource/BP/Data/MapData/MouseConfig/BP_MouseViewLevelItem.BP_MouseViewLevelItem_C'")));

		CurNewItem->Init(this, CurItem.Key, CurItem.Value);

		this->LevelItemsGridPanel->AddChildToUniformGrid(CurNewItem, i / 2, i % 2);

		i++;
	}

	//更新显示
	this->UpdateLevelItems();
}

void UGameMapUI_MouseViewEditor::UpdateLevelItems()
{
	if (TotalRound == 0)
		return;

	//隐藏所有
	for (auto Widget : this->CurShowLevelItemListPanel->GetAllChildren())
	{
		Widget->SetVisibility(ESlateVisibility::Collapsed);
	}

	FRondWidthMouseConfig& RoundConfig = this->GameMapUI_MouseTab->GetConfigRef().CurRoundConfig[this->CurRoundIndex];
	const FGameMapData& Data = this->FVMEditUI_GameMapEdit->GetCurEditData();

	//更新显示
	int32 i = 0;
	for (auto Name : RoundConfig.LevelItems)
	{
		//查询
		const FSoftObjectPath* ResultPath = Data.M_FLevelConfig.LevelItems.Find(Name);

		if (i < this->CurShowLevelItemListPanel->GetAllChildren().Num())
		{
			this->CurShowLevelItemListPanel->GetChildAt(i)->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

			if (ResultPath)
				Cast<UGameMapUI_MouseViewLevelItemView>(this->CurShowLevelItemListPanel->GetChildAt(i))->Init(Name, *ResultPath, this);
			else
				Cast<UGameMapUI_MouseViewLevelItemView>(this->CurShowLevelItemListPanel->GetChildAt(i))->Init(Name, FSoftObjectPath(), this);
		}
		else {

			UGameMapUI_MouseViewLevelItemView* CurNewItem =
				CreateWidget<UGameMapUI_MouseViewLevelItemView>(this,
					LoadClass<UGameMapUI_MouseViewLevelItemView>(0,
						TEXT("WidgetBlueprint'/Game/Resource/BP/Data/MapData/MouseConfig/BP_MouseViewLevelItemEditor.BP_MouseViewLevelItemEditor_C'")));

			if (ResultPath)
				CurNewItem->Init(Name, *ResultPath, this);
			else
				CurNewItem->Init(Name, FSoftObjectPath(), this);

			this->CurShowLevelItemListPanel->AddChildToHorizontalBox(CurNewItem);
		}

		i++;
	}
}

bool UGameMapUI_MouseViewLevelItem::Initialize()
{
	Super::Initialize();

	this->ItemNameText = Cast<UTextBlock>(this->GetWidgetFromName(FName(TEXT("TextBlock_48"))));
	this->Butt = Cast<UButton>(this->GetWidgetFromName(FName(TEXT("Button_115"))));

	return true;
}

void UGameMapUI_MouseViewLevelItem::Init(UGameMapUI_MouseViewEditor* OWner, const FString& ItemName, FSoftObjectPath Img)
{
	this->GameMapUI_MouseViewEditor = OWner;
	this->ImgRes = Img;

	this->ItemNameText->SetText(FText::FromString(ItemName));

	FButtonStyle Style;
	Style.Normal.SetResourceObject(Img.TryLoad());
	Style.Normal.TintColor = FLinearColor(FVector(1.f, 1.f, 1.f));
	Style.Hovered.SetResourceObject(Img.TryLoad());
	Style.Hovered.TintColor = FLinearColor(FVector(7.f, 7.f, 7.f));
	Style.Pressed.SetResourceObject(Img.TryLoad());
	Style.Pressed.TintColor = FLinearColor(FVector(5.f, 5.f, 5.f));

	Style.Disabled.SetResourceObject(Img.TryLoad());
	this->Butt->SetStyle(Style);
}

void UGameMapUI_MouseViewLevelItem::AddLevelItem()
{
	if (this->GameMapUI_MouseViewEditor->TotalRound == 0)
		return;

	FRondWidthMouseConfig& RoundConfig =
		this->GameMapUI_MouseViewEditor->GameMapUI_MouseTab->GetConfigRef().
		CurRoundConfig[this->GameMapUI_MouseViewEditor->CurRoundIndex];

	RoundConfig.LevelItems.Emplace(this->ItemNameText->GetText().ToString());

	this->GameMapUI_MouseViewEditor->GameMapUI_MouseTab->UpdateView();
	this->GameMapUI_MouseViewEditor->UpdateLevelItems();
}

bool UGameMapUI_MouseViewLevelItemView::Initialize()
{
	Super::Initialize();

	this->Butt = Cast<UButton>(this->GetWidgetFromName(FName(TEXT("Button_115"))));

	return true;
}

void UGameMapUI_MouseViewLevelItemView::Init(FString Name, FSoftObjectPath Img, UGameMapUI_MouseViewEditor* Class)
{
	this->GameMapUI_MouseViewEditor = Class;
	this->ItemName = Name;

	FButtonStyle Style;
	Style.Normal.SetResourceObject(Img.TryLoad());
	Style.Normal.TintColor = FLinearColor(FVector(1.f, 1.f, 1.f));
	Style.Hovered.SetResourceObject(Img.TryLoad());
	Style.Hovered.TintColor = FLinearColor(FVector(7.f, 7.f, 7.f));
	Style.Pressed.SetResourceObject(Img.TryLoad());
	Style.Pressed.TintColor = FLinearColor(FVector(5.f, 5.f, 5.f));

	Style.Disabled.SetResourceObject(Img.TryLoad());
	this->Butt->SetStyle(Style);

	this->Butt->OnClicked.Clear();
	this->Butt->OnClicked.AddDynamic(this, &UGameMapUI_MouseViewLevelItemView::Remove);
}

void UGameMapUI_MouseViewLevelItemView::Remove()
{
	FRondWidthMouseConfig& RoundConfig =
		this->GameMapUI_MouseViewEditor->GameMapUI_MouseTab->GetConfigRef().
		CurRoundConfig[this->GameMapUI_MouseViewEditor->CurRoundIndex];

	RoundConfig.LevelItems.Remove(this->ItemName);

	this->GameMapUI_MouseViewEditor->GameMapUI_MouseTab->UpdateView();
	this->GameMapUI_MouseViewEditor->UpdateLevelItems();
}
