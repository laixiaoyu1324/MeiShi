// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameStart/VS/MapBaseType.h"
#include "GameStart/VS/Components/MouseManagerComponent.h"
#include "GameStart/VS/Components/Card/CardDataComponent.h"
#include "GameSystem/Item/Card/CardAtkStruct.h"
#include "GameSystem/Item/ItemStruct.h"
#include "GameSystem/Tools/GameSystemFunction.h"
#include "GameStart/Flipbook/GameActorFlipbookBase.h"
#include "CardActor.generated.h"


class UStaticMeshComponent;
class UCardDataComponent;
class UMouseManagerComponent;
class UPaperFlipbook;
class AGameMapInstance;

class AMapMeshe;
class AMapMouseMesheManager;
class UUI_MapMeshe;

class AMouseActor;
class UUI_Card;

class UBoxComponent;

class UPaperFlipbookComponent;




//生命数据
USTRUCT(BlueprintType)
struct FCard_HP {
	GENERATED_USTRUCT_BODY()
public:
	//当前生命值
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float M_Life = 10.f;
	//总生命值
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float M_TotalLife = 10.f;
};

//卡片属性
USTRUCT(BlueprintType)
struct FCard_Property {
	GENERATED_USTRUCT_BODY()
public:
	//白天或者是夜晚(地图)
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool M_Day = true;
	//白天或者是夜晚(卡片)
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool M_CardDay = true;
	//卡片名称
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString M_CardName;
	//卡片优先级(一个格子只能存在不同优先级的卡片(只能存在一次))
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 M_CardID = 0;
	//当前卡片所在的路线
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FLine M_Line;
	//适应类型
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		ELineType M_ELineType;
	//当前需要攻击的老鼠对象
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		AMouseActor* M_CurrentMouse = nullptr;
	//当前攻击该卡片的老鼠
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		AMouseActor* M_CurrentAttackSelfMouse = nullptr;
};

//当卡片受到老鼠的攻击
DECLARE_DELEGATE_OneParam(FCardBeHurtDelegate, AMouseActor* CurMouseActor)

UCLASS()
class FVM_API ACardActor : public AGameActorFlipbookBase
{
	GENERATED_BODY()
public:
	//卡片代理函数(当卡片被激活时)
	FFunctionDelegates M_CardActiveDelegate;
	//卡片代理函数(当卡片受伤时)
	FCardBeHurtDelegate M_CardBeHurtDelegate;
	//卡片代理函数(当卡片死亡时)
	FFunctionDelegates M_CardDeathDelegate;
public:
	//资产集合，老框架
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ResourceRefPath")
		TArray<TSoftObjectPtr<UPaperFlipbook>> M_Resource;
	//资产子弹类型集合，老框架
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ResourceRefPath")
		TArray<TSoftClassPtr<AFlyItemActor>> M_BulletResource;
public:
	//获取当前卡片对应的UI
	UUI_Card* const GetCardUI();
public:
	//设置当前卡片对应的UI
	void SetCardUI(UUI_Card* _card_ui);
	//设置浮动模式
	void SetFloatModeEnable(bool _Value = true);
public:
	//获取浮动模式
	UFUNCTION(BlueprintCallable)
		FORCEINLINE bool GetFloatMode() const;
	//获取卡片原数据
	UFUNCTION(BlueprintCallable)
		const FItemCard& GetCardData();
	//获取卡片名称
	UFUNCTION(BlueprintCallable)
		FString GetCardName();
	//获取当前生命值
	UFUNCTION(BlueprintCallable)
		float GetCurrentHP();
	//获取总生命值
	UFUNCTION(BlueprintCallable)
		float GetTotalHP();
	//获取地图是白天还是夜晚
	UFUNCTION(BlueprintCallable)
		bool GetMapDay();
	//获取卡片是白天还是夜晚
	UFUNCTION(BlueprintCallable)
		bool GetCardDay();
	//获取地图状态是否游戏暂停
	UFUNCTION(BlueprintCallable)
		bool GamePause();
	//获取卡片等级
	UFUNCTION(BlueprintCallable)
		int32 GetCardGrade(const FString& _Name);
	//获取卡片ID
	UFUNCTION(BlueprintCallable)
		int32 GetCardID();
	//获取线路
	UFUNCTION(BlueprintCallable)
		FLine GetLine();
	//获取线路类型
	UFUNCTION(BlueprintCallable)
		const ELineType& GetLineType() const;
	//获取地图网格UI
	UFUNCTION(BlueprintCallable)
		UUI_MapMeshe* GetUIMapMesh();
	//获取当前指向的老鼠
	UFUNCTION(BlueprintCallable)
		AMouseActor* GetCurrentMouse();
	//获取当前攻击该卡片的老鼠
	UFUNCTION(BlueprintCallable)
		AMouseActor* GetCurrentAttackSelfMouse();
public:
	//设置卡片等级
	UFUNCTION(BlueprintCallable)
		void SetCardGrade(const int32& _CardGrade);
	//设置线路
	UFUNCTION(BlueprintCallable)
		void SetLine(const FLine& _Line);
	//设置类型和层
	UFUNCTION(BlueprintCallable)
		void SetCardType(const ELineType& _Type, const int32& _CardID);
	//设置卡片的生命值
	UFUNCTION(BlueprintCallable)
		void SetCardHP(const float& _HP, float _HPUpRate);
	//设置卡片当前生命值
	UFUNCTION(BlueprintCallable)
		void SetCardCurrentHP(const float& _HP);
	//设置地图网格UI
	UFUNCTION(BlueprintCallable)
		void SetUIMapMesh(UUI_MapMeshe* _MapMesh);
	//设置当前老鼠指向
	UFUNCTION(BlueprintCallable)
		void SetCurrentMouse(AMouseActor* _MouseActor);
	//设置当前攻击该卡片的老鼠
	UFUNCTION(BlueprintCallable)
		void SetCurrentAttackSelfMouse(AMouseActor* _MouseActor);
	//设置层级
	virtual	void SetTranslucentSortPriority(int32 _Layer) override;
public:

