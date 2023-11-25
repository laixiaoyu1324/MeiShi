// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/UI/UI_PlayerBagCardGrid.h"
#include "Game/UI/UI_PlayerBagCardDetail.h"
#include "Game/UI/UI_ExchangeAndSplit.h"
#include "Game/UI/UI_GamePrepare.h"
#include "Game/UI/UI_PlayerBag.h"
#include "Game/UI/UI_PlayerSynthesis.h"
#include <Components/Image.h>
#include <Components/CanvasPanel.h>
#include <Math/Color.h>

bool UUI_PlayerBagCardGrid::Initialize()
{
	if (!Super::Initialize())
		return false;

	this->M_CardButton = this->GetWidgetComponent<UButton>(this, "Card");
	this->M_CardGradeImage = this->GetWidgetComponent<UImage>(this, "Grade");

	return true;
}

FOnButtonClickedEvent& UUI_PlayerBagCardGrid::GetButtonClickEvent()
{
	return this->M_CardButton->OnClicked;
}

void UUI_PlayerBagCardGrid::SetFItemCardData(FItemCard* const _FItemCardData)
{
	this->M_FItemCard = _FItemCardData;
}

void UUI_PlayerBagCardGrid::SetUI_PlayerSynthesis(UUI_PlayerSynthesis* const _PlayerSynthesis)
{
	this->M_UI_PlayerSynthesis = _PlayerSynthesis;
}

void UUI_PlayerBagCardGrid::SetUI_Other(UWidget* const _Widget)
{
	this->M_UI_Other = _Widget;
}

void UUI_PlayerBagCardGrid::SetCardIndex(const int32& _Value)
{
	this->M_CardIndex = _Value;
}

void UUI_PlayerBagCardGrid::SetUIIndex(const int32& _Value)
{
	this->M_UI_Index = _Value;
}

int32 UUI_PlayerBagCardGrid::GetCardIndex()
{
	return this->M_CardIndex;
}

int32 UUI_PlayerBagCardGrid::GetUIIndex()
{
	return this->M_UI_Index;
}

FItemCard* const UUI_PlayerBagCardGrid::GetFItemCardData()
{
	return this->M_FItemCard;
}

FString UUI_PlayerBagCardGrid::GetItemName()
{
	if (this->GetFItemCardData())
	{
		return this->GetFItemCardData()->ItemName;
	}

	return FString();
}

UButton* UUI_PlayerBagCardGrid::GetButton()
{
	return this->M_CardButton;
}

void UUI_PlayerBagCardGrid::ShowCardDetails()
{
	if (!this->GetFItemCardData())
		return;

	//细节界面
	UUI_PlayerBagCardDetail* M_UI_PlayerBagCardDetail = CreateWidget<UUI_PlayerBagCardDetail>(this, LoadClass<UUI_PlayerBagCardDetail>(0, TEXT("WidgetBlueprint'/Game/Resource/BP/Game/UI/MainFrame/BPUI_PlayerBagCardDetail.BPUI_PlayerBagCardDetail_C'")));
	M_UI_PlayerBagCardDetail->M_PlayerUIBag = this->M_PlayerUIBag;
	M_UI_PlayerBagCardDetail->M_FItemCard = this->M_FItemCard;
	M_UI_PlayerBagCardDetail->ItemName = M_FItemCard->ItemName;
	M_UI_PlayerBagCardDetail->ItemDescrible = M_FItemCard->ItemDescrible;
	M_UI_PlayerBagCardDetail->ItemDescrible_2 = M_FItemCard->ItemDescrible_2;
	M_UI_PlayerBagCardDetail->M_CardHP = FString(TEXT("生命值:")) + FString::FormatAsNumber(M_FItemCard->M_CardHP);
	M_UI_PlayerBagCardDetail->M_CardPrice = FString(TEXT("火苗:")) + FString::FormatAsNumber(M_FItemCard->M_CardPrice);
	M_UI_PlayerBagCardDetail->M_IsChange = M_FItemCard->M_FCardChangeJobs.M_IsChange ? FString(TEXT("转职:可以转职")) : FString(TEXT("转职:不可转职"));
	M_UI_PlayerBagCardDetail->M_ChangeMaterialsName = FString(TEXT("转职对象:")) + M_FItemCard->M_FCardChangeJobs.M_ChangeMaterialsName;
	M_UI_PlayerBagCardDetail->M_CardColdDown = FString(TEXT("冷却时间:")) + FString::SanitizeFloat(M_FItemCard->M_CardColdDown, 2);
	M_UI_PlayerBagCardDetail->AddToViewport();
}

