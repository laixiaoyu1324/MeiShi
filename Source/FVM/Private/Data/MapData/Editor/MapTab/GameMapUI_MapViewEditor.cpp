// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除


#include "Data/MapData/Editor/MapTab/GameMapUI_MapViewEditor.h"
#include "Data/MapData/Editor/MapTab/GameMapUI_MapTab.h"
#include "Data/MapData/Editor/FVMEditUI_GameMapEdit.h"
#include "Data/MapData/Editor/MapTab/GameMapUI_MapViewCardItem.h"
#include "Data/CardData/CardDataStruct.h"
#include "Paper2D/Classes/PaperSprite.h"

#include <Components/TextBlock.h>
#include <Components/Button.h>
#include <Components/VerticalBox.h>
#include <Components/EditableTextBox.h>
#include <Components/Button.h>
#include <Components/CanvasPanel.h>
#include <Components/Image.h>
#include <Components/UniformGridPanel.h>



bool UGameMapUI_MapMesheItem::Initialize()
{
	Super::Initialize();

	this->CurMeshe = Cast<UButton>(this->GetWidgetFromName(FName(TEXT("SelectMeshe"))));
	this->bMoveImg = Cast<UImage>(this->GetWidgetFromName(FName(TEXT("bMove"))));
	this->BgImg = Cast<UImage>(this->GetWidgetFromName(FName(TEXT("Bg"))));

	return true;
}


void UGameMapUI_MapMesheItem::InitMapMesheItem(UGameMapUI_MapViewEditor* Class)
{
	this->MapViewEditor = Class;
}

FMesheCol UGameMapUI_MapMesheItem::GetConfig()
{
	return this->CurCol;
}

void UGameMapUI_MapMesheItem::UpdateConfig(FMesheCol MesheCol)
{
	this->CurCol = MesheCol;

	if (MesheCol.M_bMoveActive)
	{
		this->bMoveImg->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}
	else {
		this->bMoveImg->SetVisibility(ESlateVisibility::Collapsed);
	}

	if (MesheCol.M_MeshBG.M_bEnableBG)
	{
		this->BgImg->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

		UPaperSprite* Texture = Cast<UPaperSprite>(MesheCol.M_MeshBG.M_BG_FullPath.TryLoad());
		if (Texture)
		{
			this->BgImg->SetBrushFromTexture(Texture->GetBakedTexture(), true);
		}
		this->BgImg->SetRenderTranslation(FVector2D(MesheCol.M_MeshBG.M_Offset.Y, MesheCol.M_MeshBG.M_Offset.Z * -1.f));

	}
	else {
		this->BgImg->SetVisibility(ESlateVisibility::Collapsed);
	}

	this->UpdateView();
}

void UGameMapUI_MapMesheItem::UpdateView()
{

	if (this->CurLineType == this->CurCol.M_ELineType)
	{
		return;
	}

	FLinearColor Color = FLinearColor(FVector(1.f, 1.f, 1.f));

	switch (this->CurCol.M_ELineType)
	{
		//绿色
	case ELineType::OnGround:Color = FLinearColor(FVector(0.f, 1.f, 0.f)); break;
		//浅蓝
	case ELineType::OnWater:Color = FLinearColor(FVector(0.f, 0.f, 0.4f)); break;
		//深蓝
	case ELineType::Underwater:Color = FLinearColor(FVector(0.f, 0.f, 1.f)); break;
		//红色
	case ELineType::Magma: Color = FLinearColor(FVector(1.f, 0.f, 0.f)); break;
		//禁用【红+绿】
	case ELineType::DisEnable: Color = FLinearColor(FVector(1.f, 1.f, 0.f)); break;
		//全地形【绿 + 蓝】
	case ELineType::All: Color = FLinearColor(FVector(0.f, 1.f, 1.f)); break;
	default:
		//不变
		Color = FLinearColor(FVector(1.f, 1.f, 1.f));
		break;
	}

	FButtonStyle Style;
	Style.Normal.SetResourceObject(nullptr);
	Style.Normal.TintColor = Color;
	Style.Hovered.SetResourceObject(nullptr);
	Style.Hovered.TintColor = Color * 0.7f;
	Style.Pressed.SetResourceObject(nullptr);
	Style.Pressed.TintColor = Color * 0.5;

	Style.Disabled.SetResourceObject(nullptr);
	this->CurMeshe->SetStyle(Style);

	this->CurLineType = this->CurCol.M_ELineType;
}

