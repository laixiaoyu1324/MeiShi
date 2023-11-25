// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/UI/UI_PlayerSynthesis.h"
#include <Components/Button.h>
#include <Components/ScrollBox.h>
#include <Components/CanvasPanel.h>
#include <Components/Image.h>

//武器开槽镶嵌
#include "Game/UI/Synthesis/SynModel_WepaonGems.h"
//宝石强化
#include "Game/UI/Synthesis/SynModel_GemsUpGrade.h"
//宝石分解
#include "Game/UI/Synthesis/SynModel_GemSplit.h"
//材料基础结构
#include "GameSystem/Item/Material/MaterialBaseStruct.h"

#include "Game/UI/UI_PlayerBagMaterialGrid.h"
#include "Game/UI/UI_PlayerBagCardGrid.h"

#include "GameSystem/FVMGameInstance.h"
#include "GameSystem/PlayerStructManager.h"

bool UUI_PlayerSynthesis::Initialize()
{
	if (!Super::Initialize())
		return false;

	//卡片界面显示与隐藏
	this->M_CardsPanel_1 = this->GetWidgetComponent<UWidget>(this, "Cards_E_Panel");
	this->M_CardsPanel_2 = this->GetWidgetComponent<UWidget>(this, "CanvasPanel_7");
	this->M_ColoverPanel = this->GetWidgetComponent<UWidget>(this, "CanvasPanel_6");


	this->M_MakeCard_Butt = this->GetWidgetComponent<UButton>(this, "MakeCard_butt");
	this->M_UpCard_Butt = this->GetWidgetComponent<UButton>(this, "UpCard_butt");
	this->M_ChangeCard_Butt = this->GetWidgetComponent<UButton>(this, "ChangeCard_butt");

	this->M_MakeCardP = this->GetWidgetComponent<UCanvasPanel>(this, "Make_P");
	this->M_UpCardP = this->GetWidgetComponent<UCanvasPanel>(this, "Up_P");
	this->M_ChangeCardP = this->GetWidgetComponent<UCanvasPanel>(this, "Change_P");


	/*----------------------------------------材料加载区----------------------------------------*/
	this->M_UScrollBox_Item_Material = this->GetWidgetComponent<UScrollBox>(this, "ScrollBox_77");
	this->M_Materials_2_BoxArr = this->GetWidgetComponent<UUniformGridPanel>(this, "UniformGridPanel_60");
	/*----------------------------------------材料加载区----------------------------------------*/


	/*----------------------------------------卡片制作----------------------------------------*/
	this->M_Cards_Box = this->GetWidgetComponent<UUniformGridPanel>(this, "Cards_Box");
	this->M_Materials_Box = this->GetWidgetComponent<UUniformGridPanel>(this, "Materials_Box");

	this->M_UScrollBox_MakeCard = this->GetWidgetComponent<UScrollBox>(this, "MakeCard_ScrollBox");
	this->M_UScrollBox_MakeCard_Material = this->GetWidgetComponent<UScrollBox>(this, "ScrollBox_40");

	this->M_MakeCard_Run_Butt = this->GetWidgetComponent<UButton>(this, "Make_Butt");
	this->M_Spices_Butt = this->GetWidgetComponent<UButton>(this, "Material_3_Butt");
	this->M_SynthesisBlueprint_Butt = this->GetWidgetComponent<UButton>(this, "Main_Material_Butt");
	this->M_Material_0_Image = this->GetWidgetComponent<UImage>(this, "Material_0");
	this->M_Material_1_Image = this->GetWidgetComponent<UImage>(this, "Material_1");
	this->M_Material_2_Image = this->GetWidgetComponent<UImage>(this, "Material_2");
	/*----------------------------------------卡片制作----------------------------------------*/


	/*----------------------------------------卡片强化----------------------------------------*/
	this->M_UScrollBox_Card_Uniform = this->GetWidgetComponent<UScrollBox>(this, "ScrollBox_1");
	this->M_Uniform_CardGrid = this->GetWidgetComponent<UUniformGridPanel>(this, "Cards_Box_1");
	this->M_Insurance_Butt = this->GetWidgetComponent<UButton>(this, "Button_4");
	this->M_UpGradeCard_Main_Butt = this->GetWidgetComponent<UButton>(this, "Button_98");
	this->M_UpGradeCard_0_Butt = this->GetWidgetComponent<UButton>(this, "Button");
	this->M_UpGradeCard_1_Butt = this->GetWidgetComponent<UButton>(this, "Button_1");
	this->M_UpGradeCard_2_Butt = this->GetWidgetComponent<UButton>(this, "Button_2");
	this->M_UpGradeCard_Butt = this->GetWidgetComponent<UButton>(this, "Upgrade");
	this->M_Clover_Butt = this->GetWidgetComponent<UButton>(this, "Button_3");
	/*----------------------------------------卡片强化----------------------------------------*/



	/*----------------------------------------卡片转职----------------------------------------*/
	this->M_ChangeJob_0_Image = this->GetWidgetComponent<UImage>(this, "Image_118");
	this->M_ChangeJob_1_Image = this->GetWidgetComponent<UImage>(this, "Image_11");
	this->M_ChangeJob_2_Image = this->GetWidgetComponent<UImage>(this, "Image_12");
	this->M_ChangeJobMainCard_Butt = this->GetWidgetComponent<UButton>(this, "Button_5");
	this->M_ChangeJob_Butt = this->GetWidgetComponent<UButton>(this, "ChangeCardType_Butt");
	this->M_ChangeJob_2_P = this->GetWidgetComponent<UCanvasPanel>(this, "CanvasPanel_4");
	/*----------------------------------------卡片转职----------------------------------------*/


	/*----------------------------------------金卡进化----------------------------------------*/
	this->M_GoldCardUpHead = this->GetWidgetComponent<UImage>(this, "GoldCardUpHead");
	this->M_GoldCardUpResultHead = this->GetWidgetComponent<UImage>(this, "Image_164");
	this->M_GoldCardUp_Butt = this->GetWidgetComponent<UButton>(this, "GoldCardUp_Butt");
	/*----------------------------------------金卡进化----------------------------------------*/

	/*----------------------------------------武器开槽镶嵌----------------------------------------*/
	this->M_WepaonGem_TabButt = this->GetWidgetComponent<UButton>(this, "WeaponGem");
	this->M_WepaonGemP = this->GetWidgetComponent<UCanvasPanel>(this, "WeaponGem_P");
	/*----------------------------------------武器开槽镶嵌----------------------------------------*/

	/*----------------------------------------宝石强化----------------------------------------*/
	this->M_GemUp_TabButt = this->GetWidgetComponent<UButton>(this, "GemUp");
	this->M_GemUpP = this->GetWidgetComponent<UCanvasPanel>(this, "GemUp_P");
	/*----------------------------------------宝石强化----------------------------------------*/

	/*----------------------------------------宝石分解----------------------------------------*/
	this->M_GemSplit_TabButt = this->GetWidgetComponent<UButton>(this, "GemSplit");
	this->M_GemSplitP = this->GetWidgetComponent<UCanvasPanel>(this, "GemSplit_P");
	/*----------------------------------------宝石分解----------------------------------------*/



	return true;
}