void UUI_PlayerBagCardGrid::SelectCurrentCard()
{
	if (UUI_GamePrepare::M_GamePrepareStatic)
	{
		if (UUI_GamePrepare::M_GamePrepareStatic->GetSelectCardNums() > 17)
		{
			UWidgetBase::CreateTipWidget(TEXT("最多选择18张防御卡"));
			return;
		}

		UUI_GamePrepare::M_GamePrepareStatic->SelectCard(this->M_FItemCard->ItemName, *M_FItemCard);
		UUI_GamePrepare::M_GamePrepareStatic->M_CardDatas_Copy.Emplace(*this->M_FItemCard);
	}
}

void UUI_PlayerBagCardGrid::RemoveCurrentSelectCard()
{
	if (UUI_GamePrepare::M_GamePrepareStatic)
	{
		this->RemoveFromParent();

		//调用代理
		this->OnRemoveCurrentSelectCard();

		for (auto FCardDataPP = UUI_GamePrepare::M_GamePrepareStatic->M_CardDatas_Copy.CreateIterator(); FCardDataPP; FCardDataPP++)
		{
			if (this->M_FItemCard->ItemName.Equals((*FCardDataPP).ItemName))
			{
				FCardDataPP.RemoveCurrent();
				break;
			}
		}

		TArray<FString> Names = { M_FItemCard->ItemName };
		UUI_GamePrepare::M_GamePrepareStatic->SetCardEnable(Names, true);
		UUI_GamePrepare::M_GamePrepareStatic->CancelCardNum();
	}
}

void UUI_PlayerBagCardGrid::AddSynthesisUpGrade(bool ForceIndex, const int32& Index)
{
	if (!this->M_UI_PlayerSynthesis)
	{
		return;
	}

	int32 _Use_Index = 0;
	bool _IsUse = false;

	//顺序选择槽位
	if (!ForceIndex)
	{
		//判断卡槽是否可用
		for (bool& Number : this->M_UI_PlayerSynthesis->M_UpGradeCardState)
		{
			if (!Number)
			{
				Number = true;
				_IsUse = true;
				break;
			}
			_Use_Index++;
		}

		//如果不能使用退出
		if (!_IsUse)
		{
			return;
		}
	}

	auto SetImage = [&](UImage* _img)->void {
		_img->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		UWidgetBase::SetImageBrush(_img, UGameSystemFunction::GetCardGradeImagePath(this->M_FItemCard->M_CardGrade));
	};

	//强制选择槽位
	if (ForceIndex)
	{
		_Use_Index = Index;
		this->M_UI_PlayerSynthesis->M_UpGradeCardState[_Use_Index] = true;
	}

	//主卡
	if (_Use_Index == 0)
	{
		this->M_UI_PlayerSynthesis->_CardUpgrate_Price_0 = FString::FromInt(this->M_FItemCard->M_CardPrice);
		SetImage(this->M_UI_PlayerSynthesis->M_UpGradeCard_Main_Grade);

		//卡片等级需要的保险金金额(更新保险金所需金额)
		TArray<int32> LInsurance_CoinNum = { 0,20,40,80,100,200,500,500,600,1000,3000,10000,18000,32000,45000,50000 ,0 };
		//更新保险金 金额
		this->M_UI_PlayerSynthesis->M_Insurance_Coin_2_Text = FString::FromInt(LInsurance_CoinNum[this->M_FItemCard->M_CardGrade]);

		this->M_UI_PlayerSynthesis->M_UpGradeCard_Datas[0] = this->M_FItemCard;
		this->M_UI_PlayerSynthesis->M_UpGradeCard_UI_Index[0] = this->M_CardIndex;
		this->M_UI_PlayerSynthesis->M_MainCard_Data_Index = this->M_CardIndex;
	}
	else if (_Use_Index == 1)//副卡1
	{
		this->M_UI_PlayerSynthesis->_CardUpgrate_Price_1 = FString::FormatAsNumber(this->M_FItemCard->M_CardPrice);
		SetImage(this->M_UI_PlayerSynthesis->M_UpGradeCard_0_Grade);

		this->M_UI_PlayerSynthesis->M_UpGradeCard_Datas[1] = this->M_FItemCard;
		this->M_UI_PlayerSynthesis->M_UpGradeCard_UI_Index[1] = this->M_CardIndex;
	}
	else if (_Use_Index == 2)
	{
		this->M_UI_PlayerSynthesis->_CardUpgrate_Price_2 = FString::FormatAsNumber(this->M_FItemCard->M_CardPrice);
		SetImage(this->M_UI_PlayerSynthesis->M_UpGradeCard_1_Grade);

		this->M_UI_PlayerSynthesis->M_UpGradeCard_Datas[2] = this->M_FItemCard;
		this->M_UI_PlayerSynthesis->M_UpGradeCard_UI_Index[2] = this->M_CardIndex;
	}
	else if (_Use_Index == 3)//副卡3
	{
		this->M_UI_PlayerSynthesis->_CardUpgrate_Price_3 = FString::FormatAsNumber(this->M_FItemCard->M_CardPrice);
		SetImage(this->M_UI_PlayerSynthesis->M_UpGradeCard_2_Grade);

		this->M_UI_PlayerSynthesis->M_UpGradeCard_Datas[3] = this->M_FItemCard;
		this->M_UI_PlayerSynthesis->M_UpGradeCard_UI_Index[3] = this->M_CardIndex;
	}

	UWidgetBase::SetButtonStyle(this->M_UI_PlayerSynthesis->M_UpGradeSlot[_Use_Index], this->M_FItemCard->ItemTexturePath.ToString());
	this->GetButton()->SetIsEnabled(false);

	//计算概率
	this->M_UI_PlayerSynthesis->GetUpGradeRate();
}

