// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除


#include "GameStart/Flipbook/GameActor/Mouse/Boss/BossBase.h"

#include "GameSystem/GameMapStructManager.h"

#include "GameStart/VS/GameMapInstance.h"
#include "GameStart/VS/Components/MouseManagerComponent.h"

#include "GameStart/VS/UI/UI_MouseRound.h"

#include "GameSystem/FVMGameInstance.h"

ABossBase::ABossBase()
{
	
}

void ABossBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABossBase::BeginPlay()
{
	Super::BeginPlay();


}

void ABossBase::MouseInit()
{
	Super::MouseInit();

	//创建血条
	this->M_BossHP_Progress = CreateWidget<UUI_MouseBoss_Slider>(this->GetWorld(), LoadClass<UUI_MouseBoss_Slider>(0, TEXT("WidgetBlueprint'/Game/Resource/BP/GameStart/VS/UI_Player/MouseRound/UI_MouseBossSlider.UI_MouseBossSlider_C'")));
	if (this->M_BossHP_Progress)
	{
		if (AGameMapInstance::GetMouseManagerComponent_Static()->M_UUI_MouseRound)
			AGameMapInstance::GetMouseManagerComponent_Static()->M_UUI_MouseRound->AddMouseBossSlider(this->M_BossHP_Progress);

		this->M_BossHP_Progress->UpdateHPText(this->GetCurrentHP(), this->GetTotalHP());
	}

	//播放(Boss)BGM(如果没有任何血条才会播放BGM)
	if (AGameMapInstance::GetMouseManagerComponent_Static()->M_UUI_MouseRound && AGameMapInstance::GetMouseManagerComponent_Static()->M_UUI_MouseRound->GetMouseBossSliderCount() == 1)
	{
		if (AGameMapInstance::GetGameMapInstance()->GetGameMapStructManager())
			UFVMGameInstance::PlayBGM_Static(AGameMapInstance::GetGameMapInstance()->GetGameMapStructManager()->LevelConfig.LevelBossMusic);
	}
}

EMouseBaseType ABossBase::GetMouseBaseType()
{
	Super::GetMouseBaseType();

	return EMouseBaseType::Boss;
}

void ABossBase::MoveingUpdate(float DeltaTime)
{
	Super::MoveingUpdate(DeltaTime);

}

void ABossBase::MoveingStop()
{
	Super::MoveingStop();

}

void ABossBase::MoveingBegin()
{
	Super::MoveingBegin();

}

void ABossBase::AttackedBegin()
{
	Super::AttackedBegin();
}

void ABossBase::AttackedEnd()
{
	Super::AttackedEnd();

}

void ABossBase::AttakedUpdate(float DeltaTime)
{
	Super::AttakedUpdate(DeltaTime);

}

bool ABossBase::BeHit(float _HurtValue)
{
	if (!Super::BeHit(_HurtValue))
		return false;

	//更新血量
	this->UpdateHP(_HurtValue);

	//更新进度条
	this->M_BossHP_Progress->SetSliderRate(this->GetCurrentHP() / this->GetTotalHP());

	return true;
}

void ABossBase::BeAttakedBegin()
{
	Super::BeAttakedBegin();
}

void ABossBase::UnHit()
{
	Super::UnHit();


	//受到攻击播放动画
	//if (this->M_BossHP_Progress->IsAnimationPlaying(this->M_BossHP_Progress->GetWidgetTreeOwningClass()->Animations[1]))
	this->M_BossHP_Progress->PlayAnimation(this->M_BossHP_Progress->GetWidgetTreeOwningClass()->Animations[1]);

	this->M_BossHP_Progress->UpdateHPText(this->GetCurrentHP(), this->GetTotalHP());
}

void ABossBase::BeAttakedUpdate(float DeltaTime)
{
	Super::BeAttakedUpdate(DeltaTime);

}

void ABossBase::UnHitUpdate(float DeltaTime)
{
	Super::UnHitUpdate(DeltaTime);

}

void ABossBase::MouseDeathed()
{
	Super::MouseDeathed();

	//移除控件
	if (AGameMapInstance::GetMouseManagerComponent_Static()->M_UUI_MouseRound)
		AGameMapInstance::GetMouseManagerComponent_Static()->M_UUI_MouseRound->RemoveMouseBossSlider(this->M_BossHP_Progress);

	//播放BGM
	if (AGameMapInstance::GetMouseManagerComponent_Static()->M_UUI_MouseRound->GetMouseBossSliderCount() == 0)
	{
		if (AGameMapInstance::GetGameMapInstance()->GetGameMapStructManager())
			UFVMGameInstance::PlayBGM_Static(AGameMapInstance::GetGameMapInstance()->GetGameMapStructManager()->LevelConfig.LevelUpMusic);
	}
}

void ABossBase::SlowDownBuff(FMouseProperty_RecvImformation& _buff)
{
	Super::SlowDownBuff(_buff);

}

void ABossBase::SlowDownBuffOver(FMouseProperty_RecvImformation& _buff)
{
	Super::SlowDownBuffOver(_buff);
}

void ABossBase::AccelerateBuff(FMouseProperty_RecvImformation& _buff)
{
	Super::AccelerateBuff(_buff);
}

void ABossBase::AccelerateBuffOver(FMouseProperty_RecvImformation& _buff)
{
	Super::AccelerateBuffOver(_buff);
}

void ABossBase::SolidificationBuff(FMouseProperty_RecvImformation& _buff)
{
	Super::SolidificationBuff(_buff);

	//boss不受凝固buff的影响
	this->RemoveBuff(_buff.M_Solidification);
}

void ABossBase::SolidificationBuffOver(FMouseProperty_RecvImformation& _buff)
{
	Super::SolidificationBuffOver(_buff);
}

void ABossBase::FreezeBuff(FMouseProperty_RecvImformation& _buff)
{
	Super::FreezeBuff(_buff);

	//boss不受冻结buff的影响
	this->RemoveBuff(_buff.M_Freeze);
}

void ABossBase::FreezeBuffOver(FMouseProperty_RecvImformation& _buff)
{
	Super::FreezeBuffOver(_buff);
}

void ABossBase::BurnBuff(FMouseProperty_RecvImformation& _buff)
{
	Super::BurnBuff(_buff);
}

void ABossBase::BurnBuffOver(FMouseProperty_RecvImformation& _buff)
{
	Super::BurnBuffOver(_buff);
}

UMesheControllComponent* ABossBase::GetMesheControllComponent()
{
	return AGameMapInstance::GetGameMapInstance()->M_MesheControllComponent;
}

AMapLauncherBase* ABossBase::GetMapLauncher()
{
	if (this->GetMesheControllComponent())
		return this->GetMesheControllComponent()->GetMapLauncher();

	return nullptr;
}