void UGameMapUI_MapMesheItem::Select()
{
	this->MapViewEditor->OnSelectMeshe(this->CurCol, this->Row, this->Col);
}

bool UGameMapUI_MapViewEditor::Initialize()
{
	if (!Super::Initialize())
		return false;

	this->MeshePanel = Cast<UCanvasPanel>(this->GetWidgetFromName(FName(TEXT("CanvasPanel_40"))));
	this->Bg = Cast<UImage>(this->GetWidgetFromName(FName(TEXT("Image_54"))));
	this->CardsPanel = Cast<UUniformGridPanel>(this->GetWidgetFromName(FName(TEXT("CardsPanel"))));

	return true;
}

template<typename T>
void AddToCards(TArray<T*>& Arrays, TArray<FItemCard>& out)
{
	for (auto Item : Arrays)
	{
		out.Emplace((*(FItemCard*)(&Item->M_FItemCard)));
	}
}

void UGameMapUI_MapViewEditor::InitMapViewEditor(UGameMapUI_MapTab* CurGameMapUI_MapTab)
{
	this->GameMapUI_MapTab = CurGameMapUI_MapTab;

	//设置背景
	Bg->SetBrushResourceObject(CurGameMapUI_MapTab->GetEditor()->GetCurEditData().M_FLevelConfig.LevelBGHead.TryLoad());

	this->OnInit(CurGameMapUI_MapTab->GetConfig());

	//初始化卡片选择
	//this->CardsPanel
	TArray<FItemCard> CardDatas;

	//加载卡片表格
	UDataTable* CardsTab = LoadObject<UDataTable>(0, TEXT("DataTable'/Game/Resource/BP/Data/CardData/CardATKData.CardATKData'"));
	TArray<FCard_ATK_Data*> ATKCards;
	CardsTab->GetAllRows(TEXT("ATKCards"), ATKCards);

	CardsTab = LoadObject<UDataTable>(0, TEXT("DataTable'/Game/Resource/BP/Data/CardData/CardDefenceData.CardDefenceData'"));
	TArray<FCard_Defence_Data*> Card_DefenceCards;
	CardsTab->GetAllRows(TEXT("Card_DefenceCard"), Card_DefenceCards);

	CardsTab = LoadObject<UDataTable>(0, TEXT("DataTable'/Game/Resource/BP/Data/CardData/CardFunctionData.CardFunctionData'"));
	TArray<FCard_Function_Data*> FCard_FunctionCards;
	CardsTab->GetAllRows(TEXT("FCard_FunctionCards"), FCard_FunctionCards);

	CardsTab = LoadObject<UDataTable>(0, TEXT("DataTable'/Game/Resource/BP/Data/CardData/CardSpawnData.CardSpawnData'"));
	TArray<FCard_Spawn_Data*> FCard_SpawnCards;
	CardsTab->GetAllRows(TEXT("FCard_SpawnCards"), FCard_SpawnCards);

	CardsTab = LoadObject<UDataTable>(0, TEXT("DataTable'/Game/Resource/BP/Data/CardData/CardRangeATKData.CardRangeATKData'"));
	TArray<FCard_RangeATK_Data*> FCard_RangeATKCards;
	CardsTab->GetAllRows(TEXT("FCard_RangeATKCards"), FCard_RangeATKCards);

	AddToCards<FCard_ATK_Data>(ATKCards, CardDatas);
	AddToCards<FCard_Defence_Data>(Card_DefenceCards, CardDatas);
	AddToCards<FCard_Function_Data>(FCard_FunctionCards, CardDatas);
	AddToCards<FCard_Spawn_Data>(FCard_SpawnCards, CardDatas);
	AddToCards<FCard_RangeATK_Data>(FCard_RangeATKCards, CardDatas);

	//创建并且初始化UI
	int32 i = 0;
	for (const FItemCard& Card : CardDatas)
	{
		UGameMapUI_MapViewCardItem* NewCardItem = CreateWidget<UGameMapUI_MapViewCardItem>(
			this,
			LoadClass<UGameMapUI_MapViewCardItem>(0,
				TEXT("WidgetBlueprint'/Game/Resource/BP/Data/MapData/MapTab/BPUI_MapViewCardItem.BPUI_MapViewCardItem_C'"))
			);

		NewCardItem->Init(Card.ItemName, Card.CardActorResource, Card.CardActorHead);

		//添加到界面
		this->CardsPanel->AddChildToUniformGrid(NewCardItem, i / 4, i % 4);
		i++;
	}
}