void UUI_PlayerBagCardGrid::AddSynthesisUpGradeFunc()
{
	this->AddSynthesisUpGrade(false, 0);
}

void UUI_PlayerBagCardGrid::AddSynthesisChangeJobs()
{
	if (!this->M_UI_PlayerSynthesis)
		return;

	//赋予当前卡片的索引
	this->M_UI_PlayerSynthesis->M_CurrentCardChangeJobIndex = this->M_UI_Index;
	//设置卡片数据
	this->M_UI_PlayerSynthesis->M_CardChangeJob_Data = this->M_FItemCard;
	//将转职目标的卡片赋予合成屋成员 临时保存
	this->M_UI_PlayerSynthesis->M_CardChangeJobTargetName = this->M_FItemCard->M_FCardChangeJobs.M_ChangeMaterialsName;


	//显示概率
	if (this->M_FItemCard->M_FCardChangeJobs.M_ChangeGrade < 12)
	{
		this->M_UI_PlayerSynthesis->M_ChangeCard_Rate = FString(TEXT("30 %"));
	}
	else
	{
		this->M_UI_PlayerSynthesis->M_ChangeCard_Rate = FString(TEXT("20 %"));
	}


	//计算保险的价格 	//更新价格文字
	int32 LPrice = this->M_UI_PlayerSynthesis->M_CardChangeJob_Insurance[this->GetFItemCardData()->M_FCardChangeJobs.M_ChangeGrade];
	this->M_UI_PlayerSynthesis->M_Insurance_Coin_2_Text = FString::FromInt(LPrice);

	//从背包寻找材料
	TArray<FMaterialBase>& _Materials = UFVMGameInstance::GetFVMGameInstance()->GetPlayerStructManager()->M_PlayerItems_Material;
	//需要的材料名称->每当查询成功一次就会移除一位元素剩余的则是未查询成功的
	TArray<FString> _NeedMaterialsName = this->M_FItemCard->M_FCardChangeJobs.M_ChangeMaterials;
	//物品的名称
	TMap<FString, bool> _MaterialsName;
	//图片路径
	TArray<FString> _MaterialsTexturePath_;

	//获取图片路径
	FMaterialBase Data;
	int32 M_Count = this->M_FItemCard->M_FCardChangeJobs.M_ChangeMaterials.Num();

	//寻找并添加图片路径
	for (int32 I = 0; I < M_Count; I++)
	{
		UMaterialBaseStruct::SearchMaterailFromDataTable(this->M_FItemCard->M_FCardChangeJobs.M_ChangeMaterials[I], Data, true, EMaterialType::E_CardChangeJobMaterial);
		_MaterialsName.Emplace(Data.ItemName, false);
		_MaterialsTexturePath_.Emplace(Data.ItemTexturePath.ToString());
	}

	//计数索引
	int32 _ItemIndex = 0;
	//查询计数
	int32 _ItemSuccessCount = 0;
	//重置状态
	this->M_UI_PlayerSynthesis->M_CardChangeJobMaterialsArrays.Empty();
	this->M_UI_PlayerSynthesis->M_CardChangeJobMaterialsIndex.Empty();

	for (FMaterialBase& Item : _Materials)
	{
		for (auto PP = _NeedMaterialsName.CreateConstIterator(); PP; PP++)
		{
			if (Item.ItemName.Equals(*PP))
			{
				//添加状态
				this->M_UI_PlayerSynthesis->M_CardChangeJobMaterialsArrays.Emplace(true);
				//添加索引
				this->M_UI_PlayerSynthesis->M_CardChangeJobMaterialsIndex.Emplace(_ItemIndex);
				//修改图片状态
				*(_MaterialsName.Find(Item.ItemName)) = true;
				//移除
				_NeedMaterialsName.RemoveAt(PP.GetIndex());
				//更新计数
				_ItemSuccessCount++;
				break;
			}
		}

		//当材料全部被找到之后将直接退出循环
		if (_NeedMaterialsName.Num() == 0)
			break;

		_ItemIndex++;
	}

	//添加剩余状态
	for (FString Name : _NeedMaterialsName)
		this->M_UI_PlayerSynthesis->M_CardChangeJobMaterialsArrays.Emplace(false);

	//显示转职C界面
	if (this->M_FItemCard->M_FCardChangeJobs.M_ChangeMaterials.Num() > 2)
		this->M_UI_PlayerSynthesis->M_ChangeJob_2_P->SetVisibility(ESlateVisibility::Visible);
	else
		this->M_UI_PlayerSynthesis->M_ChangeJob_2_P->SetVisibility(ESlateVisibility::Hidden);

	//设置图片样式
	auto SetImage = [&](UImage* _Img, int32 _Index)->void {
		bool* State = _MaterialsName.Find(this->M_FItemCard->M_FCardChangeJobs.M_ChangeMaterials[_Index]);
		FString Path = _MaterialsTexturePath_[_Index];
		UWidgetBase::SetImageBrush(_Img, Path, *State ? FVector(1.f) : FVector(0.5f));
	};

	//设置第一张图片
	if (M_Count > 0)
		SetImage(this->M_UI_PlayerSynthesis->M_ChangeJob_0_Image, 0);
	//设置第二张图片
	if (M_Count > 1)
		SetImage(this->M_UI_PlayerSynthesis->M_ChangeJob_1_Image, 1);
	//设置第三张图片
	if (M_Count > 2)
		SetImage(this->M_UI_PlayerSynthesis->M_ChangeJob_2_Image, 2);

	//设置主卡按钮的图片
	UWidgetBase::SetButtonStyle(this->M_UI_PlayerSynthesis->M_ChangeJobMainCard_Butt, this->M_FItemCard->ItemTexturePath.ToString());


	this->M_CardButton->SetIsEnabled(false);

	//重新刷新列表
	this->M_UI_PlayerSynthesis->M_UItemLoadManager_CardUpGrade->ContinueRun();
}

