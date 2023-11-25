// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除


#include "GameStart/Flipbook/GameActor/Mouse/Custom/MouseCustomBase.h"

AMouseCustomBase::AMouseCustomBase()
{

}

void AMouseCustomBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AMouseCustomBase::BeginPlay()
{
	Super::BeginPlay();


}

EMouseBaseType AMouseCustomBase::GetMouseBaseType()
{
	Super::GetMouseBaseType();

	return EMouseBaseType::Other;
}

void AMouseCustomBase::SlowDownBuff(FMouseProperty_RecvImformation& _buff)
{
	Super::SlowDownBuff(_buff);
}

void AMouseCustomBase::SlowDownBuffOver(FMouseProperty_RecvImformation& _buff)
{
	Super::SlowDownBuffOver(_buff);
}

void AMouseCustomBase::AccelerateBuff(FMouseProperty_RecvImformation& _buff)
{
	Super::AccelerateBuff(_buff);
}

void AMouseCustomBase::AccelerateBuffOver(FMouseProperty_RecvImformation& _buff)
{
	Super::AccelerateBuffOver(_buff);
}

void AMouseCustomBase::SolidificationBuff(FMouseProperty_RecvImformation& _buff)
{
	Super::SolidificationBuff(_buff);
}

void AMouseCustomBase::SolidificationBuffOver(FMouseProperty_RecvImformation& _buff)
{
	Super::SolidificationBuffOver(_buff);
}

void AMouseCustomBase::FreezeBuff(FMouseProperty_RecvImformation& _buff)
{
	Super::FreezeBuff(_buff);
}

void AMouseCustomBase::FreezeBuffOver(FMouseProperty_RecvImformation& _buff)
{
	Super::FreezeBuffOver(_buff);
}

void AMouseCustomBase::BurnBuff(FMouseProperty_RecvImformation& _buff)
{
	Super::BurnBuff(_buff);
}

void AMouseCustomBase::BurnBuffOver(FMouseProperty_RecvImformation& _buff)
{
	Super::BurnBuffOver(_buff);
}

bool AMouseCustomBase::BeHit(float _HurtValue)
{
	if (!Super::BeHit(_HurtValue))
		return false;

	return true;
}

void AMouseCustomBase::BeAttakedBegin()
{
	Super::BeAttakedBegin();
}

void AMouseCustomBase::UnHit()
{
	Super::UnHit();
}

void AMouseCustomBase::BeAttakedUpdate(float DeltaTime)
{
	Super::BeAttakedUpdate(DeltaTime);
}

void AMouseCustomBase::UnHitUpdate(float DeltaTime)
{
	Super::UnHitUpdate(DeltaTime);
}

void AMouseCustomBase::MouseDeathed()
{
	Super::MouseDeathed();
}