UFVMEditUI_GameMapEdit* UGameMapUI_MapViewEditor::GetEditor()
{
	return this->GameMapUI_MapTab->GetEditor();
}

void UGameMapUI_MapViewEditor::UpdateMeshe(FMesheCol MesheCol, int32 Row, int32 Col)
{
	this->GameMapUI_MapTab->GetConfigRef().M_Meshe[Row].M_Col[Col] = MesheCol;

	Cast<UGameMapUI_MapMesheItem>(
		this->MeshePanel->GetChildAt(Row * this->GameMapUI_MapTab->GetConfigRef().M_Meshe[0].M_Col.Num() + Col)
		)->UpdateConfig(MesheCol);

	this->RenderMeshe();
}

void UGameMapUI_MapViewEditor::UniformSelectChangeMeshe(int32 Row, int32 Col, FMesheCol MesheCol)
{
	//判断行数量
	if (Row != -1)
		if (Row < this->GameMapUI_MapTab->GetConfigRef().M_Meshe.Num())
		{
			int32 ColIndex = 0;
			for (FMesheCol& CurCol : this->GameMapUI_MapTab->GetConfigRef().M_Meshe[Row].M_Col)
			{
				CurCol = MesheCol;

				Cast<UGameMapUI_MapMesheItem>(
					this->MeshePanel->GetChildAt(Row * this->GameMapUI_MapTab->GetConfigRef().M_Meshe[Row].M_Col.Num() + ColIndex)
					)->UpdateConfig(MesheCol);

				ColIndex++;
			}
		}

	//判断行数量
	if (Col != -1)
		if (this->GameMapUI_MapTab->GetConfigRef().M_Meshe.Num() != 0)
		{
			if (Col < this->GameMapUI_MapTab->GetConfigRef().M_Meshe[0].M_Col.Num())
			{
				int32 RowIndex = 0;
				for (FMesheRow& CurRow : this->GameMapUI_MapTab->GetConfigRef().M_Meshe)
				{
					CurRow.M_Col[Col] = MesheCol;

					Cast<UGameMapUI_MapMesheItem>(
						this->MeshePanel->GetChildAt(RowIndex * this->GameMapUI_MapTab->GetConfigRef().M_Meshe[RowIndex].M_Col.Num() + Col)
						)->UpdateConfig(MesheCol);


					RowIndex++;
				}
			}
		}


	this->RenderMeshe();
}

void UGameMapUI_MapViewEditor::UpdateWiew()
{
	for (auto CurMeshe : this->MeshePanel->GetAllChildren())
	{
		Cast<UGameMapUI_MapMesheItem>(CurMeshe)->UpdateView();
	}
}

