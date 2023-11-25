// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除


#include "GameStart/Flipbook/GameActor/Mouse/Boss/Boss_DongJun.h"
#include "GameSystem/Tools/GameSystemFunction.h"

//管道
#include "GameStart/Flipbook/GameActor/Mouse/Boss/DJ/DJ_Channel.h"

#include <Components/BoxComponent.h> 
#include <Kismet/KismetMathLibrary.h>

ABoss_DongJun::ABoss_DongJun()
{
	this->M_BoxMesh = CreateDefaultSubobject<UBoxComponent>(TEXT("Meshe_Box"));
	this->M_BoxBody = CreateDefaultSubobject<UBoxComponent>(TEXT("Body_Box"));

	this->M_BoxMesh->SetupAttachment(this->GetMyActor());
	this->M_BoxBody->SetupAttachment(this->M_BoxMesh);
}

void ABoss_DongJun::MouseDeathed()
{
	Super::MouseDeathed();

	this->M_BoxBody->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	this->M_BoxMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	//设置死亡动画
	this->SetPlayAnimation(LoadObject<UPaperFlipbook>(0,
		TEXT("PaperFlipbook'/Game/Resource/Texture/Sprite/VS/Sprite/Mouse/Boss/0/Anim/F_M_B_0_Death.F_M_B_0_Death'"))
	);

	//移除所有管道的父对象
	for (auto& Channel : this->GetCurrentLinkChannels())
	{
		if (IsValid(Channel))
		{
			Channel->SetMouseDongJun(nullptr);
			Channel->MesheReset();
			Channel->MouseDeathed();
		}
	}

	this->M_bDeath = true;
}

void ABoss_DongJun::SlowDownBuff(FMouseProperty_RecvImformation& _buff)
{
	Super::SlowDownBuff(_buff);

	this->M_TickRateSpeed = 0.5f;

	if (_buff.M_Accelerate.M_CurrentTime > 0.f)
	{
		this->M_TickRateSpeed = 1.f;
	}
}

void ABoss_DongJun::SlowDownBuffOver(FMouseProperty_RecvImformation& _buff)
{
	Super::SlowDownBuffOver(_buff);

	this->M_TickRateSpeed = 1.0f;

	if (_buff.M_Accelerate.M_CurrentTime > 0.f)
	{
		this->M_TickRateSpeed = 1.5f;
	}
}

void ABoss_DongJun::AccelerateBuff(FMouseProperty_RecvImformation& _buff)
{
	Super::AccelerateBuff(_buff);

	this->M_TickRateSpeed = 1.5f;

	if (_buff.M_SlowDown.M_CurrentTime > 0.f)
	{
		this->M_TickRateSpeed = 1.5f;
	}
}

void ABoss_DongJun::AccelerateBuffOver(FMouseProperty_RecvImformation& _buff)
{
	Super::AccelerateBuffOver(_buff);

	this->M_TickRateSpeed = 1.0f;

	if (_buff.M_SlowDown.M_CurrentTime > 0.f)
	{
		this->M_TickRateSpeed = 0.5f;
	}
}

void ABoss_DongJun::SolidificationBuff(FMouseProperty_RecvImformation& _buff)
{
	Super::SolidificationBuff(_buff);
}

void ABoss_DongJun::SolidificationBuffOver(FMouseProperty_RecvImformation& _buff)
{
	Super::SolidificationBuffOver(_buff);
}

void ABoss_DongJun::FreezeBuff(FMouseProperty_RecvImformation& _buff)
{
	Super::FreezeBuff(_buff);
}

void ABoss_DongJun::FreezeBuffOver(FMouseProperty_RecvImformation& _buff)
{
	Super::FreezeBuffOver(_buff);
}

void ABoss_DongJun::BurnBuff(FMouseProperty_RecvImformation& _buff)
{
	Super::BurnBuff(_buff);
}

void ABoss_DongJun::BurnBuffOver(FMouseProperty_RecvImformation& _buff)
{
	Super::BurnBuffOver(_buff);
}

void ABoss_DongJun::TimelineMoveUpdate()
{
	if (this->M_Curve)
	{
		float value = this->M_Curve->GetFloatValue(this->M_JumpTimeLine.GetPlaybackPosition());

		this->SetActorLocation(UKismetMathLibrary::VLerp(this->M_CurrentLocation, this->M_TargetLocation, value));
	}
}

