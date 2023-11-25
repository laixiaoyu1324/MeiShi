// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameStart/Flipbook/GameActorFlipbookBase.h"
//地图类型
#include "GameStart/VS/MapBaseType.h"
#include "GameStart/VS/Components/MouseManagerComponent.h"
#include <Components/BoxComponent.h>

#include "MouseActor.generated.h"

class UBoxComponent;
class ACardActor;
class AFlyItemActor;
class UMouseManagerComponent;
class UBoxComponent;

USTRUCT(BlueprintType)
struct FMouseProperty_HP {
	GENERATED_USTRUCT_BODY()
public:
	//总生命值
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float M_HP = 10.f;
	//老鼠当前生命值
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float M_CurrentHP = 10.f;
};

USTRUCT(BlueprintType)
struct FMouseProperty_ATK {
	GENERATED_USTRUCT_BODY()
public:
	//老鼠的总攻击力
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float M_ATK = 0.f;
	//老鼠的当前攻击力
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float M_CurrentATK = 0.f;
	//是否在攻击
	UPROPERTY()
		bool IsAttack = false;
	//是否受到伤害
	UPROPERTY()
		bool IsHurt = false;
	//受到的伤害值
	UPROPERTY()
		float HurtValue = 0.f;
};

USTRUCT(BlueprintType)
struct FMouseProperty_State {
	GENERATED_USTRUCT_BODY()
public:
	//移动速度
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float M_MoveSpeed = 15.f;
	//当前移动速度
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float M_CurrentMoveSpeed = 15.f;
	//当前老鼠死亡后的经验值
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 M_CurrentMouseEx = 1;
	//是否可以移动
	UPROPERTY()
		bool M_IsMove = true;
	//是否死亡
	UPROPERTY()
		bool IsDeath = false;
	//是否死亡
	UPROPERTY()
		bool MouseDeath = false;
	//是否播放死亡动画
	UPROPERTY()
		bool bPlayDeathAnim = false;
	//是否不可以使用Tick
	UPROPERTY()
		bool IsNotTick = false;
};

//一些条件
USTRUCT(BlueprintType)
struct FMouseProperty_Condition {
	GENERATED_USTRUCT_BODY()
public:
	//老鼠的类型
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		ELineType M_EMouseType;
	//当前所在的行
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FLine M_Line;
	//所在的回合
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 M_AttachRound = 0;
	//播放速度
	UPROPERTY()
		float M_PlayRate = 1.f;
	//当前播放速度
	UPROPERTY()
		float M_CurrentPlayRate = 1.f;
	//是否处于Buff
	UPROPERTY()
		bool M_bBuffState = false;
	//（凝固和冻结最后返回false 其他返回true->false表示在tick中不能进行其他行为）
	UPROPERTY()
		bool M_bBuffResult = true;
	UPROPERTY()
		float M_FixedUpdateTime = 0.f;
	//播放攻击动画
	UPROPERTY()
		bool PlayAnim_Attack = false;
	//被攻击时显示的颜色(时间计时)
	UPROPERTY()
		float M_bHurtColorTime = 0.f;
	//是否可以更换颜色
	UPROPERTY()
		bool M_bColor = false;
};

//代理结构
template<class Type>
struct FuncDelegate {
public:
	FuncDelegate() {}
	FuncDelegate(void(Type::* _Func) ()) :Func(_Func) {}
	inline void SetDelegate(void(Type::* _Func) ()) { this->Func = _Func; }
public:
	void(Type::* Func) () = nullptr;
};

class AMouseActor;

//老鼠接收的信息
USTRUCT(BlueprintType)
struct FMouseProperty_Buff {
	GENERATED_USTRUCT_BODY()
public:
	//条件
	UPROPERTY()
		bool M_bCondition = false;
	//是否已经触发buff
	UPROPERTY()
		bool M_bBegin = false;
	//当前时间(Tick)
	UPROPERTY()
		float M_CurrentTime = 0.f;
public:
	//代理函数
	FuncDelegate<AMouseActor> M_FuncBegin;
	FuncDelegate<AMouseActor> M_FuncEnd;
private:
	//被代理类
	AMouseActor* M_CurrentMouseActor = nullptr;
public:
	//设置代理类
	void SetClass(AMouseActor* _MouseActor) { this->M_CurrentMouseActor = _MouseActor; }
	AMouseActor* GetClass() { return this->M_CurrentMouseActor; }
	//设置代理函数
	void SetBeginFunc(void (AMouseActor::* FuncBegin)()) { this->M_FuncBegin.Func = FuncBegin; }
	void SetEndFunc(void (AMouseActor::* FuncEnd)()) { this->M_FuncEnd.Func = FuncEnd; }
public:
	void Clear() {
		//如果当前buff已经不存在了
		if (!this->M_bCondition)
			return;

		this->M_CurrentTime = 0.f;
		this->M_bBegin = false;
		this->M_bCondition = false;

		//调用结束函数
		if (this->M_CurrentMouseActor)
			(this->M_CurrentMouseActor->*M_FuncEnd.Func)();
	}
};

