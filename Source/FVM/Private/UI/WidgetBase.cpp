// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgetBase.h"

#include "GameSystem/Tools/GameSystemFunction.h"
#include "GameSystem/FVMGameInstance.h"
#include "GameSystem/GameConfigManager.h"
#include <Components/Image.h>

#include "Engine/StreamableManager.h"
#include "Engine/AssetManager.h"

#include <Blueprint/WidgetLayoutLibrary.h>
#include <Kismet/KismetMathLibrary.h>
#include <Kismet/GameplayStatics.h>
#include <Kismet/KismetStringLibrary.h>
#include <Components/RichTextBlock.h>
#include <Components/CanvasPanelSlot.h>
#include <Components/SizeBox.h>

#include "Engine/Texture2D.h"
#include "Game/UI/Tips/UI_Tip.h"
#include "Game/UI/Tips/UI_SelectTip.h"

/*

1.这个游戏是哔哩哔哩赖小宇ly一人制作的单机版游戏-美食大战老鼠。

2.这个游戏是免费游戏，作为Up的一个童年游戏，还是花了一定时间来制作的，因为是一个人做没有更多时间打磨细节希望理解

3. QQ:1033082401 群1:[924537454]   群2[:978043033]
	群3:[642884960] 群4:[1082582998] 群5:[674942175]

4.最后请支持正版游戏：4399美食大战老鼠官网[https://my.4399.com/yxmsdzls/]
*/

bool UWidgetBase::Initialize()
{
	if (!Super::Initialize())
		return false;




	return true;
}

void UWidgetBase::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	// Blueprint'/Game/Resource/Texture/UI/Game/TreviFountain/Animation/BP_2DCatch_GetItemAnimation.BP_2DCatch_GetItemAnimation'
}

void UWidgetBase::PlayOperateAudio(bool _bCancel_Audio, bool _bWindowsMouseEnterAudio, bool _bWindows)
{
	if (UFVMGameInstance::GetFVMGameInstance()->GetGameConfigManager()->M_FMusic.M_bEnableSpecialAudio)
	{
		//开启了windows平台专属音效  如果当前平台不是Windows
		if (_bWindows && !UGameplayStatics::GetPlatformName().Equals(TEXT("Windows")))
			return;

		//播放鼠标移入音效
		if (_bWindowsMouseEnterAudio)
		{
			switch (UFVMGameInstance::GetFVMGameInstance()->GetGameConfigManager()->M_FMusic.M_Music_SpecialAudio)
			{
			case EConfig_Music_SpecialAudio::EEAudio_1:UFVMGameInstance::PlayBGM_S_Static(TEXT("Mouse_Enter"), TEXT("GameOperationAudio")); break;
			case EConfig_Music_SpecialAudio::EEAudio_2:UFVMGameInstance::PlayBGM_S_Static(TEXT("Mouse_Enter2"), TEXT("GameOperationAudio")); break;
			}
		}
		else
		{
			//播放取消按钮的音效
			if (_bCancel_Audio)
			{
				switch (UFVMGameInstance::GetFVMGameInstance()->GetGameConfigManager()->M_FMusic.M_Music_SpecialAudio)
				{
				case EConfig_Music_SpecialAudio::EEAudio_1:UFVMGameInstance::PlayBGM_S_Static(TEXT("Cancel"), TEXT("GameOperationAudio")); break;
				case EConfig_Music_SpecialAudio::EEAudio_2:UFVMGameInstance::PlayBGM_S_Static(TEXT("Cancel2"), TEXT("GameOperationAudio")); break;
				}
			}
			else {
				//播放确认按钮的音效
				switch (UFVMGameInstance::GetFVMGameInstance()->GetGameConfigManager()->M_FMusic.M_Music_SpecialAudio)
				{
				case EConfig_Music_SpecialAudio::EEAudio_1:UFVMGameInstance::PlayBGM_S_Static(TEXT("Click"), TEXT("GameOperationAudio")); break;
				case EConfig_Music_SpecialAudio::EEAudio_2:UFVMGameInstance::PlayBGM_S_Static(TEXT("Click2"), TEXT("GameOperationAudio")); break;
				}
			}
		}

	}
	else {
		if (!_bWindowsMouseEnterAudio)
			UFVMGameInstance::PlayBGM_S_Static(UFVMGameInstance::GetFVMGameInstance()->GetGameConfigManager()->M_FMusic.M_Operating_BGM, TEXT("GameOperationAudio"));
	}

}