//初始化所有的加载器
void UUI_PlayerSynthesis::Loader_Init()
{
	if (this->M_UItemLoadManager_MakeCard)
		return;

	/*----------------------------------------[卡片界面主容器的子界面【卡片界面，装备界面】]----------------------------------------*/
	this->M_TabWidgets_Panels.Emplace(this->GetWidgetComponent<UWidget>(this, "ScrollBox_1"));
	this->M_TabWidgets_Panels.Emplace(this->GetWidgetComponent<UWidget>(this, "ScrollBox_2"));
	/*----------------------------------------[卡片界面主容器的子界面【卡片界面，装备界面】]----------------------------------------*/


	/*----------------------------------------卡片制作----------------------------------------*/
	//绑定取消选卡按钮
	this->M_SynthesisBlueprint_Butt->OnClicked.AddDynamic(this, &UUI_PlayerSynthesis::CancelSelectMakeCard);
	//绑定制作卡片按钮
	M_MakeCard_Run_Butt->OnClicked.AddDynamic(this, &UUI_PlayerSynthesis::MakeCard);
	//绑定香料的取消按钮
	this->M_Spices_Butt->OnClicked.AddDynamic(this, &UUI_PlayerSynthesis::CancelSpicesDestroy);
	/*----------------------------------------卡片制作----------------------------------------*/


	/*----------------------------------------卡片强化----------------------------------------*/
	//添加卡槽
	this->M_UpGradeSlot.Empty();
	this->M_UpGradeSlot.Emplace(this->M_UpGradeCard_Main_Butt);
	this->M_UpGradeSlot.Emplace(this->M_UpGradeCard_0_Butt);
	this->M_UpGradeSlot.Emplace(this->M_UpGradeCard_1_Butt);
	this->M_UpGradeSlot.Emplace(this->M_UpGradeCard_2_Butt);
	//绑定强化功能
	this->M_UpGradeCard_Butt->OnClicked.AddDynamic(this, &UUI_PlayerSynthesis::CardUpGrade);
	this->M_Insurance_Butt->OnClicked.AddDynamic(this, &UUI_PlayerSynthesis::SelectInsurance);

	this->M_UpGradeCard_Main_Grade = this->GetWidgetComponent<UImage>(this, "Image_14");
	this->M_UpGradeCard_0_Grade = this->GetWidgetComponent<UImage>(this, "Image_15");
	this->M_UpGradeCard_1_Grade = this->GetWidgetComponent<UImage>(this, "Image_109");
	this->M_UpGradeCard_2_Grade = this->GetWidgetComponent<UImage>(this, "Image_16");
	/*----------------------------------------卡片强化----------------------------------------*/


	/*----------------------------------------卡片转职----------------------------------------*/
	this->M_ChangeJobMainCard_Butt->OnClicked.AddDynamic(this, &UUI_PlayerSynthesis::LoadCardsToCardChange);
	this->M_ChangeJob_Butt->OnClicked.AddDynamic(this, &UUI_PlayerSynthesis::CardChangeJobs);
	/*----------------------------------------卡片转职----------------------------------------*/

	//-----------------------------------------------
	//记录卡片状态是否存放了卡片（名称为""表示没有存放卡片）
	this->M_GoldCard_Name_State.Emplace("");
	this->M_GoldCard_Name_State.Emplace("");
	//需要消耗掉的卡片
	this->M_NeedCards.Emplace("");
	this->M_NeedCards.Emplace("");
	//需要消耗掉的卡片等级
	this->M_NeedGradeCards.Emplace(0);
	this->M_NeedGradeCards.Emplace(0);
	//对应卡片的UI索引
	this->M_NeedCard_UI_Index.Emplace(-1);
	this->M_NeedCard_UI_Index.Emplace(-1);
	//-----------------------------------------------

	this->M_GoldCard_Select_Button.Emplace(this->GetWidgetComponent<UButton>(this, "Button_143"));
	this->M_GoldCard_Select_Button.Emplace(this->GetWidgetComponent<UButton>(this, "Button_7"));

	//选项卡切换功能(宝石卡片，神级进化)
	this->M_TabSelectPanels.Emplace(this->GetWidgetComponent<UWidget>(this, "PanelChange"));
	this->M_TabSelectPanels.Emplace(this->GetWidgetComponent<UWidget>(this, "PanelChange2"));

	//卡片展示界面集合
	this->M_Cards2_Box.Emplace(this->GetWidgetComponent<UUniformGridPanel>(this, "Cards_Box_1"));
	this->M_Cards2_Box.Emplace(this->GetWidgetComponent<UUniformGridPanel>(this, "Cards_Box_2"));

	//按钮事件绑定
	this->M_MakeCard_Butt->OnClicked.AddDynamic(this, &UUI_PlayerSynthesis::ShowMakeCard);
	this->M_UpCard_Butt->OnClicked.AddDynamic(this, &UUI_PlayerSynthesis::ShowUpCard);
	this->M_ChangeCard_Butt->OnClicked.AddDynamic(this, &UUI_PlayerSynthesis::ShowChangeCard);

	/*----------------------------------------武器开槽镶嵌----------------------------------------*/
	this->M_WepaonGem_TabButt->OnClicked.AddDynamic(this, &UUI_PlayerSynthesis::ShowWeaponGem);
	//初始化武器开槽镶嵌功能
	if (!this->M_USynModel_WepaonGems)
	{
		this->M_USynModel_WepaonGems = NewObject<USynModel_WepaonGems>(this, TEXT("Synthesis_USynModel_WepaonGems"));
		this->M_USynModel_WepaonGems->InitializeBySynthesis(this);
	}
	/*----------------------------------------武器开槽镶嵌----------------------------------------*/

	/*----------------------------------------宝石强化----------------------------------------*/
	this->M_GemUp_TabButt->OnClicked.AddDynamic(this, &UUI_PlayerSynthesis::ShowGemUp);
	if (!this->M_USynModel_GemsUpGrade)
	{
		this->M_USynModel_GemsUpGrade = NewObject<USynModel_GemsUpGrade>(this, TEXT("Synthesis_USynModel_GemsUpGrade"));
		this->M_USynModel_GemsUpGrade->InitializeBySynthesis(this);
	}
	/*----------------------------------------宝石强化----------------------------------------*/

	/*----------------------------------------宝石分解----------------------------------------*/
	this->M_GemSplit_TabButt->OnClicked.AddDynamic(this, &UUI_PlayerSynthesis::ShowGemSplit);
	if (!this->M_USynModel_GemSplit)
	{
		this->M_USynModel_GemSplit = NewObject<USynModel_GemSplit>(this, TEXT("Synthesis_USynModel_GemSplit"));
		this->M_USynModel_GemSplit->InitializeBySynthesis(this);
	}
	/*----------------------------------------宝石分解----------------------------------------*/

	//初始化卡片制作加载器
	this->M_UItemLoadManager_MakeCard = NewObject<UItemLoadManager>(this, TEXT("MakeCardLoad"));
	this->M_UItemLoadManager_MakeCard->InitWidget_First(this->M_Cards_Box, this->M_UScrollBox_MakeCard);
	this->M_UItemLoadManager_MakeCard->InitRange_Second(3, 7, 800, 120.f, 150.f);
	this->M_UItemLoadManager_MakeCard->OnCreateItem.BindUFunction(this, TEXT("WidgetCreate_InitMakeCard"));
	this->M_UItemLoadManager_MakeCard->OnRefreshItem.BindUFunction(this, TEXT("WidgetRefresh_UpdateMakeCard"));
	this->M_UItemLoadManager_MakeCard->OnLoadEndItem.BindUFunction(this, TEXT("CardsLoadEnd"));

	//初始化卡片制作-材料区域-加载器
	this->M_UItemLoadManager_MakeCard_Materials = NewObject<UItemLoadManager>(this, TEXT("MakeCard_Material_Load"));
	this->M_UItemLoadManager_MakeCard_Materials->InitWidget_First(this->M_Materials_Box, this->M_UScrollBox_MakeCard_Material);
	this->M_UItemLoadManager_MakeCard_Materials->InitRange_Second(3, 7, 800, 100.f, 100.f);
	this->M_UItemLoadManager_MakeCard_Materials->OnCreateItem.BindUFunction(this, TEXT("WidgetCreate_InitMakeCard_Material"));
	this->M_UItemLoadManager_MakeCard_Materials->OnRefreshItem.BindUFunction(this, TEXT("WidgetRefresh_UpdateMakeCard_Material"));
	this->M_UItemLoadManager_MakeCard_Materials->OnLoadEndItem.BindUFunction(this, TEXT("CardBlueprintsLoadEnd"));

	//材料加载器
	this->M_UItemLoadManager_Item_Material = NewObject<UItemLoadManager>(this, TEXT("Material_Load"));
	this->M_UItemLoadManager_Item_Material->InitWidget_First(this->M_Materials_2_BoxArr, this->M_UScrollBox_Item_Material);
	this->M_UItemLoadManager_Item_Material->InitRange_Second(1, 10, 800, 100.f, 100.f, true);
	this->M_UItemLoadManager_Item_Material->SetMainScrollTransfrom(FVector2D(30.f, 0.f), FVector2D(1.05f, 1.f));
	this->M_UItemLoadManager_Item_Material->OnCreateItem.BindUFunction(this, TEXT("WidgetCreate_InitMaterial"));
	this->M_UItemLoadManager_Item_Material->OnRefreshItem.BindUFunction(this, TEXT("WidgetRefresh_UpdateMaterial"));
	this->M_UItemLoadManager_Item_Material->OnLoadEndItem.BindUFunction(this, TEXT("MaterialLoadEnd"));

	//卡片强化加载器
	this->M_UItemLoadManager_CardUpGrade = NewObject<UItemLoadManager>(this, TEXT("CardUpGrade_Load"));
	this->M_UItemLoadManager_CardUpGrade->InitWidget_First(this->M_Uniform_CardGrid, this->M_UScrollBox_Card_Uniform);
	this->M_UItemLoadManager_CardUpGrade->InitRange_Second(5, 6, 800, 120.f, 150.f);
	this->M_UItemLoadManager_CardUpGrade->OnCreateItem.BindUFunction(this, TEXT("WidgetCreate_InitCardUpGrade"));
	this->M_UItemLoadManager_CardUpGrade->OnRefreshItem.BindUFunction(this, TEXT("WidgetRefresh_UpdateCardUpGrade"));
	this->M_UItemLoadManager_CardUpGrade->OnLoadEndItem.BindUFunction(this, TEXT("CardsLoadEnd"));
}

//创建-材料区域-材料UI界面
UWidget* UUI_PlayerSynthesis::WidgetCreate_InitMaterial(UItemDataTable* _Data, int32 _Index)
{
	TSoftClassPtr<UUI_PlayerBagMaterialGrid> LocalUIClass = Cast <UClass>(UAssetManager::GetStreamableManager().LoadSynchronous(FSoftClassPath(FString(TEXT("WidgetBlueprint'/Game/Resource/BP/Game/UI/MainFrame/BP_PlayerBagMaterialGrid.BP_PlayerBagMaterialGrid_C'")))));
	UUI_PlayerBagMaterialGrid* Grid = CreateWidget<UUI_PlayerBagMaterialGrid>(this, LocalUIClass.Get());

	this->SetMaterialsData(Grid, _Data, _Index, this->M_BindFunctionName_Materials);

	return Grid;
}
//刷新-材料区域-材料UI界面
void UUI_PlayerSynthesis::WidgetRefresh_UpdateMaterial(UItemDataTable* _Data, int32 _Index, UWidget* _UWidget)
{
	this->SetMaterialsData(Cast<UUI_PlayerBagMaterialGrid>(_UWidget), _Data, _Index, this->M_BindFunctionName_Materials);
}

//创建界面-卡片制作-卡片显示区域
UWidget* UUI_PlayerSynthesis::WidgetCreate_InitMakeCard(UItemDataTable* _Data, int32 _Index)
{
	TSoftClassPtr<UUI_PlayerBagCardGrid> LocalUIClass = Cast <UClass>(UAssetManager::GetStreamableManager().LoadSynchronous(FSoftClassPath(FString(TEXT("WidgetBlueprint'/Game/Resource/BP/Game/UI/MainFrame/BPUI_PlayerBagCardGrid.BPUI_PlayerBagCardGrid_C'")))));
	UUI_PlayerBagCardGrid* Grid = CreateWidget<UUI_PlayerBagCardGrid>(this, LocalUIClass.Get());

	Grid->SetUIIndex(_Index);

	this->SetMakeCardPanelData(Grid, _Data, _Index);

	return Grid;
}
//更新界面-卡片制作-卡片显示区域
void UUI_PlayerSynthesis::WidgetRefresh_UpdateMakeCard(UItemDataTable* _Data, int32 _Index, UWidget* _UWidget)
{
	this->SetMakeCardPanelData(Cast<UUI_PlayerBagCardGrid>(_UWidget), _Data, _Index);
}

//创建界面-卡片制作-防御卡合成配方材料显示区域
UWidget* UUI_PlayerSynthesis::WidgetCreate_InitMakeCard_Material(UItemDataTable* _Data, int32 _Index)
{
	TSoftClassPtr<UUI_PlayerBagMaterialGrid> LocalUIClass = Cast <UClass>(UAssetManager::GetStreamableManager().LoadSynchronous(FSoftClassPath(FString(TEXT("WidgetBlueprint'/Game/Resource/BP/Game/UI/MainFrame/BP_PlayerBagMaterialGrid.BP_PlayerBagMaterialGrid_C'")))));
	UUI_PlayerBagMaterialGrid* Grid = CreateWidget<UUI_PlayerBagMaterialGrid>(this, LocalUIClass.Get());

	this->SetMakeCard_Material_PanelData(Grid, _Data, _Index);

	return Grid;
}
//更新界面-卡片制作-防御卡合成配方材料显示区域
void UUI_PlayerSynthesis::WidgetRefresh_UpdateMakeCard_Material(UItemDataTable* _Data, int32 _Index, UWidget* _UWidget)
{
	Cast<UUI_PlayerBagMaterialGrid>(_UWidget)->SetIndex(_Index);

	this->SetMakeCard_Material_PanelData(Cast<UUI_PlayerBagMaterialGrid>(_UWidget), _Data, _Index);
}