void ABoss_DongJun::TimelineFinish()
{
	if (this->M_Curve)
	{
		//创建球体碰撞
		TArray<FHitResult> LCards;
		if (UGameSystemFunction::AddSphereTraceMulti(this,
			LCards,
			FVector(0.f),
			FVector(0.f, 0.f, 5.f),
			15.f,
			UGameSystemFunction::GetCardCollisionTraceType(ECardCollisionType::E_CardActor))
			)
		{
			for (auto& Card : LCards)
			{
				if (Cast<ACardActor>(Card.GetActor()) && !Cast<ACardActor>(Card.GetActor())->IsPendingKillPending())
				{
					Cast<ACardActor>(Card.GetActor())->KillCard();
					//卡片损坏
					UResourceManagerComponent::ResourceAddBadCard();
				}
			}
		}
	}
}

void ABoss_DongJun::OverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
}

void ABoss_DongJun::OverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
}

void ABoss_DongJun::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (FVM_VS_GamePause())
	{
		return;
	}

	if (this->M_bDeath)
	{
		return;
	}

	float LDeltaTime = DeltaTime * this->M_TickRateSpeed;

	this->M_JumpTimeLine.TickTimeline(LDeltaTime);


	switch (this->GetMouseCurrentState())
	{
	case EDongJunState::Def:this->DefState(LDeltaTime);
		break;
	case EDongJunState::Out:this->OutState(LDeltaTime);
		break;
	case EDongJunState::Build:this->BuildState(LDeltaTime);
		break;
	case EDongJunState::In:this->InState(LDeltaTime);
		break;
	case EDongJunState::Hid:this->HiddenState(LDeltaTime);
		break;
	case EDongJunState::Jump:this->JumpState(LDeltaTime);
		break;
	case EDongJunState::JumpPause:this->JumpPauseState(LDeltaTime);
		break;
	default:UE_LOG(LogTemp, Error, TEXT("Tick Def"));
		break;
	}

}

void ABoss_DongJun::AnimEnd()
{
	switch (this->GetMouseCurrentState())
	{
	case EDongJunState::Def:this->DefStateByAnimEnd();
		break;
	case EDongJunState::Out:this->OutStateByAnimEnd();
		break;
	case EDongJunState::Build:this->BuildStateByAnimEnd();
		break;
	case EDongJunState::In:this->InStateByAnimEnd();
		break;
	case EDongJunState::Hid:this->HiddenStateByAnimEnd();
		break;
	case EDongJunState::Jump:this->JumpStateByAnimEnd();
		break;
	case EDongJunState::JumpPause:this->JumpPauseStateByAnimEnd();
		break;
	default:UE_LOG(LogTemp, Error, TEXT("Tick Def"));
		break;
	}
}

void ABoss_DongJun::DefState(float DeltaTime)
{
	this->M_Time += DeltaTime;
}

void ABoss_DongJun::DefStateByAnimEnd()
{
	if (this->M_Time > 3.f)
	{
		this->M_Time = 0.f;

		//跳跃技能
		auto Jump = [&]() {
			this->SetMouseCurrentState(EDongJunState::Jump);

			if (this->GetCurrentHP() > this->GetTotalHP() * 0.4f)
			{
				this->SetPlayAnimation(this->M_MouseJump_res);
			}
			else
			{
				this->SetPlayAnimation(this->M_MouseLowJump_res);
			}

			//设置列
			this->SetMouseLine(FLine(this->GetMouseLine().Row, this->GetMouseLine().Col - 1));

			//获取位置
			this->M_CurrentLocation = this->GetActorLocation();

			//获取目标位置
			this->M_TargetLocation = this->GetMesheControllComponent()->GetMapMesh(
				this->GetMouseLine().Row, this->GetMouseLine().Col
			)->GetActorLocation();

			//播放时间线
			this->M_JumpTimeLine.PlayFromStart();
		};

		//如果没有打洞则打洞
		if (!this->M_bBuild)
		{
			this->SetMouseCurrentState(EDongJunState::Build);

			if (this->GetCurrentHP() > this->GetTotalHP() * 0.4f)
			{
				this->SetPlayAnimation(this->M_MouseBuild_res);
			}
			else
			{
				this->SetPlayAnimation(this->M_MouseLowBuild_res);
			}

			this->M_bBuild = true;
		}
		else {

			Jump();

			this->M_bBuild = false;
		}
	}
}

void ABoss_DongJun::OutState(float DeltaTime)
{

}

