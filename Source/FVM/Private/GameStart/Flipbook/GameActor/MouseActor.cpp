// Fill out your copyright notice in the Description page of Project Settings.


#include "GameStart/Flipbook/GameActor/MouseActor.h"

#include "GameStart/Flipbook/GameActor/CardActor.h"
#include "GameSystem/Tools/GameSystemFunction.h"
#include "GameStart/VS/Components/MouseManagerComponent.h"
#include "GameStart/VS/Components/ResourceManagerComponent.h"
#include "GameStart/VS/GameMapInstance.h"

//设置Buff代理函数
void SetBuffDelegate(AMouseActor* _Actor, FMouseProperty_Buff& _buff, void (AMouseActor::* FuncBegin)(), void (AMouseActor::* FuncEnd)())
{
	_buff.SetClass(_Actor);
	_buff.M_FuncBegin.SetDelegate(FuncBegin);
	_buff.M_FuncEnd.SetDelegate(FuncEnd);
}

void AMouseActor::MouseInit()
{
	//初始化血量
	this->M_Proper_HP.M_CurrentHP = this->M_Proper_HP.M_HP;
	//初始化攻击
	this->M_Proper_ATK.M_CurrentATK = this->M_Proper_ATK.M_ATK;
	//初始化速度
	this->M_Proper_State.M_CurrentMoveSpeed = this->M_Proper_State.M_MoveSpeed;
}

AMouseActor::AMouseActor()
{
	this->M_MousePosition = CreateDefaultSubobject<USceneComponent>("MouseTargetPositionComp");

	//设置依附
	this->M_MousePosition->SetupAttachment(this->GetMyActor(), TEXT("MouseTargetLocationComp"));

	//初始化代理(加速buff代理)
	SetBuffDelegate(this, this->M_RecvInformation.M_Accelerate, &AMouseActor::AccelerateFunc, &AMouseActor::AccelerateFuncEnd);
	SetBuffDelegate(this, this->M_RecvInformation.M_Freeze, &AMouseActor::FreezeFunc, &AMouseActor::FreezeFuncEnd);
	SetBuffDelegate(this, this->M_RecvInformation.M_SlowDown, &AMouseActor::SlowDownFunc, &AMouseActor::SlowDownFuncEnd);
	SetBuffDelegate(this, this->M_RecvInformation.M_Solidification, &AMouseActor::SolidificationFunc, &AMouseActor::SolidificationFuncEnd);
	SetBuffDelegate(this, this->M_RecvInformation.M_Burn, &AMouseActor::BurnFunc, &AMouseActor::BurnFuncEnd);
}

void AMouseActor::OnMouseDeathedAnimPlayEnd()
{
	this->MouseKill();
	this->Destroy();
}

void AMouseActor::AddBuffFunc(const float _DeltaTime, FMouseProperty_Buff& _buff, int32& _BuffCountResult)
{
	//处理Buff
	if (_buff.M_bCondition)
	{
		//处理
		if (!_buff.M_bBegin && _buff.GetClass() && !_buff.GetClass()->IsPendingKillPending())
		{
			//调用Begin Delegate
			if (_buff.M_FuncBegin.Func)
			{
				_buff.M_bBegin = true;
				(_buff.GetClass()->*_buff.M_FuncBegin.Func)();
			}
		}

		//倒计时
		_buff.M_CurrentTime -= _DeltaTime;

		//当时间大于0  表示这个buff是存在的
		if (_buff.M_CurrentTime > 0.f)
		{
			//buff个数加1
			_BuffCountResult++;
		}
		else {
			//调用End Delegate
			RemoveBuff(_buff);
		}

	}
}

void AMouseActor::UpdateMouseColor()
{
	//首先使用默认颜色
	this->SetFlipbookColor(FVector(1.f), this->GetMyActor()->GetSpriteColor().A);

	//根据buff优先级赋值颜色

	//减速
	if (this->M_RecvInformation.M_SlowDown.M_CurrentTime > 0.f)
	{
		this->SetFlipbookColor(FVector(0.f, 0.5f, 1.f), this->GetMyActor()->GetSpriteColor().A);
	}

	//加速
	if (this->M_RecvInformation.M_Accelerate.M_CurrentTime > 0.f)
	{
		this->SetFlipbookColor(FVector(1.f, 0.f, 0.f), this->GetMyActor()->GetSpriteColor().A);
	}

	//灼烧
	if (this->M_RecvInformation.M_Burn.M_CurrentTime > 0.f)
	{
		this->SetFlipbookColor(FVector(1.f, 0.2f, 0.f), this->GetMyActor()->GetSpriteColor().A);
	}

	//冻结
	if (this->M_RecvInformation.M_Freeze.M_CurrentTime > 0.f)
	{
		this->SetFlipbookColor(FVector(0.f, 0.f, 1.f), this->GetMyActor()->GetSpriteColor().A);
	}
}