//卡片制作界面-设置卡片UI界面
void UUI_PlayerSynthesis::SetMakeCardPanelData(UUI_PlayerBagCardGrid* _Grid, UItemDataTable* _CardData, int32 _Index)
{
	_Grid->M_CardTexturePath = ((FItemCard*)(_CardData->GetValue()))->ItemTexturePath.ToString();
	_Grid->SetFItemCardData(((FItemCard*)(_CardData->GetValue())));
	_Grid->SetUI_PlayerSynthesis(this);
	_Grid->SetCardIndex(_Index);
	_Grid->UpdateButtonTexture(FString::FormatAsNumber(((FItemCard*)(_CardData->GetValue()))->M_CardPrice));
	//_Grid->PlayBeginAnim();

	if (_Grid->GetButton()->OnClicked.IsBound())
		_Grid->GetButton()->OnClicked.Clear();

	//绑定音效
	FScriptDelegate AddFunc;
	AddFunc.BindUFunction(_Grid, "PlayOperateAudioDef");
	_Grid->GetButton()->OnClicked.Add(AddFunc);
}
//卡片制作界面-设置材料配方，配方材料UI界面
void UUI_PlayerSynthesis::SetMakeCard_Material_PanelData(UUI_PlayerBagMaterialGrid* _Grid, UItemDataTable* _CardData, int32 _Index)
{
	//设置材料索引
	_Grid->SetIndex(_Index);

	//设置材料个数
	int32 CurCount = ((FMaterialBase*)(_CardData->GetValue()))->M_Count;
	_Grid->UpdateMaterialsShowCount("x" + FString::FromInt(CurCount > 9999 ? 9999 : CurCount));

	//设置材料数据
	_Grid->SetMaterialData(((FMaterialBase*)(_CardData->GetValue())));

	//设置材料按钮
	UWidgetBase::SetButtonStyle(_Grid->GetButton(), ((FMaterialBase*)(_CardData->GetValue()))->ItemTexturePath.ToString());

	//清除材料的事件绑定并且恢复选择状态
	if (_Grid->GetButton()->OnClicked.IsBound())
	{
		_Grid->GetButton()->OnClicked.Clear();
		_Grid->GetButton()->SetIsEnabled(true);
	}

	//绑定事件(卡片制作材料（配方）)
	if (((FMaterialBase*)(_CardData->GetValue()))->M_MaterialType == EMaterialType::E_Blueprint)
	{
		_Grid->SetUI_PlayerSynthesis(this);

		FScriptDelegate AddFunc;
		AddFunc.BindUFunction(_Grid, "AddSynthesisBlueprint");
		_Grid->GetButton()->OnClicked.Add(AddFunc);

		if (this->M_MaterialBlueprintName.Equals(((FMaterialBase*)(_CardData->GetValue()))->ItemName))
		{
			_Grid->GetButton()->SetIsEnabled(false);
			this->SetSynthesisBlueprintData(this, ((FMaterialBase*)(_CardData->GetValue())), _Index);
		}
		else {
			_Grid->GetButton()->SetIsEnabled(true);
		}
	}

	//绑定音效
	FScriptDelegate AddFunc;
	AddFunc.BindUFunction(_Grid, "PlayOperateAudioDef");
	_Grid->GetButton()->OnClicked.Add(AddFunc);

	//_Grid->PlayBeginAnim();
}
//卡片制作界面-设置（香料，四叶草，等等）UI界面
void UUI_PlayerSynthesis::SetMaterialsData(UUI_PlayerBagMaterialGrid* _Grid, UItemDataTable* _CardData, int32 _Index, const TArray<FMaterialsSerachTypeBind>& _BindFuncName)
{
	_Grid->SetIndex(_Index);
	_Grid->GetButton()->SetIsEnabled(true);
	int32 CurCount = ((FMaterialBase*)(_CardData->GetValue()))->M_Count;
	_Grid->UpdateMaterialsShowCount("x" + FString::FromInt(CurCount > 9999 ? 9999 : CurCount));
	_Grid->SetMaterialData(((FMaterialBase*)(_CardData->GetValue())));
	UWidgetBase::SetButtonStyle(_Grid->GetButton(), ((FMaterialBase*)(_CardData->GetValue()))->ItemTexturePath.ToString());




	//判断当前道具类型(是否被玩家选中)
	switch (((FMaterialBase*)(_CardData->GetValue()))->M_MaterialType)
	{
	case EMaterialType::E_Spices://判断当前UI是否被玩家选中->如果选中则禁用按钮
		if (this->M_MakeCard_CurrentSelect_Index == _Index) { _Grid->GetButton()->SetIsEnabled(false); this->M_MakeCard_CurrentSelectData = ((FMaterialBase*)(_CardData->GetValue())); } break;
	}






	if (_Grid->GetButton()->OnClicked.IsBound())
		_Grid->GetButton()->OnClicked.Clear();

	//绑定函数
	for (auto& LBind : _BindFuncName)
	{
		if (((FMaterialBase*)(_CardData->GetValue()))->M_MaterialType == LBind.M_Type)
		{
			//设置指向合成屋的UI
			_Grid->SetUI_PlayerSynthesis(this);
			for (const auto& FnName : LBind.M_BindFnName)
			{
				//如果是空的则不绑定
				if (FnName.IsEqual(""))
					continue;
				//绑定
				FScriptDelegate AddFunc;
				AddFunc.BindUFunction(_Grid, FnName);
				_Grid->GetButton()->OnClicked.Add(AddFunc);
			}

			break;
		}
	}

	//绑定音效
	FScriptDelegate AddFunc;
	AddFunc.BindUFunction(_Grid, "PlayOperateAudioDef");
	_Grid->GetButton()->OnClicked.Add(AddFunc);
}


/*加载--卡片界面（卡片制作，金卡进化，卡片转职）显示区域--开始*/
//加载卡片UI界面
UWidget* UUI_PlayerSynthesis::WidgetCreate_InitCardUpGrade(UItemDataTable* _Data, int32 _Index)
{
	UUI_PlayerBagCardGrid* Grid = CreateWidget<UUI_PlayerBagCardGrid>(this, LoadClass<UUI_PlayerBagCardGrid>(0, TEXT("WidgetBlueprint'/Game/Resource/BP/Game/UI/MainFrame/BPUI_PlayerBagCardGrid.BPUI_PlayerBagCardGrid_C'")));

	this->SetCardData(Grid, _Data, _Index);

	return Grid;
}
//更新卡片UI界面
void UUI_PlayerSynthesis::WidgetRefresh_UpdateCardUpGrade(UItemDataTable* _Data, int32 _Index, UWidget* _UWidget)
{
	this->SetCardData(Cast<UUI_PlayerBagCardGrid>(_UWidget), _Data, _Index);
}
//设置卡片UI的数据（需要加载卡片的所有界面-不包括卡片制作界面）
void UUI_PlayerSynthesis::SetCardData(UUI_PlayerBagCardGrid* _Grid, UItemDataTable* _Data, int32 _Index)
{
	//设置纹理
	_Grid->M_CardTexturePath = ((FItemCard*)(_Data->GetValue()))->ItemTexturePath.ToString();
	//设置数据
	_Grid->SetFItemCardData(((FItemCard*)(_Data->GetValue())));
	//设置指向
	_Grid->SetUI_PlayerSynthesis(this);
	//启动按钮
	_Grid->GetButton()->SetIsEnabled(true);

	//设置索引
	_Grid->SetUIIndex(_Index);
	_Grid->SetCardIndex(_Index);

	//更新文本
	_Grid->UpdateButtonTexture(FString::FromInt(((FItemCard*)(_Data->GetValue()))->M_CardPrice));

	//清理绑定
	if (_Grid->GetButton()->OnClicked.IsBound())
	{
		_Grid->GetButton()->OnClicked.Clear();
	}

	//绑定对应的函数功能
	FScriptDelegate AddFunc;

	switch (this->M_Uniform_LoadCardData_Panel_Index)
	{
	case 0: { //卡片强化界面

		//绑定(强化卡片的功能)
		AddFunc.BindUFunction(_Grid, TEXT("AddSynthesisUpGradeFunc"));
		_Grid->GetButton()->OnClicked.Add(AddFunc);

		int32 IIndex = 0;
		for (auto LIndex : this->M_UpGradeCard_UI_Index)
		{
			if (LIndex != -1)
			{
				//匹配UI——>成功匹配 则禁用按钮
				if (LIndex == _Index)
				{				
					this->CancelSelectCard(IIndex, false);
					_Grid->AddSynthesisUpGrade(true, IIndex);
					break;
				}
			}

			IIndex++;
		}

	}break;
	case 1: {
		//绑定(卡片转职的功能)
		AddFunc.BindUFunction(_Grid, TEXT("AddSynthesisChangeJobs"));
		_Grid->GetButton()->OnClicked.Add(AddFunc);

		//选择当前需要转职的卡片
		if (this->M_CurrentCardChangeJobIndex == _Index)
		{
			_Grid->GetButton()->SetIsEnabled(false);
		}

		//等级限制(如果卡片等级小于转职等级  或者 不能转职)
		if (((FItemCard*)(_Data->GetValue()))->M_FCardChangeJobs.M_IsChange)
		{
			if (((FItemCard*)(_Data->GetValue()))->M_CardGrade < ((FItemCard*)(_Data->GetValue()))->M_FCardChangeJobs.M_ChangeGrade)
			{
				_Grid->GetButton()->SetIsEnabled(false);
			}
		}
		else if (!((FItemCard*)(_Data->GetValue()))->M_FCardChangeJobs.M_IsChange)
		{
			_Grid->GetButton()->SetIsEnabled(false);
		}
	}break;
	case 2: {

		//选择界面
		for (const auto& LIndex : this->M_NeedCard_UI_Index)
		{
			if (LIndex != -1 && LIndex == _Index)
			{
				_Grid->GetButton()->SetIsEnabled(false);
			}
		}

		//绑定(金卡进化)
		AddFunc.BindUFunction(_Grid, TEXT("AddSynthesisGoldCardUp"));
		_Grid->GetButton()->OnClicked.Add(AddFunc);
	}break;
	default:
		break;
	}


	//绑定音效
	FScriptDelegate AddFunc_;
	AddFunc_.BindUFunction(_Grid, "PlayOperateAudioDef");
	_Grid->GetButton()->OnClicked.Add(AddFunc_);
}
/*加载卡片界面--结束*/


//加载材料-卡片制作界面（防御卡配方-配方材料-卡片制作界面）
void UUI_PlayerSynthesis::LoadMaterialsToMakeCard()
{
	//材料数据
	TArray<FMaterialBase*> Items;
	UGameSystemFunction::GetMaterialsArrayByType
	(
		UFVMGameInstance::GetFVMGameInstance()->
		GetPlayerStructManager()->
		M_PlayerItems_Material,
		{ EMaterialType::E_Blueprint,EMaterialType::E_CardSynthesisMaterial },
		Items
	);
	this->M_UItemLoadManager_MakeCard_Materials->UpdateDatatable(Items);
	this->M_UItemLoadManager_MakeCard_Materials->SetLoadItemMaxCount(Items.Num());
	this->M_UItemLoadManager_MakeCard_Materials->ContinueRun();
}
//加载卡片-卡片制作界面（防御卡-卡片制作界面）
void UUI_PlayerSynthesis::LoadCardsToMakeCard()
{
	this->M_UItemLoadManager_MakeCard->UpdateDatatable(UFVMGameInstance::GetPlayerStructManager_Static()->M_PlayerItems_Card);
	this->M_UItemLoadManager_MakeCard->SetLoadItemMaxCount(UFVMGameInstance::GetFVMGameInstance()->GetPlayerStructManager()->M_PlayerBagGirdGroup.GetBagCount(1));
	this->M_UItemLoadManager_MakeCard->ContinueRun();
}
//加载香料-卡片制作界面（材料区域-香料加载-卡片制作界面）
void UUI_PlayerSynthesis::LoadSpicesToMakeCard()
{
	//取消香料的选择
	this->CancelSelectSpices();
	this->LoadMaterials_Type({ FMaterialsSerachTypeBind(EMaterialType::E_Spices,{"AddSynthesisSpicesSlot"}) });

	// this->M_Materials_2_BoxArr
}

//加载卡片强化界面（防御卡-卡片强化界面）
void UUI_PlayerSynthesis::LoadCardsToCardUpGrade()
{
	//设置界面->0 卡片强化
	this->M_Uniform_LoadCardData_Panel_Index = 0;

	//背包容量
	this->M_CardBagCount = UFVMGameInstance::GetFVMGameInstance()->
		GetPlayerStructManager()->
		M_PlayerBagGirdGroup.GetBagCount(1);

	//取消转职界面的数据
	this->CancelSelectCardChangeJobs();

	//获取卡片数据
	TArray<FItemCard*> Datas;
	UGameSystemFunction::GetPlayerCardDatas(Datas, this->M_CardBagCount);

	this->M_UItemLoadManager_CardUpGrade->UpdateDatatable(Datas);
	this->M_UItemLoadManager_CardUpGrade->SetLoadItemMaxCount(Datas.Num());
	this->M_UItemLoadManager_CardUpGrade->ContinueRun();
}
//加载强化四叶草（材料区域-四叶草-卡片强化界面）
void UUI_PlayerSynthesis::LoadCloversToCardUpGrade()
{
	this->M_MakeCard_CloverGrid = nullptr;
	this->CancelSelectClover();
	this->LoadMaterials_Type({ FMaterialsSerachTypeBind(EMaterialType::E_Clover,{"AddUpGradeCardCloverSlot"}) });
}