//声明buff代理函数
void SetBuffDelegate(AMouseActor* _Actor, FMouseProperty_Buff& _buff, void (AMouseActor::* FuncBegin)(), void (AMouseActor::* FuncEnd)());

//老鼠接收的信息
USTRUCT(BlueprintType)
struct FMouseProperty_RecvImformation {
	GENERATED_USTRUCT_BODY()
public:
	//减速
	UPROPERTY()
		FMouseProperty_Buff M_SlowDown;
	//加速
	UPROPERTY()
		FMouseProperty_Buff M_Accelerate;;
	//冻结
	UPROPERTY()
		FMouseProperty_Buff M_Freeze;
	//凝固
	UPROPERTY()
		FMouseProperty_Buff M_Solidification;
	//灼烧
	UPROPERTY()
		FMouseProperty_Buff M_Burn;
};


//老鼠基本类型
UENUM()
enum class EMouseBaseType : uint8
{
	Normal, //通常
	Boss, // Boss
	Other //其他
};

UCLASS()
class FVM_API AMouseActor : public AGameActorFlipbookBase
{
	GENERATED_BODY()
public:
	//血量
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FMouseProperty_HP M_Proper_HP;
	//攻击
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FMouseProperty_ATK M_Proper_ATK;
	//状态
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FMouseProperty_State M_Proper_State;
	//条件
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FMouseProperty_Condition M_Proper_Condition;
	//接收的信息
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FMouseProperty_RecvImformation M_RecvInformation;
public:
	//老鼠的位置
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		USceneComponent* M_MousePosition = nullptr;
protected:
	//添加Buff
	void AddBuffFunc(const float _DeltaTime, FMouseProperty_Buff& _buff, int32& _BuffCountResult);
public:
	//更新老鼠的图片显示颜色
	void UpdateMouseColor();
public:
	//老鼠加速buff处理函数
	void AccelerateFunc();
	//老鼠加速buff处理函数结束
	void AccelerateFuncEnd();
	//老鼠减速Buff处理
	void SlowDownFunc();
	//老鼠减速Buff处理结束
	void SlowDownFuncEnd();
	//老鼠凝固
	void SolidificationFunc();
	//老鼠凝固函数处理结束
	void SolidificationFuncEnd();
	//老鼠冻结
	void FreezeFunc();
	//老鼠冻结函数结束
	void FreezeFuncEnd();
	//老鼠灼烧
	void BurnFunc();
	//老鼠灼烧函数结束
	void BurnFuncEnd();
	//解析Buff信息
	void ParseBuff_Information(FItem_Buff& _Buff);

public:

	//获取当前生命值
	UFUNCTION(BlueprintCallable)
		float GetCurrentHP();
	//获取总生命值
	UFUNCTION(BlueprintCallable)
		float GetTotalHP();
	//获取当前速度
	UFUNCTION(BlueprintCallable)
		float GetCurrentMoveSpeed();
	//设置当前速度
	UFUNCTION(BlueprintCallable)
		void SetCurrentMoveSpeed(const float _Value);
	//获取原本的速度
	UFUNCTION(BlueprintCallable)
		float GetMoveSpeed();
	//获取当前攻击力
	UFUNCTION(BlueprintCallable)
		float GetCurrentATK();
	//获取原本的攻击力
	UFUNCTION(BlueprintCallable)
		float GetATK();
	//获取是否受到伤害
	UFUNCTION(BlueprintCallable)
		bool GetbIsHurt();
	//获取受到的伤害值
	UFUNCTION(BlueprintCallable)
		float GetHurtValue();
	//获取是否在攻击
	UFUNCTION(BlueprintCallable)
		bool GetbIsAttack() const;
	//获取是否可以移动
	UFUNCTION(BlueprintCallable)
		bool GetbIsMove();
	//获取老鼠路线类型
	UFUNCTION(BlueprintCallable)
		ELineType GetMouseLineType();
	//获取老鼠当前的格子位置
	UFUNCTION(BlueprintCallable)
		FLine& GetMouseLine();
	//获取老鼠的位置
	UFUNCTION(BlueprintCallable)
		USceneComponent* const GetMouseLocation();
	//获取是否处于Buff状态
	UFUNCTION(BlueprintCallable)
		bool GetBufff();
	//获取最终buff的结果->false 表示不能进行任何行为  true表示可以进行其他行为
	UFUNCTION(BlueprintCallable)
		bool GetBuffResult() const;
	//获取老鼠所在的回合
	UFUNCTION(BlueprintCallable)
		int32 GetMouseRound();
	//获取当前正在攻击的卡片
	UFUNCTION(BlueprintCallable)
		ACardActor* GetCurrentAttackCard();
	//获取当前老鼠是否处于攻击状态
	UFUNCTION(BlueprintCallable)
		bool GetbAttackState() const;
	//获取老鼠的中心位置
	UFUNCTION(BlueprintCallable)
		FVector GetMouseTargetPosition();
	//获取路径
	UFUNCTION(BlueprintCallable)
		FString GetPath(const FString& _SourcePath, const FString& _Name);
	//获取其他buff是否已经存在
	UFUNCTION(BlueprintCallable)
		bool GetBuffExist();