void ABoss_DongJun::OutStateByAnimEnd()
{
	this->M_Time = 0.f;
	this->SetMouseCurrentState(EDongJunState::Def);

	if (this->GetCurrentHP() > (this->GetTotalHP() * 0.4f))
	{
		this->SetPlayAnimation(this->M_MouseDef_res);
	}
	else
	{
		this->SetPlayAnimation(this->M_MouseLowDef_res);
	}

	//设置老鼠的行列
	this->SetMouseLine(FLine(this->GetMouseLine().Row,
		this->GetMesheControllComponent()->GetMapMeshRowAndCol().Col - 1)
	);
}

void ABoss_DongJun::BuildState(float DeltaTime)
{
	this->M_Time += DeltaTime;
}

void ABoss_DongJun::BuildStateByAnimEnd()
{

	if (this->M_Time > 3.f)
	{
		this->M_Time = 0.f;
		this->SetMouseCurrentState(EDongJunState::In);

		if (this->GetCurrentHP() > this->GetTotalHP() * 0.4f)
		{
			this->SetPlayAnimation(this->M_MouseIn_res);
		}
		else
		{
			this->SetPlayAnimation(this->M_MouseLowIn_res);
		}

		//判断当前是否有管道
		if (this->bChannelCreate())
		{
			//创建一个管道
			this->CreateChannel();
		}
		else {
			//维修管道
			for (auto& Channel : this->M_CurrentLinkChannels)
			{
				if (Channel && !Channel->IsPendingKillPending())
				{
					if (
						Channel->GetChannel_In().Row == this->GetMouseLine().Row
						)
					{
						Channel->HPReset();
						break;
					}
				}
			}
		}

	}
}

void ABoss_DongJun::InState(float DeltaTime)
{

}

void ABoss_DongJun::InStateByAnimEnd()
{

	this->M_Time = 0.f;
	this->SetMouseCurrentState(EDongJunState::Hid);

	this->SetActorHiddenInGame(true);

	this->M_BoxBody->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	this->M_BoxMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

}

void ABoss_DongJun::HiddenState(float DeltaTime)
{
	this->M_Time += DeltaTime;
	if (this->M_Time > 5.f)
	{
		this->M_Time = 0.f;
		this->SetMouseCurrentState(EDongJunState::Out);

		//设置老鼠的行列
		this->SetMouseLine(FLine(UGameSystemFunction::GetRandomRange(0,
			this->GetMesheControllComponent()->GetMapMeshRowAndCol().Row - 1),
			this->GetMesheControllComponent()->GetMapMeshRowAndCol().Col - 1));

		//根据是否打洞决定位置
		if (!this->M_bBuild)
		{
			if (this->bChannelCreate())
			{
				this->SetActorLocation(
					this->GetMesheControllComponent()->GetMapMeshLocation(this->GetMouseLine().Row,
						this->GetMouseLine().Col)
				);
			}
			else {
				//设置老鼠的位置
				this->SetActorLocation(
					this->GetMesheControllComponent()->GetMapMeshLocation(this->GetMouseLine().Row,
						this->GetMouseLine().Col)
					+ FVector(-0.5f, -this->M_OffsetPosition.X, 0.f)
				);
			}
		}
		else {
			//设置老鼠的位置
			this->SetActorLocation(
				this->GetMesheControllComponent()->GetMapMeshLocation(this->GetMouseLine().Row,
					this->GetMouseLine().Col) +
				FVector(-0.5f, -this->M_OffsetPosition.X, 0.f));
		}

		if (this->GetCurrentHP() > this->GetTotalHP() * 0.4f)
		{
			this->SetPlayAnimation(this->M_MouseOut_res);
		}
		else
		{
			this->SetPlayAnimation(this->M_MouseLowOut_res);
		}


		this->SetActorHiddenInGame(false);

		this->M_BoxMesh->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		this->M_BoxBody->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	}

}

void ABoss_DongJun::HiddenStateByAnimEnd()
{

}

void ABoss_DongJun::JumpState(float DeltaTime)
{

}

void ABoss_DongJun::JumpStateByAnimEnd()
{

	this->M_Time = 0.f;
	this->SetMouseCurrentState(EDongJunState::JumpPause);

	if (this->GetCurrentHP() > this->GetTotalHP() * 0.4f)
	{
		this->SetPlayAnimation(this->M_MouseBuild_res);
	}
	else
	{
		this->SetPlayAnimation(this->M_MouseLowBuild_res);
	}

}

