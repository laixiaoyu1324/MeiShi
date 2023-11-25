// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameSystem/MouseStructManager.h"
#include "Data/CardData/MouseDataStruct.h"
#include "GameStart/VS/MapBaseType.h"
#include "MouseManagerComponent.generated.h"

class UFVMGameInstance;
class UGameMapStructManager;
class UMouseStructManager;
class UFVMGameInstance;
class AMouseActor;
class UMesheControllComponent;

class UUI_MouseRound;
class UUI_MouseBoss_Slider;

//老鼠线路管理器
UCLASS(BlueprintType)
class FVM_API UMouseLineManager : public UObject
{
	GENERATED_BODY()
public:
	//添加老鼠
	void AddMouse(AMouseActor* _CurMouse);
	//移除老鼠
	void RemoveMouse(AMouseActor* _CurMouse);
	//排序老鼠位置
	void SortMouseByTick(const float& _Tick);
	//获取陆地最前老鼠
	AMouseActor* GetMouseTopByGorund();
	//获取地下最前老鼠
	AMouseActor* GetMouseTopByUnderGorund();
	//获取空中最前老鼠
	AMouseActor* GetMouseTopBySky();
	//获取所有类型最前的老鼠
	AMouseActor* GetMouseTopByAllType();
	//当前行是否存在老鼠
	bool GetMouseExist();
	//消灭所有老鼠
	void KillAllMouse();
private:
	//排序老鼠最前位置
	AMouseActor* SortMouseTopLocation(TArray<AMouseActor*>& _Mouses);
private:
	//更新Tick的时间延迟
	UPROPERTY()
		float UpdateTickTime = 0.2f;
	UPROPERTY()
		float CurTime = 0.f;

	//陆地最前老鼠
	UPROPERTY(EditAnywhere)
		AMouseActor* CurMouseGroundTop = nullptr;
	//地下最前老鼠
	UPROPERTY(EditAnywhere)
		AMouseActor* CurMouseUnderGroundTop = nullptr;
	//空中最前老鼠
	UPROPERTY(EditAnywhere)
		AMouseActor* CurMouseSkyTop = nullptr;

	//陆地老鼠
	UPROPERTY(EditAnywhere, Meta = (AllowPrivateAccess = "true"))
		TArray<AMouseActor*> CurMouseGround;
	//地下老鼠
	UPROPERTY(EditAnywhere, Meta = (AllowPrivateAccess = "true"))
		TArray<AMouseActor*> CurMouseUnderGround;
	//飞行老鼠
	UPROPERTY(EditAnywhere, Meta = (AllowPrivateAccess = "true"))
		TArray<AMouseActor*> CurMouseSky;
};

//当回合数发生改变时触发
DECLARE_DELEGATE(FOnRoundNodeChanged);
//当进入当前回合最末尾时触发
DECLARE_DELEGATE(FOnInCurRoundEnd);
//当进入下一个回合时触发
DECLARE_DELEGATE(FOnInNextRound);

//老鼠时间管理器
UCLASS()
class FVM_API UMouseTimeManager : public UObject
{
	GENERATED_BODY()
public:
	//当回合数发生改变时触发
	FOnRoundNodeChanged OnRoundNodeChanged;
	//当进入当前回合最末尾时触发
	FOnInCurRoundEnd OnInCurRoundEnd;
	//当进入下一个回合时触发
	FOnInNextRound OnInNextRound;
public:
	//重置
	void Reset();
	//更新时间
	void UpdateTime(const float& _tick, UMouseManagerComponent* const MouseManagerComponent);
	//进入下一个回合
	void InNextRound(const float& _DelayTime);
	//进入下一个回合
	void InNextRound();
	//是否是当前回合的最末尾
	bool GetCurRoundEnd();
	//老鼠是否全部生成完毕
	bool GetAllMouseSpawnEnd();
	//启动计时器
	void Enable();
	//获取最小子节点索引
	int32 GetTimeNode();
	//获取最小大节点索引
	int32 GetTimeNodeRound();
	//获取回合数索引
	int32 GetTimeRound();
	//设置回合索引
	void SetTimeRound(const int32& Index);
	//设置所有回合结束
	void SetRoundEnd();
	//获取时间
	int32 GetTime();
private:
	//启动时间计时
	UPROPERTY()
		bool bEnable = false;

	//启动进入下一个回合的计时
	UPROPERTY()
		bool bEnableNextRound = false;
	//当前进入下一个回合的时间
	UPROPERTY()
		float CurNextRoundTime = 0.f;
	//进入下一个回合的总时间
	UPROPERTY()
		float CurNextRoundTotalTime = 0.f;
	//是否继续更新当前回合的节点
	UPROPERTY()
		bool bUpdateCurNode = true;

	//进入下一个回合时检测是否还有老鼠存在
	UPROPERTY()
		bool bEnableCheckMouseExist = false;
	//开始下一个回合是否是最后一个会的检测
	UPROPERTY()
		bool bEnableCheckNextRoundIsEnd = false;
	//当前回合是否是最后一个回合
	UPROPERTY()
		bool bCurRoundIsEnd = false;

	//是否将所有老鼠全部生成完毕
	UPROPERTY()
		bool bCurAllMouseSpawnEnd = false;

