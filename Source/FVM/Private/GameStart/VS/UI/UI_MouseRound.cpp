// Fill out your copyright notice in the Description page of Project Settings.


#include "GameStart/VS/UI/UI_MouseRound.h"

#include <Components/Slider.h>
#include <Components/TextBlock.h>
#include <Components/Image.h>
#include <Components/CanvasPanel.h>
#include <Components/ProgressBar.h>
#include <Components/VerticalBox.h>
#include <Components/CanvasPanelSlot.h>
#include <Kismet/KismetMathLibrary.h>

bool UUI_MouseRound::Initialize()
{
	if (!Super::Initialize())
		return false;

	this->M_USlider = this->GetWidgetComponent<USlider>(this, "MouseRoundSlider");
	this->M_MouseRoundText = this->GetWidgetComponent<UTextBlock>(this, "MouseRoundText");
	this->M_Boss_List_VerticalBox = this->GetWidgetComponent<UVerticalBox>(this, "Boss_HP_List");
	this->M_UProgressBar = this->GetWidgetComponent<UProgressBar>(this, "Progress");
	this->M_FlagBox = this->GetWidgetComponent<UCanvasPanel>(this, "RoundFlag");


	return true;
}

void UUI_MouseRound::InitFlag(const int32& _RoundTotal)
{
	for (int32 i = 0; i < _RoundTotal; i++)
	{
		// WidgetBlueprint'/Game/Resource/BP/GameStart/VS/UI_Player/Tips/BPUI_MouseRoundFlag.BPUI_MouseRoundFlag_C'

		UWidgetBase* CurFlag = CreateWidget<UWidgetBase>(
			this,
			LoadClass<UWidgetBase>(
				0,
				TEXT("WidgetBlueprint'/Game/Resource/BP/GameStart/VS/UI_Player/Tips/BPUI_MouseRoundFlag.BPUI_MouseRoundFlag_C'")
				)
			);

		UCanvasPanelSlot* CurSlot = this->M_FlagBox->AddChildToCanvas(CurFlag);

		CurSlot->SetSize(FVector2D(24.f, 29.f));
		//设置位置
		CurFlag->SetRenderTranslation(FVector2D(UKismetMathLibrary::Lerp(0.f, 280.f, 1.f - (i * 1.f) / _RoundTotal), 0));
	}
}

void UUI_MouseRound::UpdateMouseRoundSlider(const float& _Curr, const float& _End)
{
	this->M_USlider->SetValue(1.f - _Curr / _End);
	this->M_UProgressBar->SetPercent(_Curr / _End);
}

void UUI_MouseRound::UpdateMouseRoundText(const int32& _Round)
{
	this->M_MouseRoundText->SetText(FText::FromString(FString::FromInt(_Round)));
}

void UUI_MouseRound::AddMouseBossSlider(UWidget* _Widget_)
{
	if (!_Widget_)
		return;

	this->M_Boss_List_VerticalBox->AddChildToVerticalBox(_Widget_);
	this->M_BossSlider_Count++;
}

bool UUI_MouseRound::RemoveMouseBossSlider(UWidget* _Widget_)
{
	if (!_Widget_)
		return false;

	if (this->M_Boss_List_VerticalBox->RemoveChild(_Widget_))
	{
		this->M_BossSlider_Count--;

		return true;
	}

	return false;
}

int32 UUI_MouseRound::GetMouseBossSliderCount()
{
	return this->M_BossSlider_Count;
}