void UGameMapUI_MapViewEditor::UpdateNewRowAndCol(int32 NewRow, int32 NewCol, FVector2D FirstGridLocation)
{
	if (NewRow < 0 || NewCol < 0 || NewRow > 12 || NewCol > 12)
	{
		return;
	}

	this->GameMapUI_MapTab->GetConfigRef().M_FirstRowMesheLocation = FirstGridLocation;

	//更新行
	if (NewRow > this->GameMapUI_MapTab->GetConfigRef().M_Meshe.Num())
	{
		int32 CurCount = this->GameMapUI_MapTab->GetConfigRef().M_Meshe.Num();
		//新增行
		for (int32 i = 0; i < NewRow - CurCount; i++)
		{
			FMesheRow CurNewRow;
			this->GameMapUI_MapTab->GetConfigRef().M_Meshe.Emplace(CurNewRow);
		}
	}
	else if (NewRow < this->GameMapUI_MapTab->GetConfigRef().M_Meshe.Num()) {
		for (int32 i = this->GameMapUI_MapTab->GetConfigRef().M_Meshe.Num(); i > NewRow; i--)
		{
			//删除行
			this->GameMapUI_MapTab->GetConfigRef().M_Meshe.RemoveAt(i - 1);
		}
	}


	//更新列
	for (FMesheRow& CurRow : this->GameMapUI_MapTab->GetConfigRef().M_Meshe)
	{
		//新增列
		if (NewCol > CurRow.M_Col.Num())
		{
			int32 CurCount = CurRow.M_Col.Num();
			for (int32 i = 0; i < NewCol - CurCount; i++)
			{
				FMesheCol CurNewCol;
				CurRow.M_Col.Emplace(CurNewCol);
			}
		}
		else if (NewCol < CurRow.M_Col.Num())
		{
			//删除列
			for (int32 i = CurRow.M_Col.Num(); i > NewCol; i--)
			{
				CurRow.M_Col.RemoveAt(i - 1);
			}
		}
	}



	this->RenderMeshe();
	this->UpdateWiew();
}

void UGameMapUI_MapViewEditor::RenderMeshe()
{
	//this->MeshePanel->ClearChildren();

	for (auto Item : this->MeshePanel->GetAllChildren())
	{
		Item->SetVisibility(ESlateVisibility::Collapsed);
	}

	//行
	int32 Row = 0;
	for (const FMesheRow& CurRow : this->GameMapUI_MapTab->GetConfigRef().M_Meshe)
	{
		int32 Col = 0;
		for (const FMesheCol& CurCol : CurRow.M_Col)
		{
			if (UGameMapUI_MapMesheItem* CurItem = Cast<UGameMapUI_MapMesheItem>(this->MeshePanel->GetChildAt(Row * CurRow.M_Col.Num() + Col)))
			{
				CurItem->Row = Row;
				CurItem->Col = Col;

				CurItem->UpdateConfig(CurCol);
				CurItem->InitMapMesheItem(this);

				//设置位置
				FVector2D Location = this->GameMapUI_MapTab->GetConfigRef().M_FirstRowMesheLocation;
				//UI偏移
				Location -= FVector2D(-527.f, -190.f);

				Location.X += CurCol.M_PaddingCol * Col;
				Location.Y -= CurRow.M_PaddingRow * Row;
				CurItem->SetRenderTranslation(Location);
				CurItem->SetRenderScale(FVector2D(1.f, -1.f));

				CurItem->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			}
			else {
				UGameMapUI_MapMesheItem* CurMeshe = CreateWidget<UGameMapUI_MapMesheItem>(
					this,
					LoadClass<UGameMapUI_MapMesheItem>(nullptr,
						TEXT("WidgetBlueprint'/Game/Resource/BP/Data/MapData/MapTab/BP_MapViewEditorMesheItem.BP_MapViewEditorMesheItem_C'"))
					);

				CurMeshe->Row = Row;
				CurMeshe->Col = Col;

				CurMeshe->UpdateConfig(CurCol);
				CurMeshe->InitMapMesheItem(this);

				//设置位置
				FVector2D Location = this->GameMapUI_MapTab->GetConfigRef().M_FirstRowMesheLocation;
				//UI偏移
				Location -= FVector2D(-527.f, -190.f);

				Location.X += CurCol.M_PaddingCol * Col;
				Location.Y -= CurRow.M_PaddingRow * Row;
				CurMeshe->SetRenderTranslation(Location);
				CurMeshe->SetRenderScale(FVector2D(1.f, -1.f));

				//缓存
				this->MeshePanel->AddChildToCanvas(CurMeshe);
			}

			Col++;
		}

		Row++;
	}
}