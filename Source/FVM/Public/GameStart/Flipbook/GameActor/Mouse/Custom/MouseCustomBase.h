// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除

#pragma once

#include "CoreMinimal.h"
#include "GameStart/Flipbook/GameActor/MouseActor.h"
#include "MouseCustomBase.generated.h"


UCLASS()
class FVM_API AMouseCustomBase : public AMouseActor
{
	GENERATED_BODY()
public:
	AMouseCustomBase();
public:
	//Tick 更新
	virtual void Tick(float DeltaTime) override;
	//初始化
	virtual void BeginPlay() override;
public:
	//基本老鼠类型
	virtual EMouseBaseType GetMouseBaseType() override;
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
	//当老鼠被命中时受到的伤害数值(前提是isHurt为true  调用一次(一般由FlyitemActor命中时调用))
	virtual bool BeHit(float _HurtValue) override;
	//老鼠受到攻击((当设置isHurt为true时)调用一次)
	virtual void BeAttakedBegin() override;
	//老鼠没有受到攻击((当设置isHurt为false时)调用一次)
	virtual void UnHit();

	//老鼠受到攻击((当设置isHurt为true时)每0.02ms自动调用)
	virtual void BeAttakedUpdate(float DeltaTime) override;
	//老鼠没有受到攻击(每0.02ms自动调用)
	virtual void UnHitUpdate(float DeltaTime) override;

	//当老鼠死亡时(当设置老鼠SetMouseDeath(true)时调用一次)
	virtual void MouseDeathed() override;
};
