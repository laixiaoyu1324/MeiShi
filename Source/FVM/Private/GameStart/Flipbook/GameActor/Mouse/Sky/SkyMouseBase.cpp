// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除


#include "GameStart/Flipbook/GameActor/Mouse/Sky/SkyMouseBase.h"

ASkyMouseBase::ASkyMouseBase()
{

}

void ASkyMouseBase::BeginPlay()
{
	Super::BeginPlay();
}

void ASkyMouseBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASkyMouseBase::OnAnimationPlayEnd()
{

}

void ASkyMouseBase::MoveingUpdate(float DeltaTime)
{
	Super::MoveingUpdate(DeltaTime);
}

bool ASkyMouseBase::BeHit(float _HurtValue)
{
	return Super::BeHit(_HurtValue);
}

void ASkyMouseBase::UnHit()
{
	Super::UnHit();
}

void ASkyMouseBase::AttackedBegin()
{
	Super::AttackedBegin();
}

void ASkyMouseBase::AttackedEnd()
{
	Super::AttackedEnd();
}

void ASkyMouseBase::MouseDeathed()
{
	Super::MouseDeathed();

}

void ASkyMouseBase::State()
{

}