//加载卡片转职界面（材料区域，防御卡转职材料-卡片转职界面）
void UUI_PlayerSynthesis::LoadCardsToCardChange()
{
	this->M_Uniform_LoadCardData_Panel_Index = 1;

	//取消选择卡片转职的数据
	this->CancelSelectCardChangeJobs();

	//取消选择卡片强化的数据
	for (int32 i = 0; i < 4; i++)
		this->CancelSelectCard(i, false);

	//UGameSystemFunction::WidgetSelect(this->M_Cards2_Box, ESlateVisibility::Visible, 1);

	//背包容量
	this->M_CardBagCount = UFVMGameInstance::GetFVMGameInstance()->GetPlayerStructManager()->M_PlayerBagGirdGroup.GetBagCount(1);

	//获取卡片数据
	TArray<FItemCard*> Datas;
	UGameSystemFunction::GetPlayerCardDatas(Datas, this->M_CardBagCount);

	this->M_UItemLoadManager_CardUpGrade->UpdateDatatable(Datas);
	this->M_UItemLoadManager_CardUpGrade->SetLoadItemMaxCount(Datas.Num());
	this->M_UItemLoadManager_CardUpGrade->ContinueRun();
}
//加载卡片转职的转职材料（材料区域-防御卡转职材料-卡片转职界面）
void UUI_PlayerSynthesis::LoadMaterialsToCardChange()
{
	this->LoadMaterials_Type({ FMaterialsSerachTypeBind(EMaterialType::E_CardChangeJobMaterial,{}) });
}
//【金卡进化】加载金卡进化
void UUI_PlayerSynthesis::LoadGoldCard_Card()
{
	//设置金卡进化界面
	this->M_Uniform_LoadCardData_Panel_Index = 2;

	//背包容量
	this->M_CardBagCount = UFVMGameInstance::GetFVMGameInstance()->GetPlayerStructManager()->M_PlayerBagGirdGroup.GetBagCount(1);

	//获取卡片数据
	TArray<FItemCard*> Datas;
	UGameSystemFunction::GetPlayerCardDatas(Datas, this->M_CardBagCount);

	this->M_UItemLoadManager_CardUpGrade->UpdateDatatable(Datas);
	this->M_UItemLoadManager_CardUpGrade->SetLoadItemMaxCount(Datas.Num());
	this->M_UItemLoadManager_CardUpGrade->ContinueRun();
}

//设置特定的界面显示
void UUI_PlayerSynthesis::ShowPanel(UCanvasPanel* _UCanvasPanel, uint8 _Index)
{
	UWidgetBase::SetButtonStyle(this->M_MakeCard_Butt, "Texture2D'/Game/Resource/Texture/UI/Game/PlayerSynthesis/T_PS_8.T_PS_8'");
	UWidgetBase::SetButtonStyle(this->M_UpCard_Butt, "Texture2D'/Game/Resource/Texture/UI/Game/PlayerSynthesis/T_PS_10.T_PS_10'");
	UWidgetBase::SetButtonStyle(this->M_ChangeCard_Butt, "Texture2D'/Game/Resource/Texture/UI/Game/PlayerSynthesis/T_PS_12.T_PS_12'");
	UWidgetBase::SetButtonStyle(this->M_WepaonGem_TabButt, "Texture2D'/Game/Resource/Texture/UI/Game/PlayerSynthesis/665.665'");
	UWidgetBase::SetButtonStyle(this->M_GemUp_TabButt, "Texture2D'/Game/Resource/Texture/UI/Game/PlayerSynthesis/670.670'");
	UWidgetBase::SetButtonStyle(this->M_GemSplit_TabButt, "Texture2D'/Game/Resource/Texture/UI/Game/PlayerSynthesis/675.675'");

	this->M_MakeCardP->SetVisibility(ESlateVisibility::Collapsed);
	this->M_UpCardP->SetVisibility(ESlateVisibility::Collapsed);
	this->M_ChangeCardP->SetVisibility(ESlateVisibility::Collapsed);
	this->M_GemSplitP->SetVisibility(ESlateVisibility::Collapsed);
	this->M_GemUpP->SetVisibility(ESlateVisibility::Collapsed);
	this->M_WepaonGemP->SetVisibility(ESlateVisibility::Collapsed);

	switch (_Index) {
	case 1:UWidgetBase::SetButtonStyle(this->M_UpCard_Butt, "Texture2D'/Game/Resource/Texture/UI/Game/PlayerSynthesis/T_PS_11.T_PS_11'"); break;
	case 2:UWidgetBase::SetButtonStyle(this->M_ChangeCard_Butt, "Texture2D'/Game/Resource/Texture/UI/Game/PlayerSynthesis/T_PS_13.T_PS_13'"); break;
	case 3:UWidgetBase::SetButtonStyle(this->M_WepaonGem_TabButt, "Texture2D'/Game/Resource/Texture/UI/Game/PlayerSynthesis/662.662'"); break;
	case 4:UWidgetBase::SetButtonStyle(this->M_GemUp_TabButt, "Texture2D'/Game/Resource/Texture/UI/Game/PlayerSynthesis/668.668'"); break;
	case 5:UWidgetBase::SetButtonStyle(this->M_GemSplit_TabButt, "Texture2D'/Game/Resource/Texture/UI/Game/PlayerSynthesis/673.673'"); break;
	default:UWidgetBase::SetButtonStyle(this->M_MakeCard_Butt, "Texture2D'/Game/Resource/Texture/UI/Game/PlayerSynthesis/T_PS_9.T_PS_9'"); break;
	}

	_UCanvasPanel->SetVisibility(ESlateVisibility::Visible);
}
//显示卡片制作界面
void UUI_PlayerSynthesis::ShowMakeCard()
{
	if (this->M_MakeCard_Run_Butt)
	{
		this->M_MakeCard_Run_Butt->SetIsEnabled(false);
	}

	this->ShowCardPanelOrInsurancePanel(ESlateVisibility::Hidden, 0, ESlateVisibility::Hidden);

	//材料加载完成重置
	this->M_bMaterialFinish = false;
	//卡片加载完成重置
	this->M_bCardFinish = false;
	//配方材料加载完成重置
	this->M_bCardBlueprintFinish = false;

	//更新列表
	this->CancelSelectMakeCard();
	//更新列表并且再次选中这个香料
	this->LoadSpicesToMakeCard();
	//更新卡片界面->不需要重新加载->直接添加一个到最后
	this->LoadCardsToMakeCard();

	this->ShowPanel(this->M_MakeCardP, 0);
	//强化界面关闭
	this->M_IsOpenUpCardPanel = false;
}
//显示卡片强化界面
void UUI_PlayerSynthesis::ShowUpCard()
{
	//重新绑定四叶草选择
	this->M_Clover_Butt->OnClicked.Clear();
	this->M_Clover_Butt->OnClicked.AddDynamic(this, &UUI_PlayerSynthesis::CancelSelectClover);
	this->M_Clover_Butt->OnClicked.AddDynamic(this, &UWidgetBase::PlayOperateAudioDef);


	UGameSystemFunction::OpenUpdate();

	this->ShowCardPanelOrInsurancePanel(ESlateVisibility::Visible, 0, ESlateVisibility::Visible, ESlateVisibility::Visible);

	//禁用强化按钮
	this->M_UpGradeCard_Butt->SetIsEnabled(false);
	//材料加载完成重置
	this->M_bMaterialFinish = false;
	//卡片加载完成重置
	this->M_bCardFinish = false;

	//清除卡片强化的数据
	for (int32 i = 0; i < 4; i++)
	{
		this->CancelSelectCard(i, false);
	}

	//更新卡片界面->不需要重新加载->直接添加一个到最后
	this->LoadCardsToCardUpGrade();

	//加载四叶草
	this->LoadCloversToCardUpGrade();

	this->ShowPanel(this->M_UpCardP, 1);

	//强化界面打开
	this->M_IsOpenUpCardPanel = true;
}
//显示卡片转职界面
void UUI_PlayerSynthesis::ShowChangeCard()
{
	UGameSystemFunction::OpenUpdate();

	this->ShowCardPanelOrInsurancePanel(ESlateVisibility::Visible, 0, ESlateVisibility::Visible);

	//材料加载完成重置
	this->M_bMaterialFinish = false;
	//卡片加载完成重置
	this->M_bCardFinish = false;
	//禁用转职按钮
	this->M_ChangeJob_Butt->SetIsEnabled(false);

	//加载卡片
	this->LoadCardsToCardChange();
	//加载转职材料
	this->LoadMaterialsToCardChange();


	this->ShowPanel(this->M_ChangeCardP, 2);
	//强化界面关闭
	this->M_IsOpenUpCardPanel = false;
}
//显示武器宝石界面
void UUI_PlayerSynthesis::ShowWeaponGem()
{
	this->ShowPanel(this->M_WepaonGemP, 3);

	this->M_USynModel_WepaonGems->WidgetReset();
	this->M_USynModel_WepaonGems->LoadEquipment();
	this->M_USynModel_WepaonGems->LoadMaterials();

	this->ShowCardPanelOrInsurancePanel(ESlateVisibility::Visible, 1, ESlateVisibility::Hidden);
}
//显示宝石强化界面
void UUI_PlayerSynthesis::ShowGemUp()
{
	this->ShowPanel(this->M_GemUpP, 4);

	this->M_USynModel_GemsUpGrade->WidgetReset();
	this->M_USynModel_GemsUpGrade->WidgetResetLoadData();

	this->ShowCardPanelOrInsurancePanel(ESlateVisibility::Visible, 1, ESlateVisibility::Visible, ESlateVisibility::Visible);

}
//显示宝石分解界面
void UUI_PlayerSynthesis::ShowGemSplit()
{
	this->ShowPanel(this->M_GemSplitP, 5);

	this->M_USynModel_GemSplit->WidgetReset();
	this->M_USynModel_GemSplit->WidgetResetLoadData();

	this->ShowCardPanelOrInsurancePanel(ESlateVisibility::Visible, 1, ESlateVisibility::Hidden);

}
//【金卡进化】显示金卡进化界面
void UUI_PlayerSynthesis::ShowGoldCardUpPanel()
{
	this->ShowCardPanelOrInsurancePanel(ESlateVisibility::Visible, 0, ESlateVisibility::Hidden);

	this->M_GoldCardUpHead->SetVisibility(ESlateVisibility::Collapsed);
	this->M_GoldCardUpResultHead->SetVisibility(ESlateVisibility::Collapsed);

	for (auto LBut : this->M_GoldCard_Select_Button)
		LBut->SetVisibility(ESlateVisibility::Collapsed);

	for (auto& LButState : this->M_GoldCard_Name_State)
		LButState = "";

	this->M_GoldCardUp_Butt->SetIsEnabled(false);
}
//关闭合成屋界面
void UUI_PlayerSynthesis::ClearPanel()
{
	//关闭随机更新
	UGameSystemFunction::CloseUpdate();
	//保存存档
	UGameSystemFunction::SaveCurrentPlayerData();

	this->RemoveFromParent();
}


