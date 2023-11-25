// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除


#include "GameStart/VS/UI/UI_VSCountDown.h"
#include "GameStart/VS/Components/VSManagerComponent.h"
#include "GameStart/VS/Components/MouseManagerComponent.h"
#include "GameStart/VS/GameMapInstance.h"
#include <Components/TextBlock.h>


bool UUI_VSCountDown::Initialize()
{
	if (!Super::Initialize())
		return false;

	this->TimeText = this->GetWidgetComponent<UTextBlock>(this, TEXT("TimeText"));

	return true;
}

void UUI_VSCountDown::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (AGameMapInstance::GetGameMapInstance()->GetGamePause())
		return;

	if (this->bEnable)
	{
		this->Update(InDeltaTime);
	}

}

void UUI_VSCountDown::Enable(const float& _time)
{
	this->bEnable = true;
	this->Times = _time;
	this->UpdateText();
}

void UUI_VSCountDown::DisEnable()
{
	this->bEnable = false;
}

void UUI_VSCountDown::Update(const float& _Tick)
{
	if (this->CurTime < 1.f)
	{
		this->CurTime += _Tick;
	}
	else {
		this->CurTime = 0.f;

		this->Times--;

		this->UpdateText();
	}

	if (this->Times <= 0)
	{
		//判断是否还有老鼠存在，是否是最后一个回合
		if (
			AGameMapInstance::GetMouseManagerComponent_Static()->IsMouseSpawnEnd()
			&&
			!AGameMapInstance::GetMouseManagerComponent_Static()->IsMouseExist()
			)
		{
			UVSManagerComponent::GameWin();
		}
		else {
			UVSManagerComponent::GameFailed();
		}

	}
}

void UUI_VSCountDown::UpdateText()
{
	int32 CurTimeNode = this->Times / 60;

	//分钟
	int32 CurMin = CurTimeNode * 60;
	//秒
	int32 CurSecond = this->Times - CurMin;

	//得到时间字符串
	FString LTime = FString(FString::FromInt(CurMin) + TEXT(" : ") + FString::FromInt(CurSecond));
	this->TimeText->SetText(FText::FromString(LTime));
}
