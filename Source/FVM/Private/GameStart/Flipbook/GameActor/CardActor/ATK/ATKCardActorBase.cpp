// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除


#include "GameStart/Flipbook/GameActor/CardActor/ATK/ATKCardActorBase.h"

void AATKCardActorBase::BeginPlay()
{
	Super::BeginPlay();
}

void AATKCardActorBase::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

float AATKCardActorBase::GetCurrentATK()
{
	return this->GetCurATK();
}

float AATKCardActorBase::GetCurrentSecondATK(const float& TAKRate)
{
	return this->GetCurrentATK() * TAKRate;
}

float AATKCardActorBase::GetCurrentAttackSpeed()
{
	return this->GetCurAttackSpeed();
}

int32 AATKCardActorBase::GetCurrentAttackCount()
{
	return this->GetAttackCount();
}

float AATKCardActorBase::GetCurrentFristAttackDelay()
{
	return this->GetFristAttackDelay();
}

float AATKCardActorBase::GetCurrentSecondAttackDelay()
{
	return this->GetSecondAttackDelay();
}

void AATKCardActorBase::SetATK(const float& ATKBase)
{
	this->ATK = ATKBase;
}

float AATKCardActorBase::GetATK() const
{
	return this->ATK;
}

void AATKCardActorBase::SetCurATK(const float& ATKBase)
{
	this->CurATK = ATKBase;
}

float AATKCardActorBase::GetCurATK() const
{
	return this->CurATK;
}

void AATKCardActorBase::SetAttackSpeed(const float& Speed)
{
	this->AttackSpeed = Speed;
}

float AATKCardActorBase::GetAttackSpeed() const
{
	return this->AttackSpeed;
}

void AATKCardActorBase::SetCurAttackSpeed(const float& Speed)
{
	this->CurAttackSpeed = Speed;
}

float AATKCardActorBase::GetCurAttackSpeed() const
{
	return this->CurAttackSpeed;
}

void AATKCardActorBase::SetAttackCount(const int32& Count)
{
	this->AttackCount = Count;
}

int32 AATKCardActorBase::GetAttackCount() const
{
	return this->AttackCount;
}

void AATKCardActorBase::SetAttackDelay(const float& First, const float& Second)
{
	this->FirstAttackTime = First;
	this->AttackEveryTime = Second;
}

float AATKCardActorBase::GetFristAttackDelay() const
{
	return this->FirstAttackTime;
}

float AATKCardActorBase::GetSecondAttackDelay() const
{
	return this->AttackEveryTime;
}