//取消卡片制作的选择，并且重新加载卡片制作界面
void UUI_PlayerSynthesis::CancelSelectMakeCard()
{
	//原因:它指向的玩家背包中加载的数据->并非新创建的数据，被指向的数据由玩家角色结构管理器统一管理
	this->M_MaterialBlueprintData = nullptr;

	this->M_MakeCard_CurrentSelectBlueprintData_Index = -1;

	this->M_MaterialsData.Empty();
	this->M_MaterialBlueprintName.Empty();
	this->M_MaterialBlueprintTargetName.Empty();

	UWidgetBase::SetImageBrush(this->M_Material_0_Image,
		"Texture2D'/Game/Resource/Texture/UI/Game/PlayerSynthesis/T_PS_17.T_PS_17'");

	UWidgetBase::SetImageBrush(this->M_Material_1_Image,
		"Texture2D'/Game/Resource/Texture/UI/Game/PlayerSynthesis/T_PS_17.T_PS_17'");

	UWidgetBase::SetImageBrush(this->M_Material_2_Image,
		"Texture2D'/Game/Resource/Texture/UI/Game/PlayerSynthesis/T_PS_17.T_PS_17'");

	UWidgetBase::SetButtonStyle(this->M_SynthesisBlueprint_Butt,
		"Texture2D'/Game/Resource/Texture/UI/Game/PlayerSynthesis/T_PS_16.T_PS_16'");

	this->LoadMaterialsToMakeCard();
}
//对香料进行取消选择（上一次的香料名称保留）
void UUI_PlayerSynthesis::CancelSelectSpices()
{
	this->M_MakeCard_CurrentSelect_Index = -1;
	this->M_MakeCard_CurrentSelectData = nullptr;
	//还原按钮样式
	UWidgetBase::SetButtonStyle(this->M_Spices_Butt, "Texture2D'/Game/Resource/Texture/UI/Game/PlayerSynthesis/T_PS_18.T_PS_18'");
}
//对香料进行取消选择（完全取消）
void UUI_PlayerSynthesis::CancelSpicesDestroy()
{
	//清除名称
	this->M_SelectSpicesName = "";

	//更新列表-香料
	this->LoadSpicesToMakeCard();
}

//设置玩家选择了卡片配方的数据（设置）
void UUI_PlayerSynthesis::SetSynthesisBlueprintData(UUI_PlayerSynthesis* _PlayerSynthesis, FMaterialBase* _MaterialBaseData, int32 _Index)
{
	//设置选中的UI
	_PlayerSynthesis->M_MakeCard_CurrentSelectBlueprintData_Index = _Index;

	//清空之前材料数据
	_PlayerSynthesis->M_MaterialsData.Empty();

	//获取 配方  配方材料
	TArray<FMaterial_CardSynthesisBlueprint_Data*> SynthesisBlueprintDatas;
	TArray<FMaterial_CardSynthesisMaterial_Data*> SynthesisDatas;
	UGameSystemFunction::GetDataTableRows<FMaterial_CardSynthesisBlueprint_Data>(
		"DataTable'/Game/Resource/BP/Data/MaterialData/Material_CardSynthesisBlueprint_Data.Material_CardSynthesisBlueprint_Data'",
		SynthesisBlueprintDatas
		);
	UGameSystemFunction::GetDataTableRows<FMaterial_CardSynthesisMaterial_Data>(
		"DataTable'/Game/Resource/BP/Data/MaterialData/Material_CardSynthesisMaterial_Data.Material_CardSynthesisMaterial_Data'",
		SynthesisDatas
		);

	//卡片数据（配方数据）
	FCardBlueprint* CardBlueprintData = nullptr;

	//查询卡片数据（根据配方制作的防御卡进行查询）
	for (auto Items : SynthesisBlueprintDatas)
	{
		if (Items->M_FMaterial.ItemName.Equals(_MaterialBaseData->ItemName))
		{
			CardBlueprintData = &Items->M_FMaterial;
			break;
		}
	}

	//对应材料的图片路径（配方材料组->最终查询的结果）
	TMap<FString, FString> M_MaterialTextures;
	//得到配方材料名称
	TArray<FString> M_MaterialsName = CardBlueprintData->M_Materials;

	//查询材料纹理数据
	for (auto Items : SynthesisDatas)
	{
		int32 Local_Index = 0;
		for (auto MaterialName : M_MaterialsName)
		{
			//查询配方材料的具体材料->当查询成功时->移出材料->退出循环
			if (Items->M_FMaterial.ItemName.Equals(MaterialName))
			{
				M_MaterialTextures.Emplace(Items->M_FMaterial.ItemName, Items->M_FMaterial.ItemTexturePath.ToString());
				M_MaterialsName.RemoveAt(Local_Index);
				break;
			}
			Local_Index++;
		}
	}

	//根据卡片数据从角色背包寻找对应材料
	UPlayerStructManager* PlayerData = UFVMGameInstance::GetFVMGameInstance()->GetPlayerStructManager();

	//获取对应数据(配方材料->将指向背包中的材料数据进行指向到合成屋)
	for (auto ItemName : CardBlueprintData->M_Materials)
	{
		_PlayerSynthesis->M_MaterialsData.Emplace(
			UGameSystemFunction::GetMaterialDataFromBag(PlayerData->M_PlayerItems_Material, ItemName)
		);
	}



	//获取配方名称
	_PlayerSynthesis->M_MaterialBlueprintName = _MaterialBaseData->ItemName;
	//获取对应数据(合成配方)
	_PlayerSynthesis->M_MaterialBlueprintData =
		UGameSystemFunction::GetMaterialDataFromBag(PlayerData->M_PlayerItems_Material,
			_MaterialBaseData->ItemName);
	//获取最终合成的卡片名称
	_PlayerSynthesis->M_MaterialBlueprintTargetName = CardBlueprintData->M_SynthesisCardName;

	//更新图片显示样式（显示配方图像）
	UWidgetBase::SetButtonStyle(_PlayerSynthesis->M_SynthesisBlueprint_Butt,
		CardBlueprintData->ItemTexturePath.ToString());

	//根据配方材料的个数->呈现不同的显示方式
	if (M_MaterialTextures.Num() == 2)
	{
		_PlayerSynthesis->M_Material_1_Image->SetVisibility(ESlateVisibility::Collapsed);

		SetImageGreyColor(_PlayerSynthesis->M_MaterialsData[0],
			_PlayerSynthesis->M_Material_0_Image,
			*M_MaterialTextures.Find(CardBlueprintData->M_Materials[0]));

		SetImageGreyColor(_PlayerSynthesis->M_MaterialsData[1],
			_PlayerSynthesis->M_Material_2_Image,
			*M_MaterialTextures.Find(CardBlueprintData->M_Materials[1]));
	}
	else {
		_PlayerSynthesis->M_Material_1_Image->SetVisibility(ESlateVisibility::Visible);

		//颜色判断->如果玩家没有这个道具那么显示的时候颜色暗淡 0.5f
		SetImageGreyColor(_PlayerSynthesis->M_MaterialsData[0],
			_PlayerSynthesis->M_Material_0_Image,
			*M_MaterialTextures.Find(CardBlueprintData->M_Materials[0]));

		SetImageGreyColor(_PlayerSynthesis->M_MaterialsData[1],
			_PlayerSynthesis->M_Material_1_Image,
			*M_MaterialTextures.Find(CardBlueprintData->M_Materials[1]));

		SetImageGreyColor(_PlayerSynthesis->M_MaterialsData[2],
			_PlayerSynthesis->M_Material_2_Image,
			*M_MaterialTextures.Find(CardBlueprintData->M_Materials[2]));
	}
}
//卡片制作-功能
void UUI_PlayerSynthesis::MakeCard()
{
	if (!this->M_MaterialBlueprintData)
	{
		//提示玩家应该选择卡片合成的配方
		UWidgetBase::CreateTipWidget(TEXT("请道具栏中选择防御卡合成配方"));
		return;
	}

	//查询数据的完整性
	bool Reload = false;
	for (FMaterialBase* _ItemDatas : this->M_MaterialsData)
	{
		if (!_ItemDatas)
		{
			//提示玩家——>缺少材料
			UWidgetBase::CreateTipWidget(TEXT("缺少材料"));
			return;
		}
		else {
			if (_ItemDatas->M_Count <= 0)
			{
				Reload = true;
				_ItemDatas->bWaitRemove = true;
			}
		}
	}

	//重新加载
	if (Reload)
	{
		UGameSystemFunction::ClearWaitingItems(
			UFVMGameInstance::GetPlayerStructManager_Static()->M_PlayerItems_Material
		);

		UGameSystemFunction::SaveCurrentPlayerData();

		this->CancelSelectMakeCard();

		UWidgetBase::CreateTipWidget(TEXT("材料数据出错，重新加载"));

		return;
	}

	//查询背包空间
	if (UFVMGameInstance::GetFVMGameInstance()->GetPlayerStructManager()->GetBagNum(1) == UFVMGameInstance::GetFVMGameInstance()->GetPlayerStructManager()->GetBagMaxCount(1))
	{
		UWidgetBase::CreateTipWidget(TEXT("背包空间不足"));
		return;
	}

	//支付金币200
	if (!UFVMGameInstance::GetPlayerStructManager_Static()->ReduceCoin(200, 0))
	{
		UWidgetBase::CreateTipWidget(TEXT("金币不足"));
		return;
	}

	//消耗制作材料的配方
	this->M_MaterialBlueprintData->M_Count--;
	//制作目标卡片的名称
	//this->M_MaterialBlueprintTargetName = this->M_MaterialBlueprintData->

	FCardBlueprint Tp;
	if (UMaterialBaseStruct::GetMaterialSourceData<FCardBlueprint>(
		this->M_MaterialBlueprintData->ItemName, Tp, EMaterialType::E_Blueprint
		))
	{
		UE_LOG(LogTemp, Error, TEXT("%s    %s"), *Tp.ItemName, *Tp.M_SynthesisCardName);

		this->M_MaterialBlueprintTargetName = Tp.M_SynthesisCardName;
	}


	//数量为0
	if (this->M_MaterialBlueprintData->M_Count == 0)
	{
		this->M_MaterialBlueprintData->bWaitRemove = true;
	}

	//消耗道具
	for (FMaterialBase* _ItemDatas : this->M_MaterialsData)
	{
		_ItemDatas->M_Count--;
		if (_ItemDatas->M_Count == 0)
		{
			_ItemDatas->bWaitRemove = true;
		}
	}

	//获取制作成功的卡片数据
	FItemCard _Card;
	if (UCardBaseStruct::SearchCardFromDataTable(this->M_MaterialBlueprintTargetName, _Card))
	{
		_Card.M_CardGrade = 0;

		//设置卡片等级（如果存在那么设置卡片等级然后移除）
		if (this->M_MakeCard_CurrentSelect_Index != -1)
		{
			FString ItemName = this->M_MakeCard_CurrentSelectData->ItemName;
			this->M_SelectSpicesName = ItemName;

			FSpicesMaterial _Spices;
			UMaterialBaseStruct::GetMaterialSourceData<FSpicesMaterial>(ItemName, _Spices, EMaterialType::E_Spices);

			//设置制作的卡片等级
			_Card.M_CardGrade = _Spices.M_MakeCardGrade;

			//扣除道具数量
			this->M_MakeCard_CurrentSelectData->M_Count -= 5;

			//如果数量为0
			if (this->M_MakeCard_CurrentSelectData->M_Count <= 0)
			{
				//移出当前道具
				this->M_MakeCard_CurrentSelectData->bWaitRemove = true;
			}

			//当前香料不可用
			if (this->M_MakeCard_CurrentSelectData->M_Count < 5)
			{
				//表示当前香料已经不能使用了
				this->CancelSpicesDestroy();
			}
		}

		//材料加载完成重置
		this->M_bMaterialFinish = false;
		//卡片加载完成重置
		this->M_bCardFinish = false;
		//配方材料加载完成重置
		this->M_bCardBlueprintFinish = false;

		UWidgetBase::CreateTipWidget(TEXT("制作成功"), FVector(1.f, 0.f, 1.f));

		//取消选择
		if (this->M_MaterialBlueprintData->bWaitRemove)
		{
			this->M_MaterialBlueprintTargetName.Empty();

			this->CancelSelectMakeCard();
		}

		//清空待删除的道具
		UGameSystemFunction::ClearWaitingItems<FMaterialBase>(
			UFVMGameInstance::GetFVMGameInstance()->GetPlayerStructManager()->M_PlayerItems_Material
			);

		//将卡片添加到背包
		if (!this->M_MakeCard_CardName.Equals(_Card.ItemName))
		{
			this->M_MakeCard_MakeIndex = -1;

			for (
				auto CardPP = UFVMGameInstance::GetFVMGameInstance()->
				GetPlayerStructManager()->M_PlayerItems_Card.CreateConstIterator();
				CardPP;
				++CardPP
				)
			{
				if ((*CardPP).ItemName.Equals(_Card.ItemName))
				{
					this->M_MakeCard_MakeIndex = CardPP.GetIndex();
					this->M_MakeCard_CardName = _Card.ItemName;

					break;
				}
			}
		}

		if (this->M_MakeCard_MakeIndex == -1)
		{
			UFVMGameInstance::GetFVMGameInstance()->GetPlayerStructManager()->M_PlayerItems_Card.Emplace(_Card);
		}
		else
		{
			UFVMGameInstance::GetFVMGameInstance()->GetPlayerStructManager()->
				M_PlayerItems_Card.EmplaceAt(this->M_MakeCard_MakeIndex, _Card);
		}
	}

	//材料加载完成重置
	this->M_bMaterialFinish = false;
	//卡片加载完成重置
	this->M_bCardFinish = false;
	//配方材料加载完成重置
	this->M_bCardBlueprintFinish = false;

	this->M_MakeCard_Run_Butt->SetIsEnabled(false);

	//更新列表配方
	this->LoadMaterialsToMakeCard();

	//更新列表并且再次选中这个香料
	this->LoadMaterials_Type({ FMaterialsSerachTypeBind(EMaterialType::E_Spices,{"AddSynthesisSpicesSlot"}) });

	//更新卡片界面->不需要重新加载->直接添加一个到最后
	this->LoadCardsToMakeCard();

	//更新货币文字显示
	this->Update_CoinText();

	//保存数据
	UGameSystemFunction::SaveCurrentPlayerData();
}