void UWidgetBase::PlayOperateAudioDef()
{
	this->PlayOperateAudio();
}


void UWidgetBase::SetButtonStyle(UButton* _ButtonComponent, FString _ButtonTexturePath, bool _bRefresh)
{
	if (_ButtonTexturePath.Equals(""))
	{
		if (UFVMGameInstance::GetDebug())
			UE_LOG(LogTemp, Error, TEXT("UWidgetBase::SetButtonStyle：纹理路径无效"));
		return;
	}

	if (_ButtonComponent)
	{
		FButtonStyle _Style = _ButtonComponent->WidgetStyle;
		FSlateBrush _Normal, _Hovered, _Pressed;

		_Normal.TintColor = FSlateColor(FLinearColor(FVector4(1.f, 1.f, 1.f, 1.f)));
		_Hovered.TintColor = FSlateColor(FLinearColor(FVector4(.7f, .7f, .7f, 1.f)));
		_Pressed.TintColor = FSlateColor(FLinearColor(FVector4(.5f, .5f, .5f, 1.f)));

		_Normal.DrawAs = ESlateBrushDrawType::Image;
		_Hovered.DrawAs = ESlateBrushDrawType::Image;
		_Pressed.DrawAs = ESlateBrushDrawType::Image;

		UTexture2D* Obj = UWidgetBase::WidgetLoadTexture2D(_ButtonTexturePath);

		//如果纹理有效
		if (IsValid(Obj))
		{
			const FVector2D& LSize = FVector2D(Obj->GetSizeX(), Obj->GetSizeY());

			_Hovered.SetImageSize(LSize);
			_Pressed.SetImageSize(LSize);
			_Normal.SetImageSize(LSize);

			_Hovered.SetResourceObject(Obj);
			_Pressed.SetResourceObject(Obj);
			_Normal.SetResourceObject(Obj);
		}

		_Style.SetNormal(_Normal);
		_Style.SetHovered(_Hovered);
		_Style.SetPressed(_Pressed);

		_ButtonComponent->SetStyle(_Style);

		if (_bRefresh)
		{
			//刷新显示状态
			const ESlateVisibility& Curen = _ButtonComponent->GetVisibility();

			_ButtonComponent->SetVisibility(ESlateVisibility::Collapsed);

			_ButtonComponent->SetVisibility(Curen);
		}
		//UE_LOG(LogTemp, Error, TEXT("%.2f  %.2f"), _ButtonComponent->GetDesiredSize().X, _ButtonComponent->GetDesiredSize().Y);
	}
}

void UWidgetBase::SetBoxResetSize(USizeBox* _USizeBoxComponent, FString _ButtonTexturePath)
{
	if (_USizeBoxComponent)
	{
		UTexture2D* LocalImg = LoadObject<UTexture2D>(0, *_ButtonTexturePath);
		if (LocalImg)
		{
			_USizeBoxComponent->SetWidthOverride(LocalImg->GetSizeX());
			_USizeBoxComponent->SetHeightOverride(LocalImg->GetSizeY());
		}
	}
}