void AMouseActor::AccelerateFunc()
{
	if (UFVMGameInstance::GetDebug())
		UGameSystemFunction::FVMLog(__FUNCTION__, TEXT("执行buff:加速"));

	//判断buff（先手）（后手）
	if (this->GetBuffExist())
	{
		if (UFVMGameInstance::GetDebug())
			UGameSystemFunction::FVMLog(__FUNCTION__, TEXT("后手触发"));

		//后手(加速buff只会和减速buff产生反映)
		if (this->M_RecvInformation.M_SlowDown.M_CurrentTime > 0.f)
		{
			// 时间抵消法
			float Acc_Time = this->M_RecvInformation.M_Accelerate.M_CurrentTime;
			float Slow_Time = this->M_RecvInformation.M_SlowDown.M_CurrentTime;

			this->M_RecvInformation.M_SlowDown.M_CurrentTime -= Acc_Time;
			this->M_RecvInformation.M_Accelerate.M_CurrentTime -= Slow_Time;
		}

		//最后决定是否触发buff
		if (this->M_RecvInformation.M_Accelerate.M_CurrentTime > 0.f)
		{
			//触发加速buff
			this->AccelerateBuff(this->M_RecvInformation);
			this->SetCurrentMoveSpeed(this->GetMoveSpeed() * 1.5f);

			//如果有减速buff
			if (this->M_RecvInformation.M_Freeze.M_CurrentTime > 0.f)
			{
				this->SetCurrentMoveSpeed(this->GetMoveSpeed() * 1.0f);
			}

			//如果冻结或者凝固buff不存在的情况下
			if (this->M_RecvInformation.M_Freeze.M_CurrentTime <= 0.f && this->M_RecvInformation.M_Solidification.M_CurrentTime <= 0.f)
			{
				this->SetAnimationPlayRate(1.5f);

				//如果有减速buff
				if (this->M_RecvInformation.M_Freeze.M_CurrentTime > 0.f)
				{
					this->SetAnimationPlayRate(1.0f);
				}
			}
		}
	}
	else {
		//先手
		if (UFVMGameInstance::GetDebug())
			UGameSystemFunction::FVMLog(__FUNCTION__, TEXT("先手触发"));

		//触发加速buff
		this->AccelerateBuff(this->M_RecvInformation);
		this->SetCurrentMoveSpeed(this->GetMoveSpeed() * 1.5f);
		this->SetAnimationPlayRate(1.5f);
	}
}

void AMouseActor::AccelerateFuncEnd()
{
	if (UFVMGameInstance::GetDebug())
		UGameSystemFunction::FVMLog(__FUNCTION__, TEXT("执行buff:加速结束"));

	//判断buff（先手）（后手）
	if (this->GetBuffExist())
	{
		if (UFVMGameInstance::GetDebug())
			UGameSystemFunction::FVMLog(__FUNCTION__, TEXT("后手触发"));

		//触发加速buff结束
		this->AccelerateBuffOver(this->M_RecvInformation);

		this->SetAnimationPlayRate(1.f);
		this->SetCurrentMoveSpeed(this->GetMoveSpeed() * 1.f);

		//如果减速buff
		if (this->M_RecvInformation.M_SlowDown.M_CurrentTime > 0.f)
		{
			this->SetAnimationPlayRate(0.5f);
			this->SetCurrentMoveSpeed(this->GetMoveSpeed() * 0.5f);
		}

		//如果有冻结或者凝固
		if (this->M_RecvInformation.M_Freeze.M_CurrentTime > 0.f || this->M_RecvInformation.M_Solidification.M_CurrentTime > 0.f)
		{
			this->SetAnimationPlayRate(0.f);
		}
	}
	else {
		if (UFVMGameInstance::GetDebug())
			UGameSystemFunction::FVMLog(__FUNCTION__, TEXT("先手触发"));

		//触发加速buff结束
		this->AccelerateBuffOver(this->M_RecvInformation);
		this->SetCurrentMoveSpeed(this->GetMoveSpeed() * 1.f);
		this->SetAnimationPlayRate(1.f);
		this->SetFlipbookColor(FVector(1.f), this->GetMyActor()->GetSpriteColor().A);
	}
}

void AMouseActor::SlowDownFunc()
{
	if (UFVMGameInstance::GetDebug())
		UGameSystemFunction::FVMLog(__FUNCTION__, TEXT("执行buff:减速"));

	//判断buff（先手）（后手）
	if (this->GetBuffExist())
	{
		if (UFVMGameInstance::GetDebug())
			UGameSystemFunction::FVMLog(__FUNCTION__, TEXT("后手触发"));

		// 时间抵消法
		float& Slow_Time = this->M_RecvInformation.M_SlowDown.M_CurrentTime;

		//触发顺序->灼烧->加速   其他buff不反应
		if (this->M_RecvInformation.M_Burn.M_CurrentTime > 0.f)
		{
			float Burn_Time = this->M_RecvInformation.M_Burn.M_CurrentTime;

			this->M_RecvInformation.M_Burn.M_CurrentTime -= Slow_Time;

			//时间抵消
			Slow_Time -= Burn_Time;
		}

		//继续触发反应
		if (this->M_RecvInformation.M_Accelerate.M_CurrentTime > 0.f && Slow_Time > 0.f)
		{
			float Acce_Time = this->M_RecvInformation.M_Accelerate.M_CurrentTime;
			this->M_RecvInformation.M_Accelerate.M_CurrentTime -= Slow_Time;
			Slow_Time -= Acce_Time;
		}

		//最终决定是否触发减速buff
		if (Slow_Time > 0.f)
		{
			//触发减速
			this->SlowDownBuff(this->M_RecvInformation);
			this->SetCurrentMoveSpeed(this->GetMoveSpeed() * 0.5f);

			//如果此时加速buff还存在
			if (this->M_RecvInformation.M_Accelerate.M_CurrentTime > 0.f)
			{
				this->SetCurrentMoveSpeed(this->GetMoveSpeed() * 1.0f);
			}

			//减速动画播放受到（凝固、冻结）的影响[两者都不存在的情况下]
			if (this->M_RecvInformation.M_Freeze.M_CurrentTime <= 0.f && this->M_RecvInformation.M_Solidification.M_CurrentTime <= 0.f)
			{
				this->SetAnimationPlayRate(0.5f);

				//如果此时加速buff还存在
				if (this->M_RecvInformation.M_Accelerate.M_CurrentTime > 0.f)
				{
					this->SetAnimationPlayRate(1.f);
				}
			}
		}

	}
	else {
		if (UFVMGameInstance::GetDebug())
			UGameSystemFunction::FVMLog(__FUNCTION__, TEXT("先手触发"));

		//触发减速
		this->SlowDownBuff(this->M_RecvInformation);
		this->SetAnimationPlayRate(0.5f);
		this->SetCurrentMoveSpeed(this->GetMoveSpeed() * 0.5f);
	}
}