void UUI_PlayerBagCardGrid::AddSynthesisGoldCardUp()
{
	int32 LResult = 0;
	//判断卡片名字是否已经填写
	for (auto& State : this->M_UI_PlayerSynthesis->M_GoldCard_Name_State)
	{
		if (!State.Equals(""))
		{
			LResult++;
		}
	}

	if (LResult == 2)
	{
		UWidgetBase::CreateTipWidget(TEXT("无法选择,卡槽已满"));
		return;
	}

	//隐藏
	for (auto State = this->M_UI_PlayerSynthesis->M_GoldCard_Name_State.CreateConstIterator(); State; ++State)
	{
		if ((*State).Equals(""))
			this->M_UI_PlayerSynthesis->M_GoldCard_Select_Button[State.GetIndex()]->SetVisibility(ESlateVisibility::Collapsed);
	}


	this->GetButton()->SetIsEnabled(false);

	//隐藏其他的道具(因为材料不齐全所以需要隐藏)
	this->M_UI_PlayerSynthesis->M_GoldCardUpHead->SetVisibility(ESlateVisibility::Collapsed);
	this->M_UI_PlayerSynthesis->M_GoldCardUpResultHead->SetVisibility(ESlateVisibility::Collapsed);
	//关闭进化按钮(因为材料不齐全所以需要关闭进化按钮)
	this->M_UI_PlayerSynthesis->M_GoldCardUp_Butt->SetIsEnabled(false);


	if (UFVMGameInstance::GetDebug())
		UGameSystemFunction::FVMLog(__FUNCTION__, TEXT("当前金卡进化防御卡卡槽名称："));

	//设置卡片卡槽名字
	for (auto State = this->M_UI_PlayerSynthesis->M_GoldCard_Name_State.CreateConstIterator(); State; ++State)
	{
		//判断当前卡槽是否是空的
		if (this->M_UI_PlayerSynthesis->M_GoldCard_Name_State[State.GetIndex()].Equals(""))
		{
			//添加名字
			this->M_UI_PlayerSynthesis->M_GoldCard_Name_State[State.GetIndex()] = this->GetFItemCardData()->ItemName;
			//设置卡槽
			UWidgetBase::SetButtonStyle(this->M_UI_PlayerSynthesis->M_GoldCard_Select_Button[State.GetIndex()], this->GetFItemCardData()->ItemTexturePath.ToString());
			//开启卡槽
			this->M_UI_PlayerSynthesis->M_GoldCard_Select_Button[State.GetIndex()]->SetVisibility(ESlateVisibility::Visible);
			//添加被消耗的卡片
			this->M_UI_PlayerSynthesis->M_NeedCards[State.GetIndex()] = this->GetFItemCardData()->ItemName;
			//添加卡片等级
			this->M_UI_PlayerSynthesis->M_NeedGradeCards[State.GetIndex()] = this->GetFItemCardData()->M_CardGrade;
			//添加卡片对应的UI索引
			this->M_UI_PlayerSynthesis->M_NeedCard_UI_Index[State.GetIndex()] = this->GetUIIndex();

			if (UFVMGameInstance::GetDebug())
				UGameSystemFunction::FVMLog(__FUNCTION__, this->M_UI_PlayerSynthesis->M_GoldCard_Name_State[State.GetIndex()]);

			break;
		}
	}

	//卡片名称合并查询->进化凭证
	TArray<FMaterial_GoldCardUp_Data> Datas;
	UGameSystemFunction::GetDataTableRows(TEXT("DataTable'/Game/Resource/BP/Data/EquipmentData/GoldCardUpData.GoldCardUpData'"), Datas);

	//最后从背包搜寻的金卡转职凭证
	FString LResultItemName = TEXT("");
	//对应的卡片名称
	FString LResultCardName = TEXT("");

	if (UFVMGameInstance::GetDebug())
		UGameSystemFunction::FVMLog(__FUNCTION__, TEXT("当前金卡进化查询进化凭证："));

	for (auto Data = Datas.CreateConstIterator(); Data; ++Data)
	{
		bool TLResult = true;

		FString StateResult = TEXT("");

		//搜寻凭证
		for (auto& State : this->M_UI_PlayerSynthesis->M_GoldCard_Name_State)
		{
			StateResult += State;
		}

		if (UFVMGameInstance::GetDebug())
		{
			FString Content;
			Content += TEXT("当前凭证名称:");
			Content += (*Data).M_GoldCardItemName;
			Content += TEXT(" 当前合成名字结果:");
			Content += StateResult;
			Content += TEXT(" 目标合成名称结果:");
			Content += (*Data).M_AddCardName;
			UGameSystemFunction::FVMLog(__FUNCTION__, Content);
		}

		//查询匹配最终结果
		if ((*Data).M_AddCardName.Equals(StateResult))
		{
			LResultItemName = (*Data).M_GoldCardItemName;
			LResultCardName = (*Data).M_GoldCardName;

			break;
		}
	}

	//没有任何结果，返回
	if (LResultItemName.Equals(""))
		return;

	//从背包搜索->玩家是否含有进化凭证
	if (FMaterialBase* LMaterialData = UFVMGameInstance::GetPlayerStructManager_Static()->GetMaterialByName(LResultItemName))
	{
		//查询卡片
		FItemCard CardData;
		if (UCardBaseStruct::SearchCardFromDataTable(LResultCardName, CardData))
		{
			UWidgetBase::SetImageBrush(this->M_UI_PlayerSynthesis->M_GoldCardUpResultHead, CardData.ItemTexturePath.ToString());
			this->M_UI_PlayerSynthesis->M_GoldCardUpResultHead->SetVisibility(ESlateVisibility::Visible);
		}

		UWidgetBase::SetImageBrush(this->M_UI_PlayerSynthesis->M_GoldCardUpHead, LMaterialData->ItemTexturePath.ToString());

		this->M_UI_PlayerSynthesis->M_GoldCardUpHead->SetVisibility(ESlateVisibility::Visible);

		//开启进化按钮
		this->M_UI_PlayerSynthesis->M_GoldCardUp_Butt->SetIsEnabled(true);
		//赋予对应的数值
		this->M_UI_PlayerSynthesis->M_GoldCardUp_ItemName = LResultItemName;
		this->M_UI_PlayerSynthesis->M_GoldCardUp_CardName = LResultCardName;
	}
}