void ABoss_DongJun::JumpPauseState(float DeltaTime)
{

	this->M_Time += DeltaTime;

	if (this->M_Time > 1.f)
	{
		//每次跳跃完成之后判断前方网格（然后决定是否进入下一个动作）
		this->M_CurrentJumpCount--;

		if (this->M_CurrentJumpCount <= 0)
		{
			this->M_CurrentJumpCount = this->M_JumpCount;

			this->M_Time = 0.f;

			this->SetMouseCurrentState(EDongJunState::In);

			if (this->GetCurrentHP() > this->GetTotalHP() * 0.4f)
			{
				this->SetPlayAnimation(this->M_MouseIn_res);
			}
			else
			{
				this->SetPlayAnimation(this->M_MouseLowIn_res);
			}

		}
		else {

			this->M_Time = 0.f;

			this->SetMouseCurrentState(EDongJunState::Jump);

			if (this->GetCurrentHP() > this->GetTotalHP() * 0.4f)
			{
				this->SetPlayAnimation(this->M_MouseJump_res);
			}
			else
			{
				this->SetPlayAnimation(this->M_MouseLowJump_res);
			}


			this->SetMouseLine(FLine(this->GetMouseLine().Row, this->GetMouseLine().Col - 2));

			//获取位置
			this->M_CurrentLocation = this->GetActorLocation();

			//获取目标位置
			this->M_TargetLocation = this->GetMesheControllComponent()->GetMapMesh(
				this->GetMouseLine().Row, this->GetMouseLine().Col)->GetActorLocation();

			this->M_JumpTimeLine.PlayFromStart();
		}
	}
}

void ABoss_DongJun::JumpPauseStateByAnimEnd()
{

}

void ABoss_DongJun::CreateChannel()
{
	//创建管道
	if (this->bChannelCreate())
	{
		FTransform Trans = this->GetTransform();
		Trans.SetLocation(Trans.GetLocation() + FVector(100.f, 0.f, 0.f));

		ADJ_Channel* Channel = this->GetWorld()->SpawnActor<ADJ_Channel>(LoadClass<ADJ_Channel>(0, TEXT("Blueprint'/Game/Resource/BP/GameStart/Item/Mouse/boss/dongjun/MyDJ_Channel.MyDJ_Channel_C'")));
		//设置父对象
		Channel->SetMouseDongJun(this);
		//初始化旋转
		Channel->InitRotation();
		//设置初始位置
		Channel->SetActorLocation(Trans.GetLocation());
		//设置层级
		Channel->SetTranslucentSortPriority(this->GetRenderComponent()->GetTranslucency());
		//链接管道
		if (Channel->LinkChannel(this->GetMouseLine()))
		{
			if (this->M_CurrentChannel)
			{
				FLine NewLine = this->M_CurrentChannel->GetChannel_Out();
				UGameSystemFunction::FVMLog(__FUNCTION__, FString(TEXT("新管道：") + FString::FromInt(NewLine.Row) + TEXT(":") + FString::FromInt(NewLine.Col)));
			}

		}
		else {
			Channel->Destroy();

			UGameSystemFunction::FVMLog(__FUNCTION__, FString(TEXT("无效管道")));
		}
	}
	else {
		UGameSystemFunction::FVMLog(__FUNCTION__, FString(TEXT("当前行有管道连接")));
	}
}

bool ABoss_DongJun::bChannelCreate()
{
	bool BResult = true;

	for (auto& Channel : this->M_CurrentLinkCol)
	{
		if (Channel->GetLine().Row == this->GetMouseLine().Row)
		{
			BResult = false;
		}
	}

	//如果结果是true则继续判断
	if (BResult)
		for (int32 i = 0; i < this->GetMesheControllComponent()->GetMapMeshRowAndCol().Row; i++)
		{
			AMapMeshe* Mesh = this->GetMesheControllComponent()->GetMapMesh(i, this->GetMesheControllComponent()->GetMapMeshRowAndCol().Col - 1);

			if (Mesh->GetLine().Row == this->GetMouseLine().Row)
			{
				if (Mesh->GetLineType() == ELineType::Channel)
				{
					BResult = false;
				}
			}
		}

	return BResult;
}

void ABoss_DongJun::SetCurrentChannel(ADJ_Channel* _Channel)
{
	this->M_CurrentChannel = _Channel;
}