void AMouseActor::SlowDownFuncEnd()
{
	if (UFVMGameInstance::GetDebug())
		UGameSystemFunction::FVMLog(__FUNCTION__, TEXT("执行buff:减速结束"));

	//判断buff（先手）（后手）
	if (this->GetBuffExist())
	{
		if (UFVMGameInstance::GetDebug())
			UGameSystemFunction::FVMLog(__FUNCTION__, TEXT("后手触发"));

		//触发减速结束
		this->SlowDownBuffOver(this->M_RecvInformation);
		this->SetAnimationPlayRate(1.f);
		this->SetCurrentMoveSpeed(this->GetMoveSpeed() * 1.0f);

		//如果有加速buff
		if (this->M_RecvInformation.M_Accelerate.M_CurrentTime > 0.f)
		{
			this->SetAnimationPlayRate(1.5f);
			this->SetCurrentMoveSpeed(this->GetMoveSpeed() * 1.5);
		}

		//如果有冻结或者凝固
		if (this->M_RecvInformation.M_Freeze.M_CurrentTime > 0.f || this->M_RecvInformation.M_Solidification.M_CurrentTime > 0.f)
		{
			this->SetAnimationPlayRate(0.f);
			this->M_Proper_Condition.M_bBuffResult = false;
		}

	}
	else {
		if (UFVMGameInstance::GetDebug())
			UGameSystemFunction::FVMLog(__FUNCTION__, TEXT("先手触发"));

		//如果有冻结或者凝固
		if (this->M_RecvInformation.M_Freeze.M_CurrentTime > 0.f || this->M_RecvInformation.M_Solidification.M_CurrentTime > 0.f)
		{
			this->SetAnimationPlayRate(0.f);
			this->M_Proper_Condition.M_bBuffResult = false;
		}
		else {
			//触发减速结束
			this->SlowDownBuffOver(this->M_RecvInformation);
			this->SetAnimationPlayRate(1.f);
			this->SetCurrentMoveSpeed(this->GetMoveSpeed() * 1.0f);
			this->M_Proper_Condition.M_bBuffResult = true;
		}
	}
}

void AMouseActor::SolidificationFunc()
{
	if (UFVMGameInstance::GetDebug())
		UGameSystemFunction::FVMLog(__FUNCTION__, TEXT("执行buff:凝固"));

	//凝固buff 不与其他任何buff发生反应
	if (UFVMGameInstance::GetDebug())
		UGameSystemFunction::FVMLog(__FUNCTION__, TEXT("直接触发"));

	//触发凝固
	this->SolidificationBuff(this->M_RecvInformation);
	this->SetAnimationPlayRate(0.f);
	this->M_Proper_Condition.M_bBuffResult = false;

	//判断是否死亡
	if (this->GetCurrentHP() <= 0.f)
	{
		this->M_Proper_Condition.M_bBuffResult = false;
		this->SetAnimationPlayRate(1.f);
	}
}

void AMouseActor::SolidificationFuncEnd()
{
	if (UFVMGameInstance::GetDebug())
		UGameSystemFunction::FVMLog(__FUNCTION__, TEXT("执行buff:凝固结束"));

	//判断buff（先手）（后手）
	if (this->GetBuffExist())
	{
		if (UFVMGameInstance::GetDebug())
			UGameSystemFunction::FVMLog(__FUNCTION__, TEXT("后手触发"));

		//触发凝固buff结束
		this->SolidificationBuffOver(this->M_RecvInformation);

		//(冰冻buff不存在)   凝固buff的最终返回[结果buff]受到冻结buff的影响
		if (this->M_RecvInformation.M_Freeze.M_CurrentTime <= 0.f)
		{

			this->SetAnimationPlayRate(1.f);

			//如果有减速buff
			if (this->M_RecvInformation.M_SlowDown.M_CurrentTime > 0.f)
			{
				this->SetAnimationPlayRate(0.7f);
			}

			//如果有加速buff
			if (this->M_RecvInformation.M_Accelerate.M_CurrentTime > 0.f)
			{
				this->SetAnimationPlayRate(2.f);
			}

			this->M_Proper_Condition.M_bBuffResult = true;
		}
	}
	else {
		if (UFVMGameInstance::GetDebug())
			UGameSystemFunction::FVMLog(__FUNCTION__, TEXT("先手触发"));

		//触发凝固buff结束
		this->SolidificationBuffOver(this->M_RecvInformation);

		this->SetAnimationPlayRate(1.f);
		this->SetFlipbookColor(FVector(1.f), this->GetMyActor()->GetSpriteColor().A);
		this->M_Proper_Condition.M_bBuffResult = true;

	}
}

void AMouseActor::FreezeFunc()
{
	if (UFVMGameInstance::GetDebug())
		UGameSystemFunction::FVMLog(__FUNCTION__, TEXT("执行buff:冻结"));

	//判断buff（先手）（后手）
	if (this->GetBuffExist())
	{
		if (UFVMGameInstance::GetDebug())
			UGameSystemFunction::FVMLog(__FUNCTION__, TEXT("后手触发"));

		float& Free_Time = this->M_RecvInformation.M_Freeze.M_CurrentTime;

		//触发顺序->灼烧
		if (this->M_RecvInformation.M_Burn.M_CurrentTime > 0.f)
		{
			//时间抵消
			float Burn_Time = this->M_RecvInformation.M_Burn.M_CurrentTime;
			this->M_RecvInformation.M_Burn.M_CurrentTime = Free_Time;
			Free_Time -= Burn_Time;
		}

		//没有其他可以反应的buff
		if (Free_Time > 0.f)
		{
			//触发冻结
			this->FreezeBuff(this->M_RecvInformation);
			this->SetAnimationPlayRate(0.f);
			this->SetFlipbookColor(FVector(0.f, 0.f, 1.f), this->GetMyActor()->GetSpriteColor().A);
			this->M_Proper_Condition.M_bBuffResult = false;
		}
	}
	else {
		if (UFVMGameInstance::GetDebug())
			UGameSystemFunction::FVMLog(__FUNCTION__, TEXT("先手触发"));
		//触发冻结
		this->FreezeBuff(this->M_RecvInformation);
		this->SetAnimationPlayRate(0.f);
		this->SetFlipbookColor(FVector(0.f, 0.f, 1.f), this->GetMyActor()->GetSpriteColor().A);
		this->M_Proper_Condition.M_bBuffResult = false;
	}

	//判断是否死亡
	if (this->GetCurrentHP() <= 0.f)
	{
		this->M_Proper_Condition.M_bBuffResult = false;
		this->SetAnimationPlayRate(0.7f);
	}
}

