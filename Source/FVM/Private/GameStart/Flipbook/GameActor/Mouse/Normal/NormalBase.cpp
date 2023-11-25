// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除


#include "GameStart/Flipbook/GameActor/Mouse/Normal/NormalBase.h"

#include "GameStart/Flipbook/SpriteActor.h"

//游戏实例
#include "GameSystem/FVMGameInstance.h"
#include "GameSystem/Tools/GameSystemFunction.h"

ANormalBase::ANormalBase()
{

}


//老鼠目前的行为

//移动(每0.02ms自动调用(允许移动的情况下))
void ANormalBase::MoveingUpdate(float DeltaTime)
{
	Super::MoveingUpdate(DeltaTime);

}
//停止移动(当设置停止移动时调用一次)
void ANormalBase::MoveingStop()
{
	Super::MoveingStop();

}
//开始移动(当设置开始移动时调用一次)
void ANormalBase::MoveingBegin()
{
	Super::MoveingBegin();

}

//老鼠开始攻击(如果设置了IsAttack为true 会自动调用一次)
void ANormalBase::AttackedBegin()
{
	Super::AttackedBegin();

	this->MoveStop();
}
//老鼠攻击结束(如果设置了IsAttack为false 会自动调用一次)
void ANormalBase::AttackedEnd()
{
	Super::AttackedEnd();

	//如果老鼠没有死亡则可以移动
	if (!this->GetMouseIsDeath())
		this->MoveStart();
}
//老鼠在攻击(如果设置了IsAttack为true 会自动调用->【每当攻击动画结束时调用】)
void ANormalBase::AttakedUpdate(float DeltaTime)
{
	Super::AttakedUpdate(DeltaTime);
}

//当老鼠被命中时受到的伤害数值(前提是isHurt为true  调用一次(一般由FlyitemActor命中时调用))
bool ANormalBase::BeHit(float _HurtValue)
{
	if (!Super::BeHit(_HurtValue))
		return false;

	return true;
}

//老鼠受到攻击((当设置isHurt为true时)调用一次)
void ANormalBase::BeAttakedBegin()
{
	Super::BeAttakedBegin();
}
//老鼠没有受到攻击((当设置isHurt为false时)调用一次)
void ANormalBase::UnHit()
{
	Super::UnHit();
}

//老鼠受到攻击((当设置isHurt为true时)每0.02ms自动调用)
void ANormalBase::BeAttakedUpdate(float DeltaTime)
{
	Super::BeAttakedUpdate(DeltaTime);
}
//老鼠没有受到攻击(每0.02ms自动调用)
void ANormalBase::UnHitUpdate(float DeltaTime)
{
	Super::UnHitUpdate(DeltaTime);
}

//当老鼠死亡时(当设置老鼠SetMouseDeath(true)时调用一次)
void ANormalBase::MouseDeathed()
{
	Super::MouseDeathed();
}

//触发减速buff
void ANormalBase::SlowDownBuff(FMouseProperty_RecvImformation& _buff)
{
	Super::SlowDownBuff(_buff);
}
//减速buff结束
void ANormalBase::SlowDownBuffOver(FMouseProperty_RecvImformation& _buff)
{
	Super::SlowDownBuffOver(_buff);
}
//触发加速buff
void ANormalBase::AccelerateBuff(FMouseProperty_RecvImformation& _buff)
{
	Super::AccelerateBuff(_buff);
}
//加速buff结束
void ANormalBase::AccelerateBuffOver(FMouseProperty_RecvImformation& _buff)
{
	Super::AccelerateBuffOver(_buff);
}
//触发凝固buff
void ANormalBase::SolidificationBuff(FMouseProperty_RecvImformation& _buff)
{
	Super::SolidificationBuff(_buff);

}
//凝固buff结束
void ANormalBase::SolidificationBuffOver(FMouseProperty_RecvImformation& _buff)
{
	Super::SolidificationBuffOver(_buff);

}
//触发冻结buff
void ANormalBase::FreezeBuff(FMouseProperty_RecvImformation& _buff)
{
	Super::FreezeBuff(_buff);

	//创建冰块图层
	UGameSystemFunction::CreateIceBlock(this, FVector(0.f, 0.f, -10.f),
		this->GetRenderComponent()->GetTranslucency(), _buff.M_Freeze.M_CurrentTime);
}
//冻结buff结束
void ANormalBase::FreezeBuffOver(FMouseProperty_RecvImformation& _buff)
{
	Super::FreezeBuffOver(_buff);


	//播放冰冻结束的BGM
	UFVMGameInstance::PlayBGM_S_Static(TEXT("Freeze_Over"), TEXT("ItemAudio"));
}
//触发灼烧buff
void ANormalBase::BurnBuff(FMouseProperty_RecvImformation& _buff)
{
	Super::BurnBuff(_buff);
}
//灼烧buff结束
void ANormalBase::BurnBuffOver(FMouseProperty_RecvImformation& _buff)
{
	Super::BurnBuffOver(_buff);
}

//Tick 更新
void ANormalBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
//初始化
void ANormalBase::BeginPlay()
{
	Super::BeginPlay();
}

//老鼠初始化(Base BeginPlay自动调用一次)
void ANormalBase::MouseInit()
{
	Super::MouseInit();
}