//【卡片强化界面】卡片的取消选择
void UUI_PlayerSynthesis::CancelSelectCard(int32 _Cancel_Index, bool _bReload)
{
	//将对应的卡片状态进行关闭
	this->M_UpGradeCardState[_Cancel_Index] = false;
	//还原存储槽位的索引
	this->M_UpGradeCard_UI_Index[_Cancel_Index] = -1;
	this->M_UpGradeCard_Datas[_Cancel_Index] = nullptr;

	//对主卡的图片进行单独设置
	if (_Cancel_Index == 0)
		UWidgetBase::SetButtonStyle(this->M_UpGradeSlot[_Cancel_Index],
			"Texture2D'/Game/Resource/Texture/UI/Game/PlayerSynthesis/T_PS_21.T_PS_21'");
	else
		UWidgetBase::SetButtonStyle(this->M_UpGradeSlot[_Cancel_Index],
			"Texture2D'/Game/Resource/Texture/UI/Game/PlayerSynthesis/T_PS_20.T_PS_20'");

	//取消主卡
	if (_Cancel_Index == 0)
	{
		this->_CardUpgrate_Price_0 = FString("");
		this->M_UpGradeCard_Main_Grade->SetVisibility(ESlateVisibility::Hidden);

		//更新保险金 金额
		this->M_Insurance_Coin_2_Text = TEXT("0");

		//主卡的玩家背包索引归-1
		this->M_MainCard_Data_Index = -1;
	}

	//取消副卡
	if (_Cancel_Index == 1)
	{
		this->_CardUpgrate_Price_1 = FString("");
		this->M_UpGradeCard_0_Grade->SetVisibility(ESlateVisibility::Hidden);
	}

	if (_Cancel_Index == 2)
	{
		this->_CardUpgrate_Price_2 = FString("");
		this->M_UpGradeCard_1_Grade->SetVisibility(ESlateVisibility::Hidden);
	}

	if (_Cancel_Index == 3)
	{
		this->_CardUpgrate_Price_3 = FString("");
		this->M_UpGradeCard_2_Grade->SetVisibility(ESlateVisibility::Hidden);
	}

	//更新概率
	this->GetUpGradeRate();

	if (_bReload)
	{
		this->M_UItemLoadManager_CardUpGrade->ContinueRun();
	}
}
//【卡片强化界面】四叶草取消选择
void UUI_PlayerSynthesis::CancelSelectClover()
{
	if (this->M_MakeCard_CloverGrid)
	{
		this->M_MakeCard_CloverGrid->GetButton()->SetIsEnabled(true);
		this->M_MakeCard_CloverGrid = nullptr;
	}
	UWidgetBase::SetButtonStyle(this->M_Clover_Butt, "Texture2D'/Game/Resource/Texture/UI/Game/PlayerSynthesis/T_PS_25.T_PS_25'");
	//更新概率
	this->GetUpGradeRate();
}
UButton* UUI_PlayerSynthesis::GetCloverCancelButton()
{
	return this->M_Clover_Butt;
}
UUI_PlayerBagMaterialGrid* const UUI_PlayerSynthesis::GetCloverData()
{
	return this->M_MakeCard_CloverGrid;
}
//【卡片强化界面】卡片强化-功能
void UUI_PlayerSynthesis::CardUpGrade()
{
	//判断主卡是否存在，主卡不存在则不能强化
	if (!this->M_UpGradeCardState[0])
	{
		//提示玩家请选择主要强化的卡片
		UWidgetBase::CreateTipWidget(TEXT("选择一张[主卡]"));

		return;
	}

	//判断主卡等级
	if (this->M_UpGradeCard_Datas[0]->M_CardGrade == 16)
	{
		//提示玩家主卡等级已经到最高级
		UWidgetBase::CreateTipWidget(TEXT("最高等级不能继续强化"));

		return;
	}

	//选择保险金
	if (this->M_UpGrade_bSelectInsurance)
	{
		//保险金点券金额(主卡等级决定)
		if (
			UFVMGameInstance::GetPlayerStructManager_Static()->M_FPlayerCoin.M_Coin_2
			>=
			this->LInsurance_CoinNum[this->M_UpGradeCard_Datas[0]->M_CardGrade]
			)
		{
			if (UFVMGameInstance::GetPlayerStructManager_Static()->M_FPlayerCoin.M_Coin_0 >= 200)
			{
				UFVMGameInstance::GetPlayerStructManager_Static()->ReduceCoin(this->LInsurance_CoinNum[this->M_UpGradeCard_Datas[0]->M_CardGrade], 2);
				UFVMGameInstance::GetPlayerStructManager_Static()->ReduceCoin(200, 0);
			}
			else {
				UWidgetBase::CreateTipWidget(TEXT("金币不足"));
				return;
			}
		}
		else {
			UWidgetBase::CreateTipWidget(TEXT("点券不足"));
			return;
		}
	}
	else {
		if (UFVMGameInstance::GetPlayerStructManager_Static()->M_FPlayerCoin.M_Coin_0 >= 200)
		{
			UFVMGameInstance::GetPlayerStructManager_Static()->ReduceCoin(200, 0);
		}
		else {
			UWidgetBase::CreateTipWidget(TEXT("金币不足"));
			return;
		}
	}

	//概率结果
	bool UpResult = UGameSystemFunction::GetRange(this->GetUpGradeRate(), 100);

	bool _UseUpGrade = false;

	//查询副卡
	int32 _Index = 0;
	//全部标记
	for (bool& _Condition : this->M_UpGradeCardState)
	{
		//主卡不参与
		if (_Index == 0)
		{
			_Index++;
			continue;
		}

		//没有选择保险就会被标记 (!有保险失败了) = （没有保险-失败了）(有保险-成功) (没有保险成功)
		if (_Condition)
		{
			//标记
			if (!(this->M_UpGrade_bSelectInsurance && !UpResult))
			{
				this->M_UpGradeCard_Datas[_Index]->bWaitRemove = true;
			}

			_UseUpGrade = true;
		}

		_Index++;
	}

	if (!_UseUpGrade)
	{
		//提示玩家添加副卡
		UWidgetBase::CreateTipWidget(TEXT("选择一张副卡"));
		return;
	}

	if (UpResult)
	{
		//等级提升（强化成功）
		if (this->M_UpGradeCard_Datas[0]->M_CardGrade < 16)
		{
			this->M_UpGradeCard_Datas[0]->M_CardGrade++;
		}
		UWidgetBase::CreateTipWidget(TEXT("强化成功"), FVector(1.f, 0.f, 1.f));
	}
	else {
		//等级提升（强化失败）
		if (this->M_UpGradeCard_Datas[0]->M_CardGrade >= 6 && !this->M_UpGrade_bSelectInsurance)
		{
			this->M_UpGradeCard_Datas[0]->M_CardGrade--;
		}
		UWidgetBase::CreateTipWidget(TEXT("不够好运,强化失败"), FVector(1.f, 0.f, 1.f));
	}

	//设置主卡的等级（更新等级图标）
	UWidgetBase::SetImageBrush(
		this->M_UpGradeCard_Main_Grade,
		UGameSystemFunction::GetCardGradeImagePath(this->M_UpGradeCard_Datas[0]->M_CardGrade)
	);

	//更新保险金 金额
	this->M_Insurance_Coin_2_Text = FString::FromInt(this->LInsurance_CoinNum[this->M_UpGradeCard_Datas[0]->M_CardGrade]);




	//材料加载完成重置
	this->M_bMaterialFinish = false;
	//卡片加载完成重置
	this->M_bCardFinish = false;
	//禁用强化按钮
	this->M_UpGradeCard_Butt->SetIsEnabled(false);

	//计算卡片索引是否需要重置
	bool LIndexResult = false;
	if (UpResult)
	{
		//强化成功需要计算索引
		LIndexResult = true;
	}
	else {
		//强化失败并且开启了保险
		if (this->GetSelectInsurance())
		{
			LIndexResult = false;
		}
		else {
			LIndexResult = true;
		}
	}

	UE_LOG(LogTemp, Error, TEXT("[][][]   %d"), this->M_MainCard_Data_Index);

	if (LIndexResult)
	{
		//计算主卡索引
		int32 ICount = 0;
		int32 DiCount = 0;
		for (const int32& LIndex : this->M_UpGradeCard_UI_Index)
		{
			UE_LOG(LogTemp, Error, TEXT("[][][]   %d"), LIndex);

			if (ICount == 0)
			{
				ICount++;
				continue;
			}

			if (LIndex > -1)
			{
				if (this->M_MainCard_Data_Index > LIndex)
				{
					DiCount++;
				}
			}

			ICount++;
		}

		this->M_MainCard_Data_Index -= DiCount;

		//设置主卡索引
		this->M_UpGradeCard_UI_Index[0] = this->M_MainCard_Data_Index;

		for (bool& LState : this->M_UpGradeCardState)
		{
			LState = false;
		}

		this->M_UpGradeCardState[0] = true;

		UE_LOG(LogTemp, Error, TEXT("[][][]   %d"), this->M_MainCard_Data_Index);
	}

	//强化成功->除开主卡之外的所有卡(对该卡进行取消选择)
	if (UpResult || (!this->M_UpGrade_bSelectInsurance && !UpResult))
	{
		//清除卡片强化的数据
		for (int32 i = 1; i < 4; i++)
		{
			this->CancelSelectCard(i, false);
		}
	}

	//四叶草判断
	if (this->M_MakeCard_CloverGrid)
	{
		this->M_MakeCard_CloverGrid->GetMaterialData()->M_Count--;
		if (this->M_MakeCard_CloverGrid->GetMaterialData()->M_Count == 0)
		{
			this->M_MakeCard_CloverGrid->GetMaterialData()->bWaitRemove = true;
			UGameSystemFunction::ClearWaitingItems<FMaterialBase>(
				UFVMGameInstance::GetFVMGameInstance()->GetPlayerStructManager()->M_PlayerItems_Material
				);
		}
		//取消对四叶草的选中
		this->CancelSelectClover();
	}

	//清空待删除的道具
	UGameSystemFunction::ClearWaitingItems<FItemCard>(
		UFVMGameInstance::GetFVMGameInstance()->GetPlayerStructManager()->M_PlayerItems_Card
		);

	//加载四叶草
	this->LoadCloversToCardUpGrade();

	//重新加载卡片
	this->LoadCardsToCardUpGrade();

	//更新货币文字显示
	this->Update_CoinText();

	//保存存档
	UGameSystemFunction::SaveCurrentPlayerData();
}
//【卡片强化数据】获取卡片强化升级的概率
float UUI_PlayerSynthesis::GetUpGradeRate()
{
	//概率
	float Rate = 0.0f;
	//主卡等级
	int32 _MainCardGrade = 0;
	//索引
	int32 _Index_ = 0;

	//当前卡片概率（概率）
	TArray<float> _L_CardRate;

	for (FItemCard* LCardData : this->M_UpGradeCard_Datas)
	{
		//获取主卡的概率 （第一次）
		if (_Index_ == 0)
		{
			//如果主卡格子无效   概率为0
			if (!LCardData)
			{
				this->M_UpGradeCardRateText = TEXT("0 %");
				return 0.f;
			}

			//获取主卡等级
			_MainCardGrade = LCardData->M_CardGrade;

			if (_MainCardGrade == 16)
				return 0.f;

			_Index_++;

			continue;
		}

		//副卡
		if (_Index_ != 0 && LCardData)
		{
			//当前概率
			float _L_Rate = 0.f;
			//当前卡槽副卡价格
			float _CardPrice = LCardData->M_CardPrice;
			//主卡减去副卡差
			int32 X = (_MainCardGrade - LCardData->M_CardGrade);

			//如果卡片特殊
			if (LCardData->M_SpecialCard_SynthesisUpGrade)
				_L_Rate = GetCardPriceGradeRate(600.0f, _MainCardGrade, X);
			else
				_L_Rate = GetCardPriceGradeRate(_CardPrice, _MainCardGrade, X);

			//将卡片概率添加到序列
			_L_CardRate.Emplace(_L_Rate);
		}
		else {
			//无效卡
			_L_CardRate.Emplace(0.f);
		}

	}

	//排序大小(概率)
	_L_CardRate.Sort([](const float& _A, const float& _B) {return _A > _B; });

	//设置概率
	uint8 _Index = 0u;

	if (UFVMGameInstance::GetDebug())
	{
		UE_LOG(LogTemp, Error, TEXT("[UI_PlayerSynthesis.cpp UUI_PlayerSynthesis::GetUpGradeRate]"));
		UE_LOG(LogTemp, Error, TEXT("-------------合成屋概率展示(降序)-------------"));
	}

	for (auto Item : _L_CardRate) {
		if (_Index == 0u)
		{
			//第一个最大的概率不衰减
			Rate += Item;
			_Index += 1u;

			if (UFVMGameInstance::GetDebug())
			{
				UE_LOG(LogTemp, Error, TEXT("卡片强化概率：%.2f"), Item);
			}
		}
		else {
			//剩下的概率衰减原来的 0.3
			Rate += Item * 0.3f;

			if (UFVMGameInstance::GetDebug())
			{
				UE_LOG(LogTemp, Error, TEXT("卡片强化概率：%.2f"), Item * 0.3f);
			}
		}
	}

	if (UFVMGameInstance::GetDebug())
	{
		UE_LOG(LogTemp, Error, TEXT("------------------------------------------------ \n\n"));
	}

	//是否有四叶草
	if (this->M_MakeCard_CloverGrid)
	{

		FCloverMaterial _Clover;
		if (UMaterialBaseStruct::GetMaterialSourceData<FCloverMaterial>(this->M_MakeCard_CloverGrid->GetMaterialData()->ItemName, _Clover, EMaterialType::E_Clover))
			//加上四叶草带来的祝福倍率
			Rate *= _Clover.M_UpGrateRate;
	}

	//限制概率100 显示100
	if (Rate > 100.f)
		Rate = 100.f;

	//显示概率文字
	this->M_UpGradeCardRateText = FString::Printf(TEXT("%.2f"), Rate) + TEXT("%");

	return Rate;
}
//【卡片强化数据】获取卡片强化升级的概率
float UUI_PlayerSynthesis::GetCardPriceGradeRate(float _Price, int32 _CardGrade, int32 MainCard_CurrentCard_Grade)
{
	float Rate = 0.f;

	//同星
	if (MainCard_CurrentCard_Grade == 0)
	{
		if (_Price < 100)
		{
			Rate = M_UpGradeRate_0_Arrays_0_Low[_CardGrade];
		}
		else if (_Price >= 100 && _Price < 200)
		{
			Rate = M_UpGradeRate_0_Arrays_0_Mid[_CardGrade];
		}
		else if (_Price >= 200)
		{
			Rate = M_UpGradeRate_0_Arrays_0_Hei[_CardGrade];
		}
	}//差1星
	else if (MainCard_CurrentCard_Grade == 1)
	{
		if (_Price < 100)
		{
			Rate = M_UpGradeRate_0_Arrays_1_Low[_CardGrade];
		}
		else if (_Price >= 100 && _Price < 200)
		{
			Rate = M_UpGradeRate_0_Arrays_1_Mid[_CardGrade];
		}
		else if (_Price >= 200)
		{
			Rate = M_UpGradeRate_0_Arrays_1_Hei[_CardGrade];
		}
	}//差2星
	else if (MainCard_CurrentCard_Grade == 2)
	{
		if (_Price < 100)
		{
			Rate = M_UpGradeRate_0_Arrays_2_Low[_CardGrade];
		}
		else if (_Price >= 100 && _Price < 200)
		{
			Rate = M_UpGradeRate_0_Arrays_2_Mid[_CardGrade];
		}
		else if (_Price >= 200)
		{
			Rate = M_UpGradeRate_0_Arrays_2_Hei[_CardGrade];
		}
	}
	else if (MainCard_CurrentCard_Grade > 3) // 差3直接0
	{
		return  0.f;
	}//比主高1
	else if (MainCard_CurrentCard_Grade == -1)
	{
		if (_Price < 100)
		{
			return M_UpGradeRate_0_Arrays_P1_Low[_CardGrade];
		}
		else if (_Price >= 100 && _Price < 200)
		{
			return M_UpGradeRate_0_Arrays_P1_Mid[_CardGrade];
		}
		else if (_Price >= 200)
		{
			return M_UpGradeRate_0_Arrays_P1_Hei[_CardGrade];
		}

	}//比主高2
	else if (MainCard_CurrentCard_Grade <= -2) {
		return 100.f;
	}

	return Rate;
}