void UWidgetBase::SetImageBrush(UImage* _ImageComponent, FString _ImageTexturePath, FVector _Color, float Alph, bool _ResetSize, FVector _Scale, bool _bHit)
{
	if (_ImageComponent)
	{
		if (_ImageTexturePath.Equals(""))
		{
			_ImageComponent->SetVisibility(ESlateVisibility::Collapsed);
			return;
		}

		UTexture2D* LoadImg = UWidgetBase::WidgetLoadTexture2D(_ImageTexturePath);

		if (LoadImg)
		{
			_ImageComponent->Brush.SetResourceObject(LoadImg);
			_ImageComponent->Brush.DrawAs = ESlateBrushDrawType::Image;
			UWidgetBase::SetImageColor(_ImageComponent, _Color, Alph);

			if (_ResetSize) {
				_ImageComponent->Brush.ImageSize = FVector2D(LoadImg->GetSizeX() * _Scale.X, LoadImg->GetSizeY() * _Scale.Y);
				UWidgetBase::WidgetToCanvasSlot(_ImageComponent)->SetSize(FVector2D(LoadImg->GetSizeX() * _Scale.X, LoadImg->GetSizeY() * _Scale.Y));
			}

			if (!_bHit)
				_ImageComponent->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		}
		else
			_ImageComponent->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UWidgetBase::SetImageColor(UImage* _ImageComponent, FVector _Color, float Alph)
{
	if (!_ImageComponent)
		return;

	_ImageComponent->Brush.TintColor = FSlateColor(FLinearColor(_Color.X, _Color.Y, _Color.Z, Alph));
}

UTexture2D* UWidgetBase::WidgetLoadTexture2D(const FString& _Path)
{
	FStreamableManager& LStreamLoad = UAssetManager::GetStreamableManager();

	const FSoftObjectPath& Path = _Path;

	return Cast<UTexture2D>(LStreamLoad.LoadSynchronous(Path));
}

void UWidgetBase::CreateTipWidget(const FString& _Text, FVector _Color, float Alpha)
{
	UUI_Tip* Tip = CreateWidget<UUI_Tip>(UFVMGameInstance::GetFVMGameInstance(), LoadClass<UUI_Tip>(0, TEXT("WidgetBlueprint'/Game/Resource/BP/Game/UI/UI_Tip/BPUI_Tip.BPUI_Tip_C'")));
	Tip->SetTipText(_Text);
	Tip->SetTextColor(_Color, Alpha);
	Tip->AddToViewport();
}

UButton* const UWidgetBase::CreateSelectTipWidget(const FString& _Content)
{
	UUI_SelectTip* Tip = CreateWidget<UUI_SelectTip>(UFVMGameInstance::GetFVMGameInstance(), LoadClass<UUI_SelectTip>(0, TEXT("WidgetBlueprint'/Game/Resource/BP/Game/UI/UI_Tip/UI_SelectTip.UI_SelectTip_C'")));
	Tip->M_Content = FText::FromString(_Content);
	Tip->AddToViewport();
	return Tip->GetOkButton();
}

void UWidgetBase::SetWidgetScaleFromConfig(UWidget* _Widget_Panel)
{
	_Widget_Panel->SetRenderScale(FVector2D(UFVMGameInstance::GetFVMGameInstance()->GetGameConfigManager()->M_GameScreenScale.M_X, UFVMGameInstance::GetFVMGameInstance()->GetGameConfigManager()->M_GameScreenScale.M_Y));
}

void UWidgetBase::SetWidgetTransformProgress(UWidget* _Widget_Panel, float X_Position, float XMaxSize, float Alpha)
{
	UCanvasPanelSlot* Panel = UWidgetLayoutLibrary::SlotAsCanvasSlot(_Widget_Panel);
	if (Panel)
	{
		Panel->SetPosition(FVector2D(X_Position, Panel->GetPosition().Y));
		Panel->SetSize(FVector2D(Alpha * XMaxSize, Panel->GetSize().Y));
	}
}

void UWidgetBase::AddTextToRichText(const FString& _Content, URichTextBlock* _RichTextBlock)
{
	if (!_RichTextBlock)
		return;

	FString SContent = _RichTextBlock->GetText().ToString() + _Content + TEXT("<br><br>");

	_RichTextBlock->SetText(FText::FromString(UKismetStringLibrary::Replace(SContent, TEXT("<br>"), TEXT("\r\n"), ESearchCase::IgnoreCase)));
}

UCanvasPanelSlot* UWidgetBase::WidgetToCanvasSlot(UWidget* _Widget)
{
	return UWidgetLayoutLibrary::SlotAsCanvasSlot(_Widget);
}

void UWidgetBase::SetWidgetSacle(UWidget* _Widget, const FVector2D& _Sacle)
{
	if (_Widget)
	{
		_Widget->SetRenderScale(_Sacle);
	}
}

UTexture2D* UWidgetBase::LoadTexture2D(const FString& _Path)
{
	return UWidgetBase::WidgetLoadTexture2D(_Path);
}