void AMouseActor::FreezeFuncEnd()
{
	if (UFVMGameInstance::GetDebug())
		UGameSystemFunction::FVMLog(__FUNCTION__, TEXT("执行buff:冰冻结束"));

	//判断buff（先手）（后手）
	if (this->GetBuffExist())
	{
		if (UFVMGameInstance::GetDebug())
			UGameSystemFunction::FVMLog(__FUNCTION__, TEXT("后手触发"));

		//如果凝固buff不存在
		if (this->M_RecvInformation.M_Solidification.M_CurrentTime <= 0.f)
		{
			//冻结结束
			this->FreezeBuffOver(this->M_RecvInformation);
			//启动减速buff
			this->AddNewBuff(this->M_RecvInformation.M_SlowDown, 10.f);

			this->M_Proper_Condition.M_bBuffResult = true;
		}
	}
	else {
		if (UFVMGameInstance::GetDebug())
			UGameSystemFunction::FVMLog(__FUNCTION__, TEXT("先手触发"));

		//冻结结束
		this->FreezeBuffOver(this->M_RecvInformation);

		this->M_Proper_Condition.M_bBuffResult = true;
		//启动减速buff
		this->AddNewBuff(this->M_RecvInformation.M_SlowDown, 10.f);
	}
}

void AMouseActor::BurnFunc()
{
	if (UFVMGameInstance::GetDebug())
		UGameSystemFunction::FVMLog(__FUNCTION__, TEXT("执行buff:灼烧"));

	//判断buff（先手）（后手）
	if (this->GetBuffExist())
	{
		if (UFVMGameInstance::GetDebug())
			UGameSystemFunction::FVMLog(__FUNCTION__, TEXT("后手触发"));

		//触发反应顺序->冻结->减速
		float& Burn_Time = this->M_RecvInformation.M_Burn.M_CurrentTime;

		//如果存在冻结buff
		if (this->M_RecvInformation.M_Freeze.M_CurrentTime > 0.f)
		{
			float Free_Time = this->M_RecvInformation.M_Freeze.M_CurrentTime;
			this->M_RecvInformation.M_Freeze.M_CurrentTime -= Burn_Time;
			Burn_Time -= Free_Time;
			//移除冻结buff
			this->RemoveBuff(this->M_RecvInformation.M_Freeze);
		}

		//如果存在减速buff
		if (this->M_RecvInformation.M_SlowDown.M_CurrentTime > 0.f && Burn_Time > 0.f)
		{
			float Slow_Time = this->M_RecvInformation.M_SlowDown.M_CurrentTime;
			this->M_RecvInformation.M_SlowDown.M_CurrentTime -= Burn_Time;
			Burn_Time -= Slow_Time;
		}

		//还有剩余则触发灼烧buff
		if (Burn_Time > 0.f)
		{
			//触发灼烧
			this->BurnBuff(this->M_RecvInformation);
			this->SetFlipbookColor(FVector(1.f, 0.2f, 0.f), this->GetMyActor()->GetSpriteColor().A);
		}
	}
	else {
		if (UFVMGameInstance::GetDebug())
			UGameSystemFunction::FVMLog(__FUNCTION__, TEXT("先手触发"));

		//触发灼烧
		this->BurnBuff(this->M_RecvInformation);
		this->SetFlipbookColor(FVector(1.f, 0.2f, 0.f), this->GetMyActor()->GetSpriteColor().A);
	}
}

void AMouseActor::BurnFuncEnd()
{
	if (UFVMGameInstance::GetDebug())
		UGameSystemFunction::FVMLog(__FUNCTION__, TEXT("执行buff:灼烧结束"));

	//判断buff（先手）（后手）
	if (this->GetBuffExist())
	{
		if (UFVMGameInstance::GetDebug())
			UGameSystemFunction::FVMLog(__FUNCTION__, TEXT("后手触发"));

		//触发灼烧结束
		this->BurnBuffOver(this->M_RecvInformation);
	}
	else {

		if (UFVMGameInstance::GetDebug())
			UGameSystemFunction::FVMLog(__FUNCTION__, TEXT("先手触发"));

		//触发灼烧结束
		this->BurnBuffOver(this->M_RecvInformation);
		this->SetFlipbookColor(FVector(1.f), this->GetMyActor()->GetSpriteColor().A);
	}
}

void AMouseActor::ParseBuff_Information(FItem_Buff& _Buff)
{
	auto AddBuff = [&](uint8 _Nums, float _Time)->void {
		switch (_Nums)
		{
		case 0:this->AddSlowDownBuff(_Time); break;
		case 1:this->AddFreezeBuff(_Time); break;
		case 2:this->AddSolidificationBuff(_Time); break;
		case 3:this->AddAccelerateBuff(_Time); break;
		default:this->AddBurnBuff(_Time); break;
		}
	};

	//是否携带buff条件
	if (_Buff.M_bCondition)
	{
		for (auto Infor : _Buff.M_Buffers)
		{
			AddBuff((uint8)(Infor.Key), Infor.Value);
		}
	}
}