	//分钟
	UPROPERTY()
		int32 Minute = 0;
	//秒钟
	UPROPERTY()
		int32 Second = 0;
	//当前时间流动
	UPROPERTY()
		float CurTime = 0.f;
	//总时间流动
	UPROPERTY()
		float AllTime = 0.f;
private:
	//最小节点索引
	UPROPERTY()
		int32 TimeNodeIndex = 0;
	//最小大节点索引，它决定了使用那个最小节点组
	UPROPERTY()
		int32 TimeNodeRoundIndex = 0;
	//大节点索引,它决定了当前是第多少波
	UPROPERTY()
		int32 RoundIndex = 0;
};


//老鼠生成器
UCLASS()
class UMouseSpawnManager : public UObject
{
	GENERATED_BODY()
public:

	//生成老鼠
	UFUNCTION()
		AMouseActor* SpawnMouse(const FString& _MousePath, const FLine& Line, const int32& RowMax, bool DeathFroceGameWin,const TArray<FString>& Items,const FMouseBase& MouseData);

};

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class FVM_API UMouseManagerComponent : public UActorComponent
{
	GENERATED_BODY()
public:
	static UMouseManagerComponent* M_S_CurrentClass;
public:
	//老鼠管理器
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UMouseStructManager* M_UMouseStructManager = nullptr;
	//老鼠回合和Boss的血量UI
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UUI_MouseRound* M_UUI_MouseRound = nullptr;
public:
	// Sets default values for this component's properties
	UMouseManagerComponent();
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	// Called when the game starts
	virtual void BeginPlay() override;
public:
	//获取时间管理器
	const UMouseTimeManager* const GetMouseTimeManager();
	//通过老鼠名称获取一个资产引用路径
	bool GetMousePathByName(const FString& MouseName, FString& OutMousePath,FMouseBase& OutData);
public:
	//游戏开始
	UFUNCTION(BlueprintCallable)
		void GameStart();
	//游戏结束
	UFUNCTION(BlueprintCallable)
		void GameOver();
	//当回合数更新时
	UFUNCTION()
		void OnRoundNodeChangedCallback();
	//当前回合末尾
	UFUNCTION()
		void OnCurRoundEndCallback();
	//下一个回合开始初始化【提示：最后一波 UI】
	UFUNCTION()
		void InitNextRoundCallBack();
public:
	//显示UI【波数UI】
	UFUNCTION(BlueprintCallable)
		void ShowMouseUI();
	//移除UI【波数UI】
	UFUNCTION(BlueprintCallable)
		void RemoveMouseUI();
	//更新进度值
	UFUNCTION(BlueprintCallable)
		void UpdateRoundProgress();
	//强行进入下一个回合
	UFUNCTION(BlueprintCallable)
		void ForceNextRound();
	//强行进入需要带钥匙开启的回合或者直接到最后一个回合的末尾
	UFUNCTION(BlueprintCallable)
		void ForceNextEndRound();
	//是否是最后一个回合
	UFUNCTION(BlueprintCallable)
		bool IsEndRound();
	//老鼠是否生成完毕
	UFUNCTION(BlueprintCallable)
		bool IsMouseSpawnEnd();
	//当前回合是否存在老鼠
	UFUNCTION(BlueprintCallable)
		bool IsMouseExist();
	//获取线路个数
	UFUNCTION(BlueprintCallable)
		const int32 GetLineCount() const;
	//获取时间
	UFUNCTION(BlueprintCallable)
		int32 GetCurTime();
	//获取当前回合
	UFUNCTION(BlueprintCallable)
		int32 GetCurRound();
	//获取线路最前的老鼠
	UFUNCTION(BlueprintCallable)
		AMouseActor* const GetLineTopFirstMouse(const int32 _LineIndex);
	//获取TopFirst老鼠
	UFUNCTION(BlueprintCallable)
		AMouseActor* const GetTopFirstMouse();

	//将当前老鼠从线路管理器中移除，被移除的老鼠不再被管理器管理
	UFUNCTION(BlueprintCallable)
		static void RemoveMouse(AMouseActor* _CurMouse);
	//添加老鼠
	UFUNCTION(BlueprintCallable)
		static void AddMouse(AMouseActor* _CurMouse,const int32& Row);
protected:
	//初始化老鼠配置
	void InitMouseStructManager();
	//初始化老鼠线路管理器
	void InitMouseLineManager();
	//初始化时间配置
	void InitTimer();
	//初始化老鼠资源配置
	void InitMouseResource();
	//初始化老鼠生成器
	void InitMouseSpawnManager();
private:
	//开启时间计时器的计数
	float CurGameReadyTime = 0;
	//当前游戏是否结束
	bool CurGameIsOver = false;
	//当前游戏是否胜利
	bool CurGameIsWin = true;
private:
	//老鼠线路管理器
	UPROPERTY(EditAnywhere, Meta = (AllowPrivateAccess = "true"))
		TArray<UMouseLineManager*> MouseLineManager;
	//时间计时器
	UPROPERTY()
		UMouseTimeManager* MouseTimeManager = nullptr;
	//老鼠生成器
	UPROPERTY()
		UMouseSpawnManager* MouseSpawnManager = nullptr;
	//所有的老鼠数据集合
	UPROPERTY()
		TArray<FMouse_Data> AllMouseData;
};