TArray<AMapMeshe*>& ABoss_DongJun::GetCurrentLinkCol()
{
	return this->M_CurrentLinkCol;
}

TArray<class ADJ_Channel*>& ABoss_DongJun::GetCurrentLinkChannels()
{
	return this->M_CurrentLinkChannels;
}

void ABoss_DongJun::SetMouseCurrentState(const EDongJunState& State)
{
	this->M_CurrentState = State;
}

const EDongJunState& ABoss_DongJun::GetMouseCurrentState() const
{
	return this->M_CurrentState;
}

void ABoss_DongJun::BeginPlay()
{
	//设置时间轴
	this->M_FloatStatic_Delegate.BindUFunction(this, TEXT("TimelineMoveUpdate"));
	this->M_TimelineFinish_Delegate.BindUFunction(this, TEXT("TimelineFinish"));
	this->M_JumpTimeLine.AddInterpFloat(this->M_Curve, this->M_FloatStatic_Delegate);
	this->M_JumpTimeLine.SetTimelineFinishedFunc(this->M_TimelineFinish_Delegate);
	//绑定
	this->M_BoxBody->OnComponentEndOverlap.AddDynamic(this, &ABoss_DongJun::OverlapEnd);
	this->M_BoxBody->OnComponentBeginOverlap.AddDynamic(this, &ABoss_DongJun::OverlapBegin);

	Super::BeginPlay();
}

void ABoss_DongJun::MouseInit()
{
	Super::MouseInit();

	this->GetRenderComponent()->OnAnimationPlayEnd.BindUObject(this, &ABoss_DongJun::AnimEnd);

	this->M_MouseDef_res = Cast<UPaperFlipbook>(this->M_MouseDef.TryLoad());
	this->M_MouseLowDef_res = Cast<UPaperFlipbook>(this->M_MouseLowDef.TryLoad());

	this->M_MouseOut_res = Cast<UPaperFlipbook>(this->M_MouseOut.TryLoad());
	this->M_MouseLowOut_res = Cast<UPaperFlipbook>(this->M_MouseLowOut.TryLoad());

	this->M_MouseBuild_res = Cast<UPaperFlipbook>(this->M_MouseBuild.TryLoad());
	this->M_MouseLowBuild_res = Cast<UPaperFlipbook>(this->M_MouseLowBuild.TryLoad());

	this->M_MouseIn_res = Cast<UPaperFlipbook>(this->M_MouseIn.TryLoad());
	this->M_MouseLowIn_res = Cast<UPaperFlipbook>(this->M_MouseLowIn.TryLoad());

	this->M_MouseJump_res = Cast<UPaperFlipbook>(this->M_MouseJump.TryLoad());
	this->M_MouseLowJump_res = Cast<UPaperFlipbook>(this->M_MouseLowJump.TryLoad());

	//获取网格跳跃次数
	for (int32 i = 1; i <= this->GetMesheControllComponent()->GetMapMeshRowAndCol().Col; i++)
	{
		if (i % 2 == 0)
		{
			M_JumpCount += 1;
		}
	}

	//次数限制
	if (this->M_JumpCount > this->M_JumpCountMax)
	{
		this->M_JumpCount = this->M_JumpCountMax;
	}

	this->M_CurrentJumpCount = this->M_JumpCount;

	this->SetPlayAnimation(this->M_MouseOut_res);

	//初始化状态
	this->SetMouseCurrentState(EDongJunState::Out);

	this->SetActorLocation(
		this->GetMesheControllComponent()->GetMapMeshLocation(
			this->GetMouseLine().Row,
			this->GetMouseLine().Col)
	);

	this->M_OffsetPosition = FVector(-80.f,0.f,0.f);
}

void ABoss_DongJun::MoveingUpdate(float DeltaTime)
{
	Super::MoveingUpdate(DeltaTime);

}

void ABoss_DongJun::MoveingStop()
{
	Super::MoveingStop();

}

void ABoss_DongJun::MoveingBegin()
{
	Super::MoveingBegin();

}

void ABoss_DongJun::AttackedBegin()
{
	Super::AttackedBegin();

}

void ABoss_DongJun::AttackedEnd()
{
	Super::AttackedEnd();

}

void ABoss_DongJun::AttakedUpdate(float DeltaTime)
{
	Super::AttakedUpdate(DeltaTime);


}

void ABoss_DongJun::BeAttakedBegin()
{
	Super::BeAttakedBegin();



}