void AMouseActor::MouseKill()
{
	if (!this->M_Proper_State.IsDeath)
	{
		return;
	}

	if (!FVM_VS_GameOver())
	{
		return;
	}

	//防止多次调用
	this->M_Proper_State.IsDeath = false;

	//取消Tick的使用
	this->M_Proper_State.IsNotTick = true;

	//将当前老鼠从线路管理器中移除
	UMouseManagerComponent* MouseManager = AGameMapInstance::GetGameMapInstance()->M_MouseManagerComponent;

	if (IsValid(MouseManager))
	{
		//生成专属掉落物品
		if (this->CurMouseLevelItems.Num())
		{
			//立刻生成
			for (const FString& CurItem : this->CurMouseLevelItems)
			{
				UResourceManagerComponent::SpawnItemByName(CurItem, this->GetActorLocation());
			}
		}

		//生成回合掉落物
		UResourceManagerComponent::SpawnItem(this->GetActorLocation());

		//生成金币资源
		UResourceManagerComponent::SpawnItemCoin(this->GetActorLocation());

		//生成经验值
		UResourceManagerComponent::ResourceAddEx(this->M_Proper_State.M_CurrentMouseEx);

		//添加得分
		if (this->GetMouseBaseType() == EMouseBaseType::Boss)
		{
			UResourceManagerComponent::ResourceAddScore(UGameSystemFunction::GetRandomRange(95, 100));
		}
		else if (this->GetMouseBaseType() == EMouseBaseType::Other)
		{
			UResourceManagerComponent::ResourceAddScore(UGameSystemFunction::GetRandomRange(1, 5));
		}
		else {
			UResourceManagerComponent::ResourceAddScore(UGameSystemFunction::GetRandomRange(5, 10));
		}

		if (this->CurMouseDeathForceGameWin)
		{
			MouseManager->ForceNextEndRound();
		}
		else {
			MouseManager->RemoveMouse(this);
		}
	}

	if (UFVMGameInstance::GetDebug())
	{
		UGameSystemFunction::FVMLog(__FUNCTION__, TEXT("老鼠死亡"));
	}
}

void AMouseActor::UpdateHP(float _HurtValue)
{
	//血量更新
	this->M_Proper_HP.M_CurrentHP -= _HurtValue;

	if (this->M_Proper_HP.M_CurrentHP <= 0.f)
	{
		this->M_Proper_HP.M_CurrentHP = 0.f;
		this->SetMouseDeath(true);
	}

	//结束受攻击状态(更新血量的函数一般在受到伤害时才会触发)
	this->SetbIsHurt(false);
}

void AMouseActor::SetHP(float HP)
{
	this->M_Proper_HP.M_HP = HP;
	this->M_Proper_HP.M_CurrentHP = HP;
}

float AMouseActor::GetCurrentHP()
{
	return this->M_Proper_HP.M_CurrentHP;
}

float AMouseActor::GetTotalHP()
{
	return this->M_Proper_HP.M_HP;
}

float AMouseActor::GetCurrentMoveSpeed()
{
	return this->M_Proper_State.M_CurrentMoveSpeed;
}

void AMouseActor::SetCurrentMoveSpeed(const float _Value)
{
	this->M_Proper_State.M_CurrentMoveSpeed = _Value;
}

float AMouseActor::GetMoveSpeed()
{
	return this->M_Proper_State.M_MoveSpeed;
}

void AMouseActor::SetMoveSpeed(const float _Value)
{
	this->M_Proper_State.M_MoveSpeed = _Value;
	this->SetCurrentMoveSpeed(_Value);
}

float AMouseActor::GetCurrentATK()
{
	return this->M_Proper_ATK.M_CurrentATK;
}

float AMouseActor::GetATK()
{
	return this->M_Proper_ATK.M_ATK;
}

void AMouseActor::SetMouseATK(float _Value)
{
	this->M_Proper_ATK.M_CurrentATK = _Value;
	this->M_Proper_ATK.M_ATK = _Value;
}

bool AMouseActor::GetbIsHurt()
{
	return this->M_Proper_ATK.IsHurt;
}

void AMouseActor::SetbIsHurt(bool _Value)
{
	this->M_Proper_ATK.IsHurt = _Value;

	if (_Value)
	{
		//第一次受到攻击时
		this->BeAttakedBegin();
	}
	else
	{
		//第一次未受到攻击时
		this->UnHit();
	}
}

float AMouseActor::GetHurtValue()
{
	return this->M_Proper_ATK.HurtValue;
}

void AMouseActor::SetHurtValue(float _Value)
{
	this->M_Proper_ATK.HurtValue = _Value;
}

bool AMouseActor::GetbIsAttack() const
{
	return this->M_Proper_ATK.IsAttack;
}

void AMouseActor::SetbIsAttack(bool _Value)
{
	this->M_Proper_ATK.IsAttack = _Value;

	if (_Value)
	{
		//开始攻击
		this->AttackedBegin();
	}
	else
	{
		//攻击结束
		this->AttackedEnd();
	}
}

bool AMouseActor::MoveStart()
{
	if (!this->M_Proper_State.M_IsMove)
	{
		this->M_Proper_State.M_IsMove = true;

		//开始移动时调用一次
		this->MoveingBegin();

		return true;
	}
	return false;
}

bool AMouseActor::MoveStop()
{
	if (this->M_Proper_State.M_IsMove)
	{
		this->M_Proper_State.M_IsMove = false;

		//结束移动时调用一次
		this->MoveingStop();

		return true;
	}
	return false;
}

bool AMouseActor::GetbIsMove()
{
	return M_Proper_State.M_IsMove;
}

void AMouseActor::SetMouseLineType(ELineType _Type)
{
	this->M_Proper_Condition.M_EMouseType = _Type;
}

ELineType AMouseActor::GetMouseLineType()
{
	return this->M_Proper_Condition.M_EMouseType;
}

FLine& AMouseActor::GetMouseLine()
{
	return this->M_Proper_Condition.M_Line;
}

void AMouseActor::SetMouseLine(FLine _Line)
{
	this->M_Proper_Condition.M_Line = _Line;
}

USceneComponent* const AMouseActor::GetMouseLocation()
{
	return this->M_MousePosition;
}

void AMouseActor::SetMouseLocation(USceneComponent* _Position)
{
	this->M_MousePosition = _Position;
}

bool AMouseActor::GetMouseIsDeath()
{
	return this->M_Proper_State.MouseDeath;
}

void AMouseActor::SetMouseDeath(bool _Value)
{
	if (!_Value && this->M_Proper_State.IsDeath == true)
	{
		return;
	}

	this->M_Proper_State.IsDeath = _Value;
	this->M_Proper_State.MouseDeath = _Value;
	this->M_Proper_State.bPlayDeathAnim = _Value;
	this->M_Proper_HP.M_CurrentHP = 0.f;

	if (_Value)
	{
		this->MouseDeathed();
	}
}