	//解析actor
	UFUNCTION(BlueprintCallable)
		void AnalysisActor(AActor* _Actor);
	//转换卡片适应的白天或者夜晚
	UFUNCTION(BlueprintCallable)
		void CardDayTransform(bool _Day);
	//移除当前卡片
	UFUNCTION(BlueprintCallable)
		void KillCard();
	//打印老鼠线路通知
	UFUNCTION(BlueprintCallable)
		void PrintMouseLineNotify();
	//更新卡片状态（受到的攻击力和受到的回复）
	UFUNCTION(BlueprintCallable)
		bool UpdateCardState(const float& _ATK_Value, const float& _UpHP_Value = 0.f);
	//更新卡片激活状态
	UFUNCTION(BlueprintCallable)
		void UpdateCardEnableState();
public:
	//函数调用发射器(当函数被调用时，会触发指针函数调用)

	//卡片被激活时
	UFUNCTION()
		void CardActive();
	//卡片受到伤害时
	UFUNCTION()
		void CardBeHurt(class AMouseActor* CurMouseActor);
	//卡片死亡伤害时
	UFUNCTION()
		void CardDeath();
public:
	//当卡片移动时
	UFUNCTION()
		void OnCardMovedUpdate(const float& DeltaTime, const int32& _Layer, const FVector& _Location, const FLine& _Line);
	//当卡片开始移动
	UFUNCTION()
		void OnCardMoveBegin(const int32& _Layer, const FLine& _Line);
	//当卡片移动结束时
	UFUNCTION()
		void OnCardMoveEnd(const int32& _Layer, const FLine& _Line);
public:
	ACardActor();

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;
private:
	//颜色设置控制变量
	UPROPERTY()
		bool M_bColorSet = false;
	//当前卡片所对应的UI
	UPROPERTY()
		UUI_Card* M_UUI_Card = nullptr;
	//卡片数据
	UPROPERTY()
		FItemCard M_SourceCardDataBase;
	//浮动模式(跟随地板浮动)
	UPROPERTY()
		bool M_bFloatMode = false;
public:
	//卡片等级动画
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Mesh")
		UStaticMeshComponent* M_CardGradeStaticMesh = nullptr;
	//游戏地图实例
	UPROPERTY(EditAnywhere)
		AGameMapInstance* M_AGameMapInstance = nullptr;
	//卡片碰撞
	UPROPERTY(EditAnywhere)
		UBoxComponent* M_CardTypeBoxCollision = nullptr;
private:
	//生命类
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		FCard_HP M_FCardActor_HP;
	//卡片属性
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		FCard_Property M_CardProperty;
	//当前卡片所在的UI网格
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		UUI_MapMeshe* M_UUI_MapMeshe = nullptr;

	/*

			新结构新版本

	*/
public:
	//获取卡片数据组件
	UFUNCTION()
		UCardDataComponent* GetCardDataComponent();
	//获取盒体碰撞组件
	UFUNCTION()
		UBoxComponent* GetBoxComponent();
public:
	void InitCheckMeshe();

	void CheckMeshe(const FLineCheckSetting& CurCheck, const int32& CheckLength, UMesheControllComponent* CurMesheCtr);

	bool CheckMouse(const float& DeltaTime);
public:
	//当检测到老鼠
	virtual void OnCheckMouse(AMouseActor* Mouse);
	//检测到陆地老鼠
	virtual void OnCheckMouseGround(AMouseActor* Mouse);
	//检测到空中老鼠
	virtual void OnCheckMouseSky(AMouseActor* Mouse);
	//检测到老鼠地下
	virtual void OnCheckMouseUnderground(AMouseActor* Mouse);
public:
	//卡片的基础属性蓝图填写

	//卡片名称
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "卡片预设属性")
		FString CardActor_Name = TEXT("");
protected:
	//设置卡片数据
	void SetCardActor(const FItemCard& _CardData);
private:
	//卡片数据组件
	UPROPERTY()
		UCardDataComponent* CurCardDataComponent = nullptr;
private:
	//当前线路
	UPROPERTY(EditAnywhere)
		FLine CurCheckMesheLine;
	//当前所得到所有检测网格
	UPROPERTY(EditAnywhere)
		TArray<AMapMouseMesheManager*> CurMapMouseMesheManagers;
	//检测陆地
	UPROPERTY()
		bool bCheckGround = false;
	//检测空中
	UPROPERTY()
		bool bCheckSky = false;
	//检测地下
	UPROPERTY()
		bool bCheckUnderGround = false;
};