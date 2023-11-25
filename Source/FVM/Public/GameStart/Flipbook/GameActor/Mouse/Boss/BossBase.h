// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除

#pragma once

#include "CoreMinimal.h"
#include "GameStart/Flipbook/GameActor/MouseActor.h"

//boss 进度条
#include "GameStart/VS/UI/UI_MouseBoss_Slider.h" 

#include "GameStart/VS/MapMeshe.h"
#include "GameStart/VS/GameMapInstance.h"
#include "GameStart/VS/Components/MesheControllComponent.h"

#include "BossBase.generated.h"

/**
 *
 */

class UUI_MouseBoss_Slider;

UCLASS()
class FVM_API ABossBase : public AMouseActor
{
	GENERATED_BODY()
public:
	//boss老鼠的血量进度
	UPROPERTY()
		UUI_MouseBoss_Slider* M_BossHP_Progress = nullptr;
private:
	//tick更新速度
	UPROPERTY()
		float M_TickRate = 1.f;
public:
	ABossBase();
public:
	//Tick 更新
	virtual void Tick(float DeltaTime) override;
	//初始化
	virtual void BeginPlay() override;
	//老鼠初始化(Base BeginPlay自动调用一次)
	virtual void MouseInit() override;
public:
	//基本老鼠类型
	virtual EMouseBaseType GetMouseBaseType() override;
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

	//当老鼠被命中时受到的伤害数值(前提是isHurt为true  调用一次(一般由FlyitemActor命中时调用))
	virtual bool BeHit(float _HurtValue) override;

	//老鼠受到攻击((当设置isHurt为true时)调用一次)
	virtual void BeAttakedBegin() override;
	//老鼠没有受到攻击((当设置isHurt为false时)调用一次)
	virtual void UnHit() override;

	//老鼠受到攻击((当设置isHurt为true时)每0.02ms自动调用)
	virtual void BeAttakedUpdate(float DeltaTime) override;
	//老鼠没有受到攻击(每0.02ms自动调用)
	virtual void UnHitUpdate(float DeltaTime) override;

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
public:
	//获取网格控制组件
	UFUNCTION()
		UMesheControllComponent* GetMesheControllComponent();
	//获取地图发射器
	UFUNCTION()
		AMapLauncherBase* GetMapLauncher();
};