void UUI_PlayerBagCardGrid::AddExchangeSplit_SplitSlot()
{
	if (IsValid(this->M_UI_Other))
	{
		if (UUI_ExchangeAndSplit* const LExUI = Cast<UUI_ExchangeAndSplit>(this->M_UI_Other))
		{
			if (IsValid(LExUI->GetCurrentSelectButt()))
			{
				LExUI->GetCurrentSelectButt()->SetIsEnabled(true);
			}

			UItemDataTable* Data = NewObject<UItemDataTable>(this);
			Data->SetValue((FTableRowBase*)(this->M_FItemCard));
			LExUI->SetCurrentSlipItem(EItemType::E_Card, this->GetUIIndex(), Data, this->GetButton());
			this->GetButton()->SetIsEnabled(false);
		}
	}
}

void UUI_PlayerBagCardGrid::UpdateButtonTexture(const FString& _Price)
{
	//等级资源
	if (this->M_FItemCard->M_CardGrade != 0)
	{
		FSlateBrush GradeBrush;
		GradeBrush.SetResourceObject(UAssetManager::GetStreamableManager().LoadSynchronous(FSoftObjectPath(FString("Texture2D'/Game/Resource/Texture/CardGrade/") + FString::FromInt(this->M_FItemCard->M_CardGrade) + (".") + FString::FormatAsNumber(this->M_FItemCard->M_CardGrade) + ("'"))));
		GradeBrush.DrawAs = ESlateBrushDrawType::Image;
		this->M_CardGradeImage->SetBrush(GradeBrush);
	}

	//资源
	FSlateBrush Brush;
	Brush.SetResourceObject(this->LoadTexture2D(this->M_CardTexturePath));
	//设置按钮不同事件时图片颜色深浅
	FButtonStyle Style;
	Style.SetNormal(Brush);

	Brush.TintColor = FLinearColor(FVector4(.7f, .7f, .7f, 1.f));
	Style.SetHovered(Brush);

	Brush.TintColor = FLinearColor(FVector4(.5f, .5f, .5f, 1.f));
	Style.SetPressed(Brush);

	this->M_CardButton->SetStyle(Style);
	this->M_CardPriceText = M_FItemCard->M_CardPriceAutoUp ? FText::FromString(_Price + "+") : FText::FromString(_Price);

	//如果卡片等级为0则不显示
	if (this->M_FItemCard->M_CardGrade != 0)
		this->M_CardGradeImage->SetVisibility(ESlateVisibility::Visible);
	else
		this->M_CardGradeImage->SetVisibility(ESlateVisibility::Hidden);
}

UTexture2D* UUI_PlayerBagCardGrid::GetTexture()
{
	return Cast<UTexture2D>(UAssetManager::GetStreamableManager().LoadSynchronous(FSoftObjectPath(this->M_CardTexturePath)));
}

void UUI_PlayerBagCardGrid::PlayBeginAnim()
{
	if (this->GetWidgetTreeOwningClass()->Animations.Num())
	{
		this->PlayAnimation(this->GetWidgetTreeOwningClass()->Animations[0]);
	}
}
