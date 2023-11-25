// Fill out your copyright notice in the Description page of Project Settings.


#include "GameStart/VS/UI/UI_CardBar.h"

#include "GameStart/VS/UI/UI_EradicateCard.h"

#include <Components/HorizontalBox.h>
#include <Components/VerticalBox.h>
#include <Components/Image.h>
#include <Components/Textblock.h>

#include <Components/CanvasPanel.h>
#include <Components/CanvasPanelSlot.h>
#include <Components/ScrollBox.h>
#include "Game/UI/UI_PlayerShow.h"
#include "GameSystem/Tools/GameSystemFunction.h"
#include<UMG/Public/Blueprint/WidgetLayoutLibrary.h>
#include <kismet/KismetMathLibrary.h>


bool UUI_CardBar::Initialize()
{
	if (!Super::Initialize())
		return false;

	this->M_UHorizontalBox_Cards = this->GetWidgetComponent<UHorizontalBox>(this, TEXT("HorizontalBox_55"));
	this->M_UVerticalBox_Cards = this->GetWidgetComponent<UVerticalBox>(this, TEXT("VerticalBox_65"));
	this->M_Bg_1 = this->GetWidgetComponent<UImage>(this, TEXT("Bg_Len_1"));
	this->M_Bg_2 = this->GetWidgetComponent<UImage>(this, TEXT("Bg_Len_2"));
	this->M_Bg_3 = this->GetWidgetComponent<UImage>(this, TEXT("Bg_"));
	this->M_TCard_Item_2 = this->GetWidgetComponent<UUI_EradicateCard>(this, TEXT("BPUI_EradicateCard_3"));
	this->M_UCanvasPanel_CardSlot = this->GetWidgetComponent<UCanvasPanel>(this, TEXT("Left"));

	this->PlayerPoint = this->GetWidgetComponent<UCanvasPanel>(this, TEXT("PlayerPoint"));
	this->PlayerGrade = this->GetWidgetComponent<UImage>(this, TEXT("CurPlayerGrade"));
	this->PlayerName = this->GetWidgetComponent<UTextBlock>(this, TEXT("PlayerName"));


	return true;
}

void UUI_CardBar::Init()
{
	UUI_PlayerShow* PlayerInfor = CreateWidget<UUI_PlayerShow>(
		this,
		LoadClass<UUI_PlayerShow>(
			nullptr,
			TEXT("WidgetBlueprint'/Game/Resource/BP/Game/UI/MainFrame/BP_PlayerShow.BP_PlayerShow_C'")
			)
		);


	if (UFVMGameInstance::GetPlayerStructManager_Static()->M_FPlayerSuit.M_bPlayerSuit
		&&
		UFVMGameInstance::GetPlayerStructManager_Static()->M_FPlayerSuit.M_ShowPlayerSuit
		)
	{
		PlayerInfor->SetRenderScale(FVector2D(0.85f));
		PlayerInfor->SetRenderTranslation(FVector2D(0.f,-15.f));
	}

	this->PlayerPoint->AddChildToCanvas(PlayerInfor);
	this->PlayerGrade->SetBrushFromTexture(LoadObject<UTexture2D>(0, *UGameSystemFunction::GetPlayerGradeImagePath()));
	this->PlayerName->SetText(FText::FromString(UFVMGameInstance::GetPlayerStructManager_Static()->M_PlayerName));
}

void UUI_CardBar::AddCard(UWidget* _Wid)
{
	//横向最多12张
	if (this->M_UHorizontalBox_Cards->GetChildrenCount() < 12)
	{
		//max x 3385 y 33
		//min x 280  y 33

		//max posX  1796.0
		//min posX  370.0

		float ValueRate = this->M_UHorizontalBox_Cards->GetChildrenCount() / 12.f;

		//设置图片长度
		double Hori_X_Value = UKismetMathLibrary::Lerp(360.f, 3385.f, ValueRate);
		UWidgetBase::WidgetToCanvasSlot(M_Bg_1)->SetSize(FVector2D(Hori_X_Value, 33.f));

		//设置铲子背景
		UWidgetBase::SetImageBrush(M_TCard_Item_2->GetBG(), TEXT("Texture2D'/Game/Resource/Texture/UI/GameVS/UIMouseRound/Card/1016.1016'"));
		//设置铲子的位置
		UWidgetBase::WidgetToCanvasSlot(M_TCard_Item_2)->SetPosition(FVector2D(134.f + -130.f * (12 - this->M_UHorizontalBox_Cards->GetChildrenCount()), -25.f));

		this->M_UHorizontalBox_Cards->AddChildToHorizontalBox(_Wid);
	}
	else {
		//打开拐角
		this->M_Bg_3->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		//打开第二条垂直线
		this->M_Bg_2->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

		//设置图片长度->水平长度
		UWidgetBase::WidgetToCanvasSlot(M_Bg_1)->SetSize(FVector2D(3385.f, 33.f));

		//设置图片长度->垂直长度
		float ValueRate = this->M_UVerticalBox_Cards->GetChildrenCount() / 6.f;
		//设置图片高度
		double Verit_Y_Value = UKismetMathLibrary::Lerp(190.f, 1260.f, ValueRate);
		UWidgetBase::WidgetToCanvasSlot(M_Bg_2)->SetSize(FVector2D(32.f, Verit_Y_Value));
		//设置铲子背景
		UWidgetBase::SetImageBrush(M_TCard_Item_2->GetBG(), TEXT("Texture2D'/Game/Resource/Texture/UI/GameVS/UIMouseRound/Card/1019.1019'"));
		//设置铲子的位置
		UWidgetBase::WidgetToCanvasSlot(M_TCard_Item_2)->SetPosition(FVector2D(10.f, 170.f + 170.f * this->M_UVerticalBox_Cards->GetChildrenCount()));

		this->M_UVerticalBox_Cards->AddChildToVerticalBox(_Wid);

		//如果达到最大值(使用铲子3)
		if (this->M_UVerticalBox_Cards->GetChildrenCount() == 6)
		{
			//设置铲子背景
			UWidgetBase::SetImageBrush(M_TCard_Item_2->GetBG(), TEXT("Texture2D'/Game/Resource/Texture/UI/GameVS/UIMouseRound/Card/1022.1022'"));
			//设置铲子的位置
			UWidgetBase::WidgetToCanvasSlot(M_TCard_Item_2)->SetPosition(FVector2D(-1715.f, 140.f));
		}
	}
}