bool AMouseActor::GetBufff()
{
	return this->M_Proper_Condition.M_bBuffState;
}
bool AMouseActor::GetBuffResult() const
{
	return this->M_Proper_Condition.M_bBuffResult;
}
int32 AMouseActor::GetMouseRound()
{
	return this->M_Proper_Condition.M_AttachRound;
}

void AMouseActor::SetMouseRound(int32 _Value)
{
	this->M_Proper_Condition.M_AttachRound = _Value;
}

ACardActor* AMouseActor::GetCurrentAttackCard()
{
	return this->M_CurrentAttackCardActor;
}

bool AMouseActor::GetbAttackState() const
{
	//如果老鼠处于攻击状态
	if (this->GetBuffResult() && this->GetbIsAttack())
	{
		return true;
	}
	return false;
}

ACardActor* AMouseActor::SetCurrentAttackCard(ACardActor* _Card)
{
	if (!IsValid(_Card))
	{
		this->M_CurrentAttackCardActor = nullptr;
		return nullptr;
	}

	//如果当前卡片无效
	if (!IsValid(this->M_CurrentAttackCardActor))
	{
		this->M_CurrentAttackCardActor = _Card;
		return _Card;
	}

	//如果当前新卡片 0
	if (this->M_CurrentAttackCardActor->GetCardData().M_CardLayer == 1 && _Card->GetCardData().M_CardLayer == 0)
	{
		return this->M_CurrentAttackCardActor;
	}

	//如果当前新卡片 1
	if (_Card->GetCardData().M_CardLayer == 1)
	{
		this->M_CurrentAttackCardActor = _Card;
		return _Card;
	}


	this->M_CurrentAttackCardActor = _Card;

	return _Card;
}

void AMouseActor::SetHitSelfFlyActor(AFlyItemActor* _HitSelfFlyActor)
{
	this->M_HitSelfFlyActor = _HitSelfFlyActor;
}

FVector AMouseActor::GetMouseTargetPosition()
{
	return 	this->GetActorLocation() + this->M_MousePosition->GetComponentLocation();
}

FString AMouseActor::GetPath(const FString& _SourcePath, const FString& _Name)
{
	return FString(_SourcePath + _Name + TEXT(".") + _Name + TEXT("'"));
}

bool AMouseActor::GetBuffExist()
{
	int32 M_Count = 0;

	if (this->M_RecvInformation.M_Accelerate.M_CurrentTime > 0.f)
	{
		M_Count++;
	}

	if (this->M_RecvInformation.M_Burn.M_CurrentTime > 0.f)
	{
		M_Count++;
	}

	if (this->M_RecvInformation.M_Freeze.M_CurrentTime > 0.f)
	{
		M_Count++;
	}

	if (this->M_RecvInformation.M_SlowDown.M_CurrentTime > 0.f)
	{
		M_Count++;
	}

	if (this->M_RecvInformation.M_Solidification.M_CurrentTime > 0.f)
	{
		M_Count++;
	}

	return M_Count <= 1 ? false : true;
}

EMouseBaseType AMouseActor::GetMouseBaseType()
{
	return EMouseBaseType::Normal;
}

void AMouseActor::SetEx(const int32& _Ex)
{
	this->M_Proper_State.M_CurrentMouseEx = _Ex;
}

void AMouseActor::SetCurMouseDeathForceGameWin(bool Enable)
{
	this->CurMouseDeathForceGameWin = Enable;
}

void AMouseActor::SetMouseLevelItems(const TArray<FString>& Items)
{
	this->CurMouseLevelItems = Items;
}

void AMouseActor::SetMouseCollisionType(UPrimitiveComponent* _Collison, UPrimitiveComponent* _MouseBodyOverlap, ECollisionChannel _MouseType)
{
	if (_Collison && _MouseBodyOverlap)
	{
		_Collison->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		_Collison->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
		_Collison->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
		_Collison->SetCollisionResponseToChannel(_MouseType, ECollisionResponse::ECR_Block);
		_Collison->SetCollisionObjectType(ECC_Pawn);

		_MouseBodyOverlap->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		_MouseBodyOverlap->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
		_MouseBodyOverlap->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
		_MouseBodyOverlap->SetCollisionObjectType(ECC_Pawn);
	}
}

