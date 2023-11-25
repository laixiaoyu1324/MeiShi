// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include <Components/Button.h>
#include <Components/SizeBox.h>

#include "WidgetBase.generated.h"

#define F __FUNCTION__

class UUI_SelectTip;
class UButton;
class USizeBox;
class UImage;
class URichTextBlock;
class UUI_Tip;
class UCanvasPanelSlot;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMyMuitDynamicDele, FString, str);
DECLARE_DYNAMIC_DELEGATE_OneParam(FMyDyanimDele, FString, str);

UCLASS()
class FVM_API UWidgetBase : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual bool Initialize() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
public:
	//播放操作音效
	UFUNCTION(BlueprintCallable)
		void PlayOperateAudio(bool _bUseCancel_Audio = false, bool _bWindowsMouseEnterAudio = false,bool _bWindows = false);
	//播放操作音效->默认
	UFUNCTION(BlueprintCallable)
		void PlayOperateAudioDef();
public:
	//绑定按钮事件
	template <class Type>
	static void ButtonDelegateBind(UButton* _ButtonComponent, Type* _DelegateClass, FName _FuncName)
	{

		if (_ButtonComponent->OnClicked.IsBound())
			_ButtonComponent->OnClicked.Clear();

		FScriptDelegate Func;
		Func.BindUFunction(_DelegateClass, _FuncName);
		_ButtonComponent->OnClicked.Add(Func);
	}
public:
	//设置按钮风格
	UFUNCTION(BlueprintCallable)
		static void SetButtonStyle(UButton* _ButtonComponent, FString _ButtonTexturePath,bool _bRefresh = false);
	//设置一个图片的原始大小赋予盒子大小
	UFUNCTION(BlueprintCallable)
		static void SetBoxResetSize(USizeBox* _USizeBoxComponent, FString _ButtonTexturePath);
	//设置图片纹理(bHit = 是否可以点击图片  默认false)
	UFUNCTION(BlueprintCallable)
		static void SetImageBrush(UImage* _ImageComponent, FString _ImageTexturePath, FVector _Color = FVector(1.f), float Alph = 1.f, bool _ResetSize = false, FVector _Scale = FVector(1.f, 1.f, 0.f), bool _bHit = false);
	//设置图片颜色
	UFUNCTION(BlueprintCallable)
		static void SetImageColor(UImage* _ImageComponent, FVector _Color = FVector(1.f), float Alph = 1.f);
	//通过Widget加载2d纹理
	UFUNCTION(BlueprintCallable)
		static UTexture2D* WidgetLoadTexture2D(const FString& _Path);
	//创建一个提示
	UFUNCTION(BlueprintCallable)
		static void CreateTipWidget(const FString& _Text, FVector _Color = FVector(0.f, 1.f, 1.f), float Alpha = 1.f);
	//创建一个选择确认提示框
	UFUNCTION(BlueprintCallable)
		static UButton* const CreateSelectTipWidget(const FString& _Content);
	//从自配置文件中设置画布比例
	UFUNCTION(BlueprintCallable)
		static void SetWidgetScaleFromConfig(UWidget* _Widget_Panel);
	//将UI转换成(CanvasSlot)并且拥有Progress行为
	UFUNCTION(BlueprintCallable)
		static void SetWidgetTransformProgress(UWidget* _Widget_Panel, float X_Position, float XMaxSize, float Alpha);
	//向富文本框添加文字
	UFUNCTION(BlueprintCallable)
		static void AddTextToRichText(const FString& _Content, URichTextBlock* _RichTextBlock);
	//将组件转换成一个CanvasSlot
	UFUNCTION(BlueprintCallable)
		static UCanvasPanelSlot* WidgetToCanvasSlot(UWidget* _Widget);
public:
	//设置画布缩放
	UFUNCTION(BlueprintCallable)
		void SetWidgetSacle(UWidget* _Widget, const FVector2D& _Sacle);
	//加载2D纹理
	UFUNCTION(BlueprintCallable)
		UTexture2D* LoadTexture2D(const FString& _Path);
public:
	template <class _Cast, class T = UWidgetBase>
	_Cast* GetWidgetComponent(T* _Widget, const FString& _ComponentName, const FString& _FuncName = "")
	{
		if (!_Widget)
		{
			UE_LOG(LogTemp, Error, TEXT("[%s]:传入的_Widget是无效的"), *_FuncName);
			return nullptr;
		}

		if (_ComponentName.Equals(TEXT("")))
		{
			UE_LOG(LogTemp, Error, TEXT("你使用【空】的名称，来获取UI组件，返回nullptr"));
			return nullptr;
		}

		//获取组件
		_Cast* WidgetComponent = Cast<_Cast>(_Widget->GetWidgetFromName(*_ComponentName));
		if (!WidgetComponent)
		{
			UE_LOG(LogTemp, Error, TEXT("[%s]:%s这个名称在传入的Widget中不存在!"), *_FuncName, *_ComponentName);
			return nullptr;
		}
		return WidgetComponent;
	}

	//UI 动画
public:

};