//【共享界面】保险金的选择
void UUI_PlayerSynthesis::SelectInsurance()
{
	if (this->M_UpGrade_bSelectInsurance)
	{
		this->M_UpGrade_bSelectInsurance = false;
		UWidgetBase::SetButtonStyle(this->M_Insurance_Butt, "Texture2D'/Game/Resource/Texture/UI/Game/PlayerSynthesis/T_PS_26.T_PS_26'");
	}
	else {
		this->M_UpGrade_bSelectInsurance = true;
		UWidgetBase::SetButtonStyle(this->M_Insurance_Butt, "Texture2D'/Game/Resource/Texture/UI/Game/PlayerSynthesis/T_PS_27.T_PS_27'");
	}
}

//【卡片转职】取消所有的转职选择
void UUI_PlayerSynthesis::CancelSelectCardChangeJobs()
{
	this->M_CardChangeJobTargetName = "";
	this->M_ChangeCard_Rate = FString(TEXT("0 %"));
	this->M_Insurance_Coin_2_Text = FString(TEXT("0"));
	this->M_CardChangeJobMaterialsIndex.Empty();
	this->M_CardChangeJobMaterialsArrays.Empty();

	this->M_CurrentCardChangeJobIndex = -1;
	this->M_CardChangeJob_Data = nullptr;

	UWidgetBase::SetImageBrush(this->M_ChangeJob_0_Image, "Texture2D'/Game/Resource/Texture/UI/Game/PlayerSynthesis/T_PS_22.T_PS_22'");
	UWidgetBase::SetImageBrush(this->M_ChangeJob_1_Image, "Texture2D'/Game/Resource/Texture/UI/Game/PlayerSynthesis/T_PS_23.T_PS_23'");
	UWidgetBase::SetImageBrush(this->M_ChangeJob_2_Image, "Texture2D'/Game/Resource/Texture/UI/Game/PlayerSynthesis/T_PS_24.T_PS_24'");
	UWidgetBase::SetButtonStyle(this->M_ChangeJobMainCard_Butt, "Texture2D'/Game/Resource/Texture/UI/Game/PlayerSynthesis/T_PS_21.T_PS_21'");
}
//【卡片转职】卡片开始转职的功能
void UUI_PlayerSynthesis::CardChangeJobs()
{
	if (!this->M_CardChangeJob_Data)
	{
		UWidgetBase::CreateTipWidget(TEXT("请先选择需要转职的防御卡"), FVector(1.f, 0.f, 1.f));
		return;
	}


	//材料检测->材料不足则退出
	for (bool& B : this->M_CardChangeJobMaterialsArrays)
		if (!B)
		{
			UWidgetBase::CreateTipWidget(TEXT("缺少转职材料"));
			return;
		}

	//默认保险500
	int32 LPrice = this->M_CardChangeJob_Insurance[this->M_CardChangeJob_Data->M_FCardChangeJobs.M_ChangeGrade];

	//（是否开启保险金）
	if (this->M_UpGrade_bSelectInsurance)
	{
		//先确认金币是否充足
		if (UFVMGameInstance::GetPlayerStructManager_Static()->M_FPlayerCoin.M_Coin_0 >= 200)
		{
			if (UFVMGameInstance::GetPlayerStructManager_Static()->M_FPlayerCoin.M_Coin_2 >= LPrice)
			{
				//支付点券
				UFVMGameInstance::GetPlayerStructManager_Static()->ReduceCoin(LPrice, 2);
				//支付金币
				UFVMGameInstance::GetPlayerStructManager_Static()->ReduceCoin(200, 0);

			}
			else {
				UWidgetBase::CreateTipWidget(TEXT("点券不足"));
				return;
			}
		}
		else {
			UWidgetBase::CreateTipWidget(TEXT("金币不足"));
			return;
		}
	}
	else {
		//先确认金币是否充足
		if (UFVMGameInstance::GetPlayerStructManager_Static()->M_FPlayerCoin.M_Coin_0 >= 200)
		{
			//支付金币
			UFVMGameInstance::GetPlayerStructManager_Static()->ReduceCoin(200, 0);
		}
		else {
			UWidgetBase::CreateTipWidget(TEXT("金币不足"));
			return;
		}
	}

	//开始转职
	bool LResult = false;

	//转职结果
	if (LPrice <= 500)
	{
		LResult = UGameSystemFunction::GetRange(30, 101);
	}
	else
	{
		LResult = UGameSystemFunction::GetRange(20, 101);
	}

	auto Run = [&](FItemCard*& _Card, bool Result) ->bool {

		if (!_Card)
			return false;

		//查询材料（消除材料）
		for (int32& _Index : this->M_CardChangeJobMaterialsIndex)
		{
			UFVMGameInstance::GetFVMGameInstance()->GetPlayerStructManager()->M_PlayerItems_Material[_Index].M_Count--;
			if (UFVMGameInstance::GetFVMGameInstance()->GetPlayerStructManager()->M_PlayerItems_Material[_Index].M_Count == 0)
				UFVMGameInstance::GetFVMGameInstance()->GetPlayerStructManager()->M_PlayerItems_Material[_Index].bWaitRemove = true;
		}

		if (Result)
		{
			//创建所需卡片
			FItemCard CardData;
			UCardBaseStruct::SearchCardFromDataTable(this->M_CardChangeJobTargetName, CardData, true, _Card->M_FCardChangeJobs.M_ChangeType);
			CardData.M_CardGrade = _Card->M_CardGrade;

			*_Card = CardData;
		}

		return true;
	};

	if (LResult)
	{
		//消除道具
		if (Run(this->M_CardChangeJob_Data, true))
		{

			LResult = false;

			UWidgetBase::CreateTipWidget(TEXT("转职成功"), FVector(1.f, 0.f, 1.f));
		}
	}
	else {
		//失败
		if (!this->M_UpGrade_bSelectInsurance)//没开启了保险
		{
			//消除道具
			Run(this->M_CardChangeJob_Data, false);
		}

		LResult = true;

		UWidgetBase::CreateTipWidget(TEXT("转职失败"), FVector(1.f, 0.f, 1.f));
	}


	//材料查询->如果没有材料了->查询----重新加载
	//查询材料
	for (int32& _Index : this->M_CardChangeJobMaterialsIndex)
	{
		if (!UFVMGameInstance::GetFVMGameInstance()->GetPlayerStructManager()->M_PlayerItems_Material[_Index].bWaitRemove)
		{
			if (UFVMGameInstance::GetFVMGameInstance()->GetPlayerStructManager()->M_PlayerItems_Material[_Index].M_Count <= 0)
			{
				LResult = false;
				break;
			}
		}
		else {
			LResult = false;

			break;
		}
	}

	//如果转职成功 或者 转职失败没有开启保险
	if (!LResult)
	{
		//材料加载完成重置
		this->M_bMaterialFinish = false;
		//卡片加载完成重置
		this->M_bCardFinish = false;
		//禁用转职按钮
		this->M_ChangeJob_Butt->SetIsEnabled(false);
		//重新加载卡片列表
		this->LoadCardsToCardChange();
	}

	//清空待删除的道具
	UGameSystemFunction::ClearWaitingItems<FMaterialBase>(UFVMGameInstance::GetFVMGameInstance()->GetPlayerStructManager()->M_PlayerItems_Material);

	//更新货币文字显示
	this->Update_CoinText();

	//加载转职材料列表
	this->LoadMaterialsToCardChange();

	//保存游戏
	UGameSystemFunction::SaveCurrentPlayerData();
}


