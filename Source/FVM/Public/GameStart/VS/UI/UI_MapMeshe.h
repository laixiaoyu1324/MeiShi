// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetBase.h"
#include "GameStart/VS/MapBaseType.h"
#include "GameSystem/Item/ItemStruct.h"
#include "UI_MapMeshe.generated.h"

class UImage;
class ACardActor;
class AMapMeshe;
class UCardManagerComponent;
class UUI_Card;
class AMouseActor;

UCLASS()
class FVM_API UUI_MapMeshe : public UWidgetBase
{
	GENERATED_BODY()
public:
	//地图网格的位置
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MapMeshe | Tranform")
		FTransform M_MapMesheTransform;
	//格子类型
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MapMeshe | Type")
		ELineType M_ELineType;
	//卡片信息集合
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MapMeshe | Card")
		TMap<int32, ACardActor*> M_Card_Data;
public:
	//放置卡片（卡片组件，卡片数据，卡片价格，是否重置卡片冷却时间）
	void PlayCard(UCardManagerComponent* _CardMComp, UClass* _CardRes, FItemCard& _CardData, int32 _Price, bool ResetCoolDown);
public:
	//获取当前网格所有的卡片数据
	const TMap<int32, ACardActor*>& GetCardDatas();
public:
	//初始化
	virtual bool Initialize() override;
public:
	//设置颜色
	UFUNCTION(BlueprintCallable)
		void SetColor(FVector _Vec, float _Alpha);
	//设置网格地图
	UFUNCTION(BlueprintCallable)
		void SetMapMeshe(AMapMeshe* _Meshe);
	//设置线路
	UFUNCTION(BlueprintCallable)
		void SetLine(FLine CurLine);
public:
	//获取当前网格是否可用
	UFUNCTION(BlueprintCallable)
		bool GetCurMesheIsEnable();
	//获取当前网格
	UFUNCTION(BlueprintCallable)
		AMapMeshe* GetMapMeshe();
	//获取当前网格的最顶部网格位置
	UFUNCTION(BlueprintCallable)
		FVector GetMapMeshCurrentTop(int32 _Col);
	//获取当前网格的最底部网格位置
	UFUNCTION(BlueprintCallable)
		FVector GetMapMeshCurrentBottom(int32 _Col);
	//获取当前网格的最左边的网格位置
	UFUNCTION(BlueprintCallable)
		FVector GetMapMeshCurrentLeft(int32 Row);
	//获取当前网格的最右边的网格位置
	UFUNCTION(BlueprintCallable)
		FVector GetMapMeshCurrentRight(int32 Row);
	//通过ID获取卡片名称(获取失败返回(""))
	UFUNCTION(BlueprintCallable)
		FString GetCardNameByID(const int32& _Id);
	//通过ID获取卡片对应的UI(获取失败返回(""))
	UFUNCTION(BlueprintCallable)
		UUI_Card* GetCardUIByID(const int32& _Id);
	//通过ID获取目标卡片名称(获取失败返回(""))
	UFUNCTION(BlueprintCallable)
		FString GetCardActorNameByID(const int32& _Id);
	//获取线路
	UFUNCTION(BlueprintCallable)
		FLine GetLine();
public:
	//添加ID
	UFUNCTION(BlueprintCallable)
		bool IdAdd(const int32 _ID, ACardActor* _Card);
	//卡片ID测试
	UFUNCTION(BlueprintCallable)
		bool TestID(const int32 _ID);
	//销毁卡片
	UFUNCTION(BlueprintCallable)
		void EradicateCard();
	//创建卡片实例
	UFUNCTION(BlueprintCallable)
		bool CreateNewCard(UCardManagerComponent* _CardMComp);
	/*
	 // 修改新方案【将网格修改为不同的类型，分别实现函数【CreateNewCard，通过蓝图UI去调用】】
	*/
	virtual bool CreateCard(UUI_MapMeshe* _Meshe, UCardManagerComponent* _CardMComp, UClass* _CardRes, const FString& _CardActorName, int32 _Price, bool ResetCoolDown, bool _bCheck);

	//当一个Actor开始与网格发生重叠
	virtual void BoxOverlapBeginByAMapMeshe(AActor* _Actor);
	//当一个Actor与网格发生重叠结束
	virtual void BoxOverlapEndByAMapMeshe(AActor* _Actor);
	//网格更新
	virtual void TickByAMapMeshe(float DeltaTime);

public:
	//放置卡片
	bool BeginPlayCard(FItemCard& _Card, UCardManagerComponent* _CardMComp, UClass* _CardRes, int32 _Price, bool ResetCoolDown);
	//销毁卡片
	bool EradicateCard(const int32& _CardLayer);
	//根据卡片的类型，打印提示文字
	void CardTypeDebug(const FString& _CardName, ELineType _LineType);

public:
	//获取排序优先级
	UFUNCTION(BlueprintCallable)
		int32 GetTranslucency();

	//获取角色的渲染层
	UFUNCTION(BlueprintCallable)
		int32 GetCharTranslucency();

	//设置排序优先级
	void SetTranslucency(const int32& _Translucency);

	//排序优先级更新
	UFUNCTION(BlueprintImplementableEvent)
		void TranslucencyChange(int32 _CurTrans);
private:
	//当老鼠进入时
	void OnMouseEnter(AMouseActor* Mouse);
	//当老鼠退出时
	void OnMouseEnd(AMouseActor* Mouse);
private:
	UPROPERTY()
		UImage* M_Gird = nullptr;
	UPROPERTY()
		AMapMeshe* M_MapMeshe = nullptr;
private:
	//Tick更新时间
	UPROPERTY()
		float M_UpdateTime = 0.f;
	//渲染优先级
	UPROPERTY()
		int32 M_Translucency = 0;
	//UIMapMeshe线路
	UPROPERTY()
		FLine M_Line;
	//是否有角色
	UPROPERTY()
		bool bHasChar = false;
private:
	//播放音效
	void PlayCard_PlayBGM();
	//放置角色
	void PlayPlayer();

};