	//获取老鼠基本类型
	UFUNCTION()
		virtual EMouseBaseType GetMouseBaseType();
public:
	//设置经验值
	UFUNCTION(BlueprintCallable)
		void SetEx(const int32& _Ex);
	//设置原本的速度
	UFUNCTION(BlueprintCallable)
		void SetMoveSpeed(const float _Value);
	//设置生命值
	UFUNCTION(BlueprintCallable)
		void SetHP(float HP);
	//设置老鼠攻击力
	UFUNCTION(BlueprintCallable)
		void SetMouseATK(float _Value);
	//设置是否受到伤害
	UFUNCTION(BlueprintCallable)
		void SetbIsHurt(bool _Value);
	//设置受到的伤害值
	UFUNCTION(BlueprintCallable)
		void SetHurtValue(float _Value);
	//设置是否在攻击
	UFUNCTION(BlueprintCallable)
		void SetbIsAttack(bool _Value);
	//设置老鼠线路类型
	UFUNCTION(BlueprintCallable)
		void SetMouseLineType(ELineType _Type);
	//设置老鼠当前的格子位置
	UFUNCTION(BlueprintCallable)
		void SetMouseLine(FLine _Line);
	//设置老鼠的位置
	UFUNCTION(BlueprintCallable)
		void SetMouseLocation(USceneComponent* _Position);
	//获取老鼠是否死亡
	UFUNCTION(BlueprintCallable)
		bool GetMouseIsDeath();
	//设置老鼠是否死亡
	UFUNCTION(BlueprintCallable)
		void SetMouseDeath(bool _Value);
	//设置老鼠的所在回合
	UFUNCTION(BlueprintCallable)
		void SetMouseRound(int32 _Value);
	//设置正在攻击的卡片
	UFUNCTION(BlueprintCallable)
		ACardActor* SetCurrentAttackCard(ACardActor* _Card);
	//设置当前正在攻击的Actor
	UFUNCTION(BlueprintCallable)
		void SetHitSelfFlyActor(AFlyItemActor* _HitSelfFlyActor);
	//设置当前老鼠死亡时，强行进行到最后一个回合并且直接结束
	UFUNCTION(BlueprintCallable)
		void SetCurMouseDeathForceGameWin(bool Enable);
	//设置老鼠专属掉落物品
	UFUNCTION(BlueprintCallable)
		void SetMouseLevelItems(const TArray<FString>& Items);
	//设置老鼠碰撞类型(默认老鼠类型->陆地)
	UFUNCTION(BlueprintCallable)
		void SetMouseCollisionType(class UPrimitiveComponent* _Collison, class UPrimitiveComponent* _MouseBodyOverlap, ECollisionChannel _MouseType);
public:
	//关闭碰撞
	UFUNCTION(BlueprintCallable)
		void ClosedBoxComponent(UPrimitiveComponent* _Box_Comp);
	//攻击防御卡
	UFUNCTION(BlueprintCallable)
		bool AttackCard();
	//当前老鼠被消灭
	UFUNCTION(BlueprintCallable)
		void MouseKill();
	//开始移动
	UFUNCTION(BlueprintCallable)
		bool MoveStart();
	//移动停止
	UFUNCTION(BlueprintCallable)
		bool MoveStop();
	//更新生命值
	UFUNCTION(BlueprintCallable)
		void UpdateHP(float _HurtValue);
	//移除一个buff
	UFUNCTION(BlueprintCallable)
		void RemoveBuff(FMouseProperty_Buff& _Buff);
public:
	//添加一个新的buff
	UFUNCTION(BlueprintCallable)
		void AddNewBuff(FMouseProperty_Buff& _Buff, float _Time);
	//凝固buff
	UFUNCTION(BlueprintCallable)
		void AddSolidificationBuff(float _Time);
	//灼烧buff
	UFUNCTION(BlueprintCallable)
		void AddBurnBuff(float _Time);
	//冻结buff
	UFUNCTION(BlueprintCallable)
		void AddFreezeBuff(float _Time);
	//加速buff
	UFUNCTION(BlueprintCallable)
		void AddAccelerateBuff(float _Time);
	//减速buff
	UFUNCTION(BlueprintCallable)
		void AddSlowDownBuff(float _Time);
	//添加攻击线条
	UFUNCTION(BlueprintCallable)
		void AddAttackLine(const FVector& _BeginOffset, const FVector& _EndOffset, FColor _Color, ECollisionChannel _ECollisionChannel, float DeltaTime);
	UFUNCTION(BlueprintCallable)
		FORCEINLINE	void AddAttackLineFunc(const ECollisionChannel& CollisionType, const float& DeltaTime);

public:
	//老鼠目前的行为

	//移动(每0.02ms自动调用(允许移动的情况下))
	virtual void MoveingUpdate(float DeltaTime);
	//停止移动(当设置停止移动时调用一次)
	virtual void MoveingStop();
	//开始移动(当设置开始移动时调用一次)
	virtual void MoveingBegin();

	//老鼠开始攻击(如果设置了IsAttack为true 会自动调用一次)
	virtual void AttackedBegin();
	//老鼠攻击结束(如果设置了IsAttack为false 会自动调用一次)
	virtual void AttackedEnd();
	//老鼠在攻击(如果设置了IsAttack为true 会自动调用->【每当攻击动画结束时调用】)
	virtual void AttakedUpdate(float DeltaTime);

	//当老鼠被命中时受到的伤害数值(前提是isHurt为true  调用一次(一般由FlyitemActor命中时调用))
	virtual bool BeHit(float _HurtValue);

	//老鼠受到攻击((当设置isHurt为true时)调用一次)
	virtual void BeAttakedBegin();
	//老鼠没有受到攻击((当设置isHurt为false时)调用一次)
	virtual void UnHit();

	//老鼠受到攻击((当设置isHurt为true时)每0.02ms自动调用)
	virtual void BeAttakedUpdate(float DeltaTime);
	//老鼠没有受到攻击(每0.02ms自动调用)
	virtual void UnHitUpdate(float DeltaTime);

	//当老鼠死亡时(当设置老鼠SetMouseDeath(true)时调用一次)
	virtual void MouseDeathed();

public:

	//触发减速buff
	virtual void SlowDownBuff(FMouseProperty_RecvImformation& _buff);
	//减速buff结束
	virtual void SlowDownBuffOver(FMouseProperty_RecvImformation& _buff);
	//触发加速buff
	virtual void AccelerateBuff(FMouseProperty_RecvImformation& _buff);
	//加速buff结束
	virtual void AccelerateBuffOver(FMouseProperty_RecvImformation& _buff);
	//触发凝固buff
	virtual void SolidificationBuff(FMouseProperty_RecvImformation& _buff);
	//凝固buff结束
	virtual void SolidificationBuffOver(FMouseProperty_RecvImformation& _buff);
	//触发冻结buff
	virtual void FreezeBuff(FMouseProperty_RecvImformation& _buff);
	//冻结buff结束
	virtual void FreezeBuffOver(FMouseProperty_RecvImformation& _buff);
	//触发灼烧buff
	virtual void BurnBuff(FMouseProperty_RecvImformation& _buff);
	//灼烧buff结束
	virtual void BurnBuffOver(FMouseProperty_RecvImformation& _buff);

public:
	//Tick 更新
	virtual void Tick(float DeltaTime) override;
	//初始化
	virtual void BeginPlay() override;
	//老鼠初始化(Base BeginPlay自动调用一次)
	virtual void MouseInit();
public:
	//构造
	AMouseActor();

	//老鼠死亡动画播放完毕
	UFUNCTION()
		void OnMouseDeathedAnimPlayEnd();
private:
	//老鼠管理器
	UPROPERTY()
		UMouseManagerComponent* M_MouseManager = nullptr;
	//当前老鼠死亡是否立刻结束游戏
	UPROPERTY()
		bool CurMouseDeathForceGameWin = false;
	//指定掉落物品
	UPROPERTY()
		TArray<FString> CurMouseLevelItems;
private:
	//正在攻击的卡片
	UPROPERTY(EditAnywhere)
		ACardActor* M_CurrentAttackCardActor = nullptr;
	//击中老鼠的子弹
	UPROPERTY()
		AFlyItemActor* M_HitSelfFlyActor = nullptr;
};