//【金卡进化】将一张被选择的卡片从金卡进化槽里面移出
void UUI_PlayerSynthesis::RemoveCardSlot(int32 _Index)
{
	if (_Index < 0 || _Index > 2)
		return;

	this->M_NeedCards[_Index] = "";
	this->M_GoldCard_Name_State[_Index] = "";
	this->M_NeedCard_UI_Index[_Index] = -1;
	this->M_GoldCard_Select_Button[_Index]->SetVisibility(ESlateVisibility::Collapsed);
	this->M_GoldCardUpHead->SetVisibility(ESlateVisibility::Collapsed);
	this->M_GoldCardUpResultHead->SetVisibility(ESlateVisibility::Collapsed);
	this->M_GoldCardUp_Butt->SetIsEnabled(false);

	this->LoadGoldCard_Card();
}
//【金卡进化】金卡进化 进化 功能
void UUI_PlayerSynthesis::GoldCardUp()
{
	if (UPlayerStructManager* Player = UFVMGameInstance::GetPlayerStructManager_Static())
	{
		//搜寻进化凭证
		if (FMaterialBase* GoldCardUpItemData = Player->GetMaterialByName(this->M_GoldCardUp_ItemName))
		{
			GoldCardUpItemData->M_Count--;
			if (GoldCardUpItemData->M_Count <= 0)
				GoldCardUpItemData->bWaitRemove = true;

			//删除道具
			UGameSystemFunction::ClearWaitingItems(Player->M_PlayerItems_Material);
		}

		//搜寻卡片
		for (auto CardPP = this->M_NeedCards.CreateConstIterator(); CardPP; ++CardPP)
		{
			//搜索对应卡片
			for (auto SourceCardData = Player->M_PlayerItems_Card.CreateIterator(); SourceCardData; ++SourceCardData)
			{
				if ((*SourceCardData).ItemName.Equals((*CardPP)) && (*SourceCardData).M_CardGrade == this->M_NeedGradeCards[CardPP.GetIndex()])
				{
					//直接移除当前元素
					SourceCardData.RemoveCurrent();
					//跳出
					break;
				}
			}
		}

		UWidgetBase::CreateTipWidget(TEXT("进化成功"));
		//直接发送新的卡片到背包
		UGameSystemFunction::SendCardToPlayerBag(this->M_GoldCardUp_CardName, this->M_NeedGradeCards[0] > this->M_NeedGradeCards[1] ? this->M_NeedGradeCards[0] : this->M_NeedGradeCards[1]);

		//取消卡片的选择
		this->M_NeedCard_UI_Index[0] = -1;
		this->M_NeedCard_UI_Index[1] = -1;

		//重新加载一次金卡进化
		this->ShowGoldCardUpPanel();
		this->LoadGoldCard_Card();
	}
}

USynModel_WepaonGems* UUI_PlayerSynthesis::GetWepaonGemsFunction()
{
	return this->M_USynModel_WepaonGems;
}

USynModel_GemsUpGrade* UUI_PlayerSynthesis::GetGemsUpGradeFunction()
{
	return this->M_USynModel_GemsUpGrade;
}

USynModel_GemSplit* UUI_PlayerSynthesis::GetGemSplitFunction()
{
	return this->M_USynModel_GemSplit;
}

//【材料加载】当材料区加载完毕后
void UUI_PlayerSynthesis::MaterialLoadEnd()
{
	//材料加载完成
	this->M_bMaterialFinish = true;

	//启用卡片制作按钮
	if (this->M_bCardFinish && this->M_bCardBlueprintFinish)
	{
		this->M_MakeCard_Run_Butt->SetIsEnabled(true);
	}

	//启用强化和转职按钮
	if (this->M_bCardFinish)
	{
		this->M_UpGradeCard_Butt->SetIsEnabled(true);
		this->M_ChangeJob_Butt->SetIsEnabled(true);
	}
}
//【卡片加载】当卡片区加载完毕后
void UUI_PlayerSynthesis::CardsLoadEnd()
{
	//卡片加载完成
	this->M_bCardFinish = true;

	//启用卡片制作按钮
	if (this->M_bMaterialFinish && this->M_bCardBlueprintFinish)
	{
		this->M_MakeCard_Run_Butt->SetIsEnabled(true);
	}

	//启用强化和转职按钮
	if (this->M_bMaterialFinish)
	{
		this->M_UpGradeCard_Butt->SetIsEnabled(true);
		this->M_ChangeJob_Butt->SetIsEnabled(true);
	}
}
//【材料加载】当材料区-配方材料-加载完毕后
void UUI_PlayerSynthesis::CardBlueprintsLoadEnd()
{
	//配方加载完成
	this->M_bCardBlueprintFinish = true;

	if (this->M_bMaterialFinish && this->M_bMaterialFinish)
	{
		this->M_MakeCard_Run_Butt->SetIsEnabled(true);
	}

}
//【选项卡选择界面】宝石强化选项卡
void UUI_PlayerSynthesis::Select_GemCardPanel()
{
	UGameSystemFunction::WidgetSelect<UWidget>(this->M_TabSelectPanels, ESlateVisibility::Visible, 0);

	this->ShowCardPanelOrInsurancePanel(ESlateVisibility::Collapsed, 0, ESlateVisibility::Collapsed);

	this->ShowMakeCard();
}
//【选项卡选择界面】金卡进化选项卡
void UUI_PlayerSynthesis::Select_GoldCardUpPanel()
{
	UGameSystemFunction::WidgetSelect<UWidget>(this->M_TabSelectPanels, ESlateVisibility::Visible, 1);

	this->ShowCardPanelOrInsurancePanel(ESlateVisibility::Visible, 0, ESlateVisibility::Collapsed);

	this->ShowGoldCardUpPanel();
	this->LoadMaterialsToCardChange();
	this->LoadGoldCard_Card();
}
//【更新显示】更新金币的文本
void UUI_PlayerSynthesis::Update_CoinText()
{
	this->M_Coin_0_Text = FString::FromInt(UFVMGameInstance::GetPlayerStructManager_Static()->M_FPlayerCoin.M_Coin_0);
	this->M_Coin_2_Text = FString::FromInt(UFVMGameInstance::GetPlayerStructManager_Static()->M_FPlayerCoin.M_Coin_2);
}
//【界面显示】显示保险金UI界面【卡片界面显示(武器的显示界面，卡片的显示界面)】
void UUI_PlayerSynthesis::ShowCardPanelOrInsurancePanel(ESlateVisibility _CardPanel_State, uint8 _CardPanel_ShowIndex, ESlateVisibility _InsurancePanel_State2, ESlateVisibility _ColoverPanel_State)
{
	this->M_CardsPanel_1->SetVisibility(_CardPanel_State);

	for (auto CurrentWidget : this->M_TabWidgets_Panels)
		CurrentWidget->SetVisibility(ESlateVisibility::Collapsed);
	this->M_TabWidgets_Panels[_CardPanel_ShowIndex]->SetVisibility(ESlateVisibility::Visible);

	this->M_CardsPanel_2->SetVisibility(_InsurancePanel_State2);
	this->M_ColoverPanel->SetVisibility(_ColoverPanel_State);
}

void UUI_PlayerSynthesis::SetRateText(const FString& _Text)
{
	this->M_UpGradeCardRateText = _Text;
}

void UUI_PlayerSynthesis::SetInsuranceText(const FString& _Text)
{
	this->M_Insurance_Coin_2_Text = _Text;
}

bool UUI_PlayerSynthesis::GetSelectInsurance()
{
	return this->M_UpGrade_bSelectInsurance;
}

void UUI_PlayerSynthesis::LoadMaterials_Type(const TArray<FMaterialsSerachTypeBind>& _BindFuncName)
{
	this->M_BindFunctionName_Materials.Empty();

	//赋值绑定数据
	this->M_BindFunctionName_Materials.Append(_BindFuncName);

	//材料数据(获取指定的材料数据)		
	TArray<FMaterialBase*> TempArrays;

	//获取类型
	TArray<EMaterialType> Types;
	for (const auto& _Type : _BindFuncName)
		Types.Emplace(_Type.M_Type);

	UGameSystemFunction::GetMaterialsArrayByType(UFVMGameInstance::GetFVMGameInstance()->GetPlayerStructManager()->M_PlayerItems_Material, Types, TempArrays);

	this->M_UItemLoadManager_Item_Material->UpdateDatatable(TempArrays);
	this->M_UItemLoadManager_Item_Material->SetLoadItemMaxCount(TempArrays.Num());
	this->M_UItemLoadManager_Item_Material->SetCurrentPage(0);
	this->M_UItemLoadManager_Item_Material->SetResetScrollBoxOffset();
	this->M_UItemLoadManager_Item_Material->ContinueRun();
}
