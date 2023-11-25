// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除

#pragma once

#include "CoreMinimal.h"
#include "GameStart/Flipbook/GameActor/Mouse/Boss/BossBase.h"

#include "Components/TimelineComponent.h"
#include "Curves/CurveFloat.h"

#include "Boss_DongJun.generated.h"


class UBoxComponent;


//洞军的状态
UENUM(BlueprintType)
enum class EDongJunState : uint8 {
	Out,
	Def,
	Build,
	In,
	Hid,
	Jump,
	JumpPause
};

//洞君
UCLASS()
class FVM_API ABoss_DongJun : public ABossBase
{
	GENERATED_BODY()
public:
	//默认状态动画
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Def")
		FSoftObjectPath M_MouseDef;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Def")
		FSoftObjectPath M_MouseLowDef;

	//从地上钻出来状态动画
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Def")
		FSoftObjectPath M_MouseOut;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Def")
		FSoftObjectPath M_MouseLowOut;

	//修建管道状态动画
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Def")
		FSoftObjectPath M_MouseBuild;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Def")
		FSoftObjectPath M_MouseLowBuild;

	//从地上钻下去状态动画
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Def")
		FSoftObjectPath M_MouseIn;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Def")
		FSoftObjectPath M_MouseLowIn;

	//从地上钻下去状态动画
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Def")
		FSoftObjectPath M_MouseJump;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Def")
		FSoftObjectPath M_MouseLowJump;

	//跳跃的最大次数
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 M_JumpCountMax = 3;
	//老鼠的跳跃曲线
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UCurveFloat* M_Curve = nullptr;
	//网格的碰撞盒子
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UBoxComponent* M_BoxMesh = nullptr;
	//身体的碰撞盒子
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UBoxComponent* M_BoxBody = nullptr;

public:

	ABoss_DongJun();

	//Tick 更新
	virtual void Tick(float DeltaTime) override;

	void AnimEnd();

	UFUNCTION()
		void OverlapBegin(UPrimitiveComponent* OverlappedComponent,
			AActor* OtherActor,
			UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex,
			bool bFromSweep,
			const FHitResult& SweepResult);

	UFUNCTION()
		void OverlapEnd(UPrimitiveComponent* OverlappedComponent,
			AActor* OtherActor,
			UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex);

public:
	//默认状态
	void DefState(float DeltaTime);
	void DefStateByAnimEnd();
	//从地上钻出来
	void OutState(float DeltaTime);
	void OutStateByAnimEnd();
	//建造老鼠洞
	void BuildState(float DeltaTime);
	void BuildStateByAnimEnd();
	//进入地洞
	void InState(float DeltaTime);
	void InStateByAnimEnd();
	//消失
	void HiddenState(float DeltaTime);
	void HiddenStateByAnimEnd();
	//跳跃踩卡
	void JumpState(float DeltaTime);
	void JumpStateByAnimEnd();
	//跳跃完成的状态
	void JumpPauseState(float DeltaTime);
	void JumpPauseStateByAnimEnd();
public:
	//创建管道
	void CreateChannel();
	//当前线路管道判断(当前线路是否可以创建管道)
	bool bChannelCreate();
	//设置当前管道实例
	void SetCurrentChannel(class ADJ_Channel* _Channel);
	//获取当前链接的列
	TArray<AMapMeshe*>& GetCurrentLinkCol();
	//获取当前链接的管道
	TArray<class ADJ_Channel*>& GetCurrentLinkChannels();

	//设置洞君的状态
	void SetMouseCurrentState(const EDongJunState& State);
	//获取洞君当前的状态
	const EDongJunState& GetMouseCurrentState() const;
public:
	//时间更新
	UFUNCTION()
		void TimelineMoveUpdate();
	//时间完成
	UFUNCTION()
		void TimelineFinish();
public:
	//初始化
	virtual void BeginPlay() override;
	//老鼠初始化(Base BeginPlay自动调用一次)
	virtual void MouseInit() override;
public:
	//移动(每0.02ms自动调用(允许移动的情况下))
	virtual void MoveingUpdate(float DeltaTime) override;
	//停止移动(当设置停止移动时调用一次)
	virtual void MoveingStop() override;
	//开始移动(当设置开始移动时调用一次)
	virtual void MoveingBegin() override;