void AMouseActor::ClosedBoxComponent(UPrimitiveComponent* _Box_Comp)
{
	if (_Box_Comp)
	{
		_Box_Comp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

bool AMouseActor::AttackCard()
{
	//当前卡片无效
	if (!IsValid(this->GetCurrentAttackCard()))
	{
		return false;
	}

	//当前卡片有效
	if (IsValid(this->GetCurrentAttackCard()) && this->GetCurrentAttackCard()->GetCurrentHP() > 0.f)
	{
		//设置当前攻击该卡片的老鼠
		this->GetCurrentAttackCard()->SetCurrentAttackSelfMouse(this);

		//当卡片死亡时
		if (this->GetCurrentAttackCard()->UpdateCardState(this->GetCurrentATK(), 0.f))
		{
			//销毁卡片
			this->GetCurrentAttackCard()->KillCard();
			//卡片摧毁
			UResourceManagerComponent::ResourceAddBadCard();
			return false;
		}

		//调用卡片被攻击的函数
		this->GetCurrentAttackCard()->CardBeHurt(this);

		//卡片没有死亡 //还有目标继续攻击
		return true;
	}

	return true;
}

void AMouseActor::AddNewBuff(FMouseProperty_Buff& _Buff, float _Time)
{
	//如果这个buff的时间是小于或者等于0的时候->才表示是一个新buff
	this->M_Proper_Condition.M_bBuffState = true;
	_Buff.M_bCondition = true;
	_Buff.M_bBegin = false;
	//时间判断
	if (_Buff.M_CurrentTime < _Time)
		_Buff.M_CurrentTime = _Time;
}

void AMouseActor::RemoveBuff(FMouseProperty_Buff& _Buff)
{
	_Buff.Clear();
}

void AMouseActor::AddSolidificationBuff(float _Time)
{
	this->AddNewBuff(this->M_RecvInformation.M_Solidification, _Time);
}

void AMouseActor::AddBurnBuff(float _Time)
{
	this->AddNewBuff(this->M_RecvInformation.M_Burn, _Time);
}

void AMouseActor::AddFreezeBuff(float _Time)
{
	this->AddNewBuff(this->M_RecvInformation.M_Freeze, _Time);
}

void AMouseActor::AddAccelerateBuff(float _Time)
{
	this->AddNewBuff(this->M_RecvInformation.M_Accelerate, _Time);
}

void AMouseActor::AddSlowDownBuff(float _Time)
{
	this->AddNewBuff(this->M_RecvInformation.M_SlowDown, _Time);
}

void AMouseActor::AddAttackLine(const FVector& _BeginOffset, const FVector& _EndOffset, FColor _Color, ECollisionChannel _ECollisionChannel, float DeltaTime)
{
	//添加射线
	FHitResult Hit;

	if (UFVMGameInstance::GetDebug())
		DrawDebugLine(this->GetWorld(), this->GetActorLocation() + _BeginOffset, this->GetActorLocation() + _EndOffset, _Color);

	if (this->GetWorld()->LineTraceSingleByChannel(Hit, this->GetActorLocation(),
		this->GetActorLocation() + _EndOffset, _ECollisionChannel))
	{
		this->SetCurrentAttackCard(Cast<ACardActor>(Hit.GetActor()));

		this->MoveStop();

		//单向如果没有攻击，则开始攻击
		if (!this->GetbIsAttack())
		{
			this->SetbIsAttack(true);
		}
	}
	else {

		if (!this->GetMouseIsDeath())
		{
			this->MoveStart();
		}
		else {
			this->MoveStop();
		}

		this->SetCurrentAttackCard(0);

		//如果有在攻击，则取消攻击
		if (this->GetbIsAttack())
		{
			this->SetbIsAttack(false);
		}
	}
}

void AMouseActor::AddAttackLineFunc(const ECollisionChannel& CollisionType, const float& DeltaTime)
{
	this->AddAttackLine(FVector::ZeroVector, FVector(0.f, -15.f, 0.f), FColor::Red, CollisionType, DeltaTime);
}

void AMouseActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (FVM_VS_GamePause())
	{
		return;
	}

	//如果不能使用Tick
	if (this->M_Proper_State.IsNotTick)
	{
		return;
	}

	//如果允许移动
	if (this->GetbIsMove())
	{
		if (this->GetBuffResult() == true)
		{
			this->MoveingUpdate(DeltaTime);
		}
	}


	if (this->GetBufff())
	{
		int32 M_BuffCount = 0;

		//添加加速buff
		this->AddBuffFunc(DeltaTime, this->M_RecvInformation.M_Solidification, M_BuffCount);
		this->AddBuffFunc(DeltaTime, this->M_RecvInformation.M_Freeze, M_BuffCount);
		this->AddBuffFunc(DeltaTime, this->M_RecvInformation.M_SlowDown, M_BuffCount);
		this->AddBuffFunc(DeltaTime, this->M_RecvInformation.M_Accelerate, M_BuffCount);
		this->AddBuffFunc(DeltaTime, this->M_RecvInformation.M_Burn, M_BuffCount);

		//更新颜色
		this->UpdateMouseColor();

		//buff个数为0则不在处理
		if (!M_BuffCount)
		{
			this->M_Proper_Condition.M_bBuffState = false;
		}
	}

	//当没有任何buff存在时->才可以更新被击中的颜色
	if (!this->M_Proper_Condition.M_bBuffState)
	{
		if (this->M_Proper_Condition.M_bHurtColorTime > 0.f)
		{
			this->M_Proper_Condition.M_bHurtColorTime -= DeltaTime;

			if (this->M_Proper_Condition.M_bHurtColorTime <= 0.f)
			{
				//更换颜色（恢复默认颜色）
				if (this->M_Proper_Condition.M_bColor)
				{
					this->M_Proper_Condition.M_bColor = false;

					this->SetFlipbookColor(FVector(1.f), this->GetMyActor()->GetSpriteColor().A);
				}
			}
		}
	}

	//是否受到攻击
	if (this->GetbIsHurt())
	{
		//当老鼠受到攻击时更新
		this->BeAttakedUpdate(DeltaTime);
	}
	else
	{
		//当老鼠未受到攻击时更新
		this->UnHitUpdate(DeltaTime);
	}


	/*
	//攻击状态
	if (this->GetBuffResult() && this->GetbIsAttack())
	{
		//开始攻击【持续攻击更新】
		this->AttakedUpdate(DeltaTime);

		//攻击卡片(如果当前没有卡片可以攻击)
		if (
			IsValid(this->GetCurrentAttackCard()) && this->GetCurrentAttackCard()->GetCurrentHP() <= 0
			||
			!IsValid(this->GetCurrentAttackCard())
			)
		{
			//取消攻击
			this->SetbIsAttack(false);
		}
	}
	*/
}

void AMouseActor::BeginPlay()
{
	Super::BeginPlay();

	//获取老鼠管理器
	this->M_MouseManager = AGameMapInstance::GetGameMapInstance()->M_MouseManagerComponent;
	if (!this->M_MouseManager)
	{
		this->SetActorTickEnabled(false);
	}

	//老鼠初始化
	this->MouseInit();
}

//移动(每0.02ms自动调用(允许移动的情况下))
void AMouseActor::MoveingUpdate(float DeltaTime)
{
}
//停止移动(每0.02ms自动调用(不能移动的情况下))
void AMouseActor::MoveingStop() {
	if (UFVMGameInstance::GetDebug())
		UGameSystemFunction::FVMLog(__FUNCTION__, TEXT("移动停止"));
}
//开始移动(当设置开始移动时调用一次)
void AMouseActor::MoveingBegin()
{
	if (UFVMGameInstance::GetDebug())
		UGameSystemFunction::FVMLog(__FUNCTION__, TEXT("开始移动"));
}
//老鼠开始攻击(如果设置了IsAttack为true 会自动调用一次)
void AMouseActor::AttackedBegin()
{
	if (UFVMGameInstance::GetDebug())
		UGameSystemFunction::FVMLog(__FUNCTION__, TEXT("开始攻击"));
}
void AMouseActor::AttackedEnd()
{
	if (UFVMGameInstance::GetDebug())
		UGameSystemFunction::FVMLog(__FUNCTION__, TEXT("攻击结束"));
}
//老鼠受到攻击((当设置isHurt为true时)调用一次)
void AMouseActor::BeAttakedBegin()
{
	if (UFVMGameInstance::GetDebug())
		UGameSystemFunction::FVMLog(__FUNCTION__, TEXT("受到攻击【开始】"));

	//更新颜色(当没有buff存在的时候更新)
	if (!this->M_Proper_Condition.M_bBuffState)
	{
		this->M_Proper_Condition.M_bColor = true;
		this->M_Proper_Condition.M_bHurtColorTime = 0.1f;
		this->SetFlipbookColor(FVector(0.5f), this->GetMyActor()->GetSpriteColor().A);
	}

}
//当老鼠被命中时受到的伤害数值(前提是isHurt为true  调用一次(一般由FlyitemActor命中时调用))
bool AMouseActor::BeHit(float _HurtValue)
{
	if (this->GetbIsHurt())
		return true;
	return false;
}
//老鼠没有受到攻击((当设置isHurt为false时)调用一次)
void AMouseActor::UnHit()
{
	if (UFVMGameInstance::GetDebug())
		UGameSystemFunction::FVMLog(__FUNCTION__, TEXT("受到攻击【结束】"));
}

//老鼠在攻击(如果设置了IsAttack 会自动调用->【每当攻击动画结束时调用】)
void AMouseActor::AttakedUpdate(float DeltaTime) {

}
//老鼠受到攻击(每0.02ms自动调用)
void AMouseActor::BeAttakedUpdate(float DeltaTime) {

}
//老鼠没有受到攻击(每0.02ms自动调用)
void AMouseActor::UnHitUpdate(float DeltaTime)
{
}
//当老鼠死亡时(当设置老鼠SetMouseDeath(true)时调用一次)【如果在此处调用了 Destroy()函数，将不会执行MouseKill(),当前老鼠死亡将不会记录到老鼠生成器的个数中 】
void AMouseActor::MouseDeathed()
{
	if (UFVMGameInstance::GetDebug())
		UGameSystemFunction::FVMLog(__FUNCTION__, TEXT("老鼠死亡预备，播放死亡动画"));

	this->RemoveBuff(this->M_RecvInformation.M_Freeze);
	this->RemoveBuff(this->M_RecvInformation.M_Solidification);

	//绑定死亡动画播放结束
	this->GetRenderComponent()->OnAnimationPlayEnd.Unbind();
	this->GetRenderComponent()->OnAnimationPlayEnd.BindUObject(this, &AMouseActor::OnMouseDeathedAnimPlayEnd);

	//设置老鼠播放死亡动画
	this->M_Proper_State.bPlayDeathAnim = true;
	//停止移动
	this->MoveStop();
	//释放当前的所有攻击目标
	this->SetCurrentAttackCard(nullptr);
	//取消攻击
	this->SetbIsAttack(false);
	//设置攻击力为0
	this->SetMouseATK(0.f);
}


void AMouseActor::SlowDownBuff(FMouseProperty_RecvImformation& _buff)
{
	if (UFVMGameInstance::GetDebug())
		UGameSystemFunction::FVMLog(__FUNCTION__, TEXT("触发:减速"));
}

void AMouseActor::SlowDownBuffOver(FMouseProperty_RecvImformation& _buff)
{
	if (UFVMGameInstance::GetDebug())
		UGameSystemFunction::FVMLog(__FUNCTION__, TEXT("触发:减速结束"));
}

void AMouseActor::AccelerateBuff(FMouseProperty_RecvImformation& _buff)
{
	if (UFVMGameInstance::GetDebug())
		UGameSystemFunction::FVMLog(__FUNCTION__, TEXT("触发:加速"));
}

void AMouseActor::AccelerateBuffOver(FMouseProperty_RecvImformation& _buff)
{
	if (UFVMGameInstance::GetDebug())
		UGameSystemFunction::FVMLog(__FUNCTION__, TEXT("触发:加速结束"));
}

void AMouseActor::SolidificationBuff(FMouseProperty_RecvImformation& _buff)
{
	if (UFVMGameInstance::GetDebug())
		UGameSystemFunction::FVMLog(__FUNCTION__, TEXT("触发:凝固"));
}

void AMouseActor::SolidificationBuffOver(FMouseProperty_RecvImformation& _buff)
{
	if (UFVMGameInstance::GetDebug())
		UGameSystemFunction::FVMLog(__FUNCTION__, TEXT("触发:凝固结束"));
}

void AMouseActor::FreezeBuff(FMouseProperty_RecvImformation& _buff)
{
	if (UFVMGameInstance::GetDebug())
		UGameSystemFunction::FVMLog(__FUNCTION__, TEXT("触发:冻结"));
}

void AMouseActor::FreezeBuffOver(FMouseProperty_RecvImformation& _buff)
{
	if (UFVMGameInstance::GetDebug())
		UGameSystemFunction::FVMLog(__FUNCTION__, TEXT("触发:冻结结束"));
}

void AMouseActor::BurnBuff(FMouseProperty_RecvImformation& _buff)
{
	if (UFVMGameInstance::GetDebug())
		UGameSystemFunction::FVMLog(__FUNCTION__, TEXT("触发:灼烧"));
}

void AMouseActor::BurnBuffOver(FMouseProperty_RecvImformation& _buff)
{
	if (UFVMGameInstance::GetDebug())
		UGameSystemFunction::FVMLog(__FUNCTION__, TEXT("触发:灼烧结束"));
}