	//老鼠开始攻击(如果设置了IsAttack为true 会自动调用一次)
	virtual void AttackedBegin() override;
	//老鼠攻击结束(如果设置了IsAttack为false 会自动调用一次)
	virtual void AttackedEnd() override;
	//老鼠在攻击(如果设置了IsAttack为true 会自动调用->【每当攻击动画结束时调用】)
	virtual void AttakedUpdate(float DeltaTime) override;

	//老鼠受到攻击((当设置isHurt为true时)调用一次)
	virtual void BeAttakedBegin() override;

	//当老鼠死亡时(当设置老鼠SetMouseDeath(true)时调用一次)
	virtual void MouseDeathed() override;
public:
	//触发减速buff
	virtual void SlowDownBuff(FMouseProperty_RecvImformation& _buff) override;
	//减速buff结束
	virtual void SlowDownBuffOver(FMouseProperty_RecvImformation& _buff) override;
	//触发加速buff
	virtual void AccelerateBuff(FMouseProperty_RecvImformation& _buff) override;
	//加速buff结束
	virtual void AccelerateBuffOver(FMouseProperty_RecvImformation& _buff) override;
	//触发凝固buff
	virtual void SolidificationBuff(FMouseProperty_RecvImformation& _buff) override;
	//凝固buff结束
	virtual void SolidificationBuffOver(FMouseProperty_RecvImformation& _buff) override;
	//触发冻结buff
	virtual void FreezeBuff(FMouseProperty_RecvImformation& _buff) override;
	//冻结buff结束
	virtual void FreezeBuffOver(FMouseProperty_RecvImformation& _buff) override;
	//触发灼烧buff
	virtual void BurnBuff(FMouseProperty_RecvImformation& _buff) override;
	//灼烧buff结束
	virtual void BurnBuffOver(FMouseProperty_RecvImformation& _buff) override;


private:
	//float曲线加载代理
	FOnTimelineFloatStatic M_FloatStatic_Delegate;
	//时间轴播放完成
	FOnTimelineEventStatic M_TimelineFinish_Delegate;
private:
	//boss老鼠跳跃的时间线
	UPROPERTY()
		FTimeline M_JumpTimeLine;

	//当前状态
	UPROPERTY(EditAnywhere)
		EDongJunState M_CurrentState = EDongJunState::Out;

	//时间
	UPROPERTY()
		float M_Time = 0.f;

	//跳跃次数
	UPROPERTY()
		int32 M_JumpCount = 0;

	//当前跳跃次数
	UPROPERTY()
		int32 M_CurrentJumpCount = 0;

	//位置
	UPROPERTY()
		FVector M_CurrentLocation;

	UPROPERTY()
		FVector M_TargetLocation;

	//tick更新速度
	UPROPERTY()
		float M_TickRateSpeed = 1.f;

	//boss是否死亡
	UPROPERTY()
		bool M_bDeath = false;

	//boss是否打洞了
	UPROPERTY()
		bool M_bBuild = false;
private:
	//当前被链接的列
	UPROPERTY()
		TArray<AMapMeshe*> M_CurrentLinkCol;
	//当前被链接的管道
	UPROPERTY()
		TArray<class ADJ_Channel*> M_CurrentLinkChannels;
	//当前管道实例
	UPROPERTY()
		class ADJ_Channel* M_CurrentChannel = nullptr;
private:
	UPROPERTY()
		UPaperFlipbook* M_MouseDef_res = nullptr;
	UPROPERTY()
		UPaperFlipbook* M_MouseLowDef_res = nullptr;

	UPROPERTY()
		UPaperFlipbook* M_MouseOut_res = nullptr;
	UPROPERTY()
		UPaperFlipbook* M_MouseLowOut_res = nullptr;

	UPROPERTY()
		UPaperFlipbook* M_MouseBuild_res = nullptr;
	UPROPERTY()
		UPaperFlipbook* M_MouseLowBuild_res = nullptr;

	UPROPERTY()
		UPaperFlipbook* M_MouseIn_res = nullptr;
	UPROPERTY()
		UPaperFlipbook* M_MouseLowIn_res = nullptr;

	UPROPERTY()
		UPaperFlipbook* M_MouseJump_res = nullptr;
	UPROPERTY()
		UPaperFlipbook* M_MouseLowJump_res = nullptr;
};
