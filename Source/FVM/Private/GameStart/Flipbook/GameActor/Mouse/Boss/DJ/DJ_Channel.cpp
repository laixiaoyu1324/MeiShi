// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除


#include "GameStart/Flipbook/GameActor/Mouse/Boss/DJ/DJ_Channel.h"
#include "GameStart/Flipbook/GameActor/Mouse/Boss/Boss_DongJun.h"
#include "GameSystem/Tools/GameSystemFunction.h"

FLine ADJ_Channel::GetChannel_In()
{
	return this->M_TargetLine;
}

FLine ADJ_Channel::GetChannel_Out()
{
	return this->M_CurrentLine;
}

ADJ_Channel::ADJ_Channel()
{
	this->M_MouseTypeBox = CreateDefaultSubobject<UBoxComponent>(TEXT("DJ_Mehse_Type"));
	this->M_MouseBodyBox = CreateDefaultSubobject<UBoxComponent>(TEXT("DJ_Channel_Body"));

	this->M_MouseTypeBox->SetupAttachment(this->M_MousePosition);
	this->M_MouseBodyBox->SetupAttachment(this->M_MouseTypeBox);
}

void ADJ_Channel::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//表示结果动画演示
	if (this->M_DongJun_Channel_State == EDongJun_Channel_State::End)
	{
		//创建范围检测->用于实现传送
		TArray<FHitResult> THis;
		if (UGameSystemFunction::AddSphereTraceMulti(this->M_Channel_In, THis, FVector::ZeroVector, FVector::ZeroVector, 15.f, ETraceTypeQuery::TraceTypeQuery4))
		{
			for (auto& Mouse : THis)
			{
				if (AMouseActor* MouseObj = Cast<AMouseActor>(Mouse.GetActor()))
				{
					//不能是boss类型的老鼠
					if (MouseObj->GetMouseBaseType() != EMouseBaseType::Boss
						&&
						MouseObj->GetMouseBaseType() != EMouseBaseType::Other
						)
					{
						MouseObj->SetMouseLine(this->GetChannel_Out());
						MouseObj->SetActorLocation(this->GetActorLocation() + FVector(0.f, 90.f, -15.f));

						AMapMeshe* CurrentMeshe = this->M_Owner->GetMesheControllComponent()->GetMapMesh(
							this->GetChannel_Out().Row, this->GetChannel_Out().Col
						);

						MouseObj->SetTranslucentSortPriority(CurrentMeshe->GetTranslucency() + 19);
					}
				}
			}
		}
	}

	if (this->M_DongJun_Channel_State == EDongJun_Channel_State::Init)
	{
		if (IsValid(this->M_Owner))
		{
			if (this->M_Owner->GetMouseCurrentState() == EDongJunState::Hid)
			{
				//设置管道出口位置
				//判断当前地形
				if (this->M_Channel_MesheType_In == ELineType::OnWater)
				{
					this->GetMyActor()->SetPlayAnimation(
						LoadObject<UPaperFlipbook>(0,
							TEXT("PaperFlipbook'/Game/Resource/Texture/Sprite/VS/Sprite/Mouse/Boss/0/Anim/F_M_B_0_Water.F_M_B_0_Water'")
							));
				}
				else {
					this->GetMyActor()->SetPlayAnimation(
						LoadObject<UPaperFlipbook>(0,
							TEXT("PaperFlipbook'/Game/Resource/Texture/Sprite/VS/Sprite/Mouse/Boss/0/Anim/F_M_B_0_Ground.F_M_B_0_Ground'")
							));
				}

				//设置位置
				this->SetActorLocation(
					this->M_Owner->GetMesheControllComponent()->GetMapMesh(
						this->GetChannel_Out().Row, this->GetChannel_Out().Col
					)->GetActorLocation());

				//设置状态
				this->M_DongJun_Channel_State = EDongJun_Channel_State::Finish;

				//创建一个卡片区域检测->销毁卡片
				//创建一个检测->将检测到的卡片全部销毁
				TArray<FHitResult> Hits;
				if (
					UGameSystemFunction::AddSphereTraceMulti(this,
						Hits,
						FVector::ZeroVector,
						FVector::ZeroVector,
						15.f,
						ETraceTypeQuery::TraceTypeQuery3)
					)
				{
					for (auto& CardActor : Hits)
					{
						if (Cast<ACardActor>(CardActor.GetActor()))
						{
							Cast<ACardActor>(CardActor.GetActor())->KillCard();
						}
					}
				}
			}
		}
	}
}

void ADJ_Channel::BeginPlay()
{
	Super::BeginPlay();

	this->SetMouseCollisionType(this->M_MouseTypeBox, this->M_MouseBodyBox, ECollisionChannel::ECC_GameTraceChannel2);

	this->GetRenderComponent()->OnAnimationPlayEnd.BindUObject(this, &ADJ_Channel::OnAnimationPlayEnd);
}

void ADJ_Channel::OnAnimationPlayEnd()
{
	if (this->M_DongJun_Channel_State == EDongJun_Channel_State::None)
	{
		if (IsValid(this->M_Channel_In))
		{
			//判断当前地形
			if (this->M_Channel_MesheType_In == ELineType::OnWater)
			{
				this->M_Channel_In->GetMyActor()->SetPlayAnimation(
					LoadObject<UPaperFlipbook>(0,
						TEXT("PaperFlipbook'/Game/Resource/Texture/Sprite/VS/Sprite/Mouse/Boss/0/Anim/F_M_B_0_Water_2.F_M_B_0_Water_2'")
						));
			}
			else {
				this->M_Channel_In->GetMyActor()->SetPlayAnimation(
					LoadObject<UPaperFlipbook>(0,
						TEXT("PaperFlipbook'/Game/Resource/Texture/Sprite/VS/Sprite/Mouse/Boss/0/Anim/F_M_B_0_Ground_2.F_M_B_0_Ground_2'")
						));
			}

			this->M_DongJun_Channel_State = EDongJun_Channel_State::Init;
		}
	}
	else if (this->M_DongJun_Channel_State == EDongJun_Channel_State::Finish)
	{
		//设置管道出口位置
		//判断当前地形
		if (this->M_Channel_MesheType_In == ELineType::OnWater)
		{
			this->GetMyActor()->SetPlayAnimation(
				LoadObject<UPaperFlipbook>(0,
					TEXT("PaperFlipbook'/Game/Resource/Texture/Sprite/VS/Sprite/Mouse/Boss/0/Anim/F_M_B_0_Water_Def.F_M_B_0_Water_Def'")
					));
		}
		else {
			this->GetMyActor()->SetPlayAnimation(
				LoadObject<UPaperFlipbook>(0,
					TEXT("PaperFlipbook'/Game/Resource/Texture/Sprite/VS/Sprite/Mouse/Boss/0/Anim/F_M_B_0_Ground_Def.F_M_B_0_Ground_Def'")
					));
		}

		this->M_DongJun_Channel_State = EDongJun_Channel_State::End;
	}
}

void ADJ_Channel::SetMouseDongJun(ABoss_DongJun* _Owner)
{
	this->M_Owner = _Owner;
}

bool ADJ_Channel::LinkChannel(FLine _CurrentLine)
{
	//获取当前管道的地形->根据地形决定目标地形
	AMapMeshe* CurrentMeshe = this->M_Owner->GetMesheControllComponent()->GetMapMesh(_CurrentLine.Row, _CurrentLine.Col);
	//当前网格地形
	ELineType CurrentMesheType = CurrentMeshe->GetLineType();
	//设置当前网格的新地形为（管道）
	CurrentMeshe->SetLineType(ELineType::Channel);
	//设置入口类型
	this->M_Channel_MesheType_In = CurrentMesheType;


	//添加所有匹配的地形//

	//需要排除的列项
	TArray<int32> NotCols = { 0,1,2 };
	//获取最大列个数
	int32 MaxCols = this->M_Owner->GetMesheControllComponent()->GetMapMeshRowAndCol().Col;
	//排除最后一列
	NotCols.Emplace(MaxCols - 1);
	//获取最大行
	int32 MaxRows = this->M_Owner->GetMesheControllComponent()->GetMapMeshRowAndCol().Row;
	//所有匹配的地形集合
	TArray<AMapMeshe*> AllMatchMesheType;
	//寻找匹配的地形
	for (int32 Row = 0; Row < MaxRows; Row++)
	{
		for (int32 Col = 0; Col < MaxCols; Col++)
		{
			bool BResult = true;
			//判断当前列是否是排除列
			for (int32 Col_Index = 0; Col_Index < NotCols.Num(); Col_Index++)
			{
				if (Col == NotCols[Col_Index])
					BResult = false;
			}


			//首先当前地形不属于排除地形
			if (BResult) {
				//判断当前非排除地形是否匹配需求地形(如果匹配则添加地形)
				if (CurrentMesheType == this->M_Owner->GetMesheControllComponent()->GetMapMesh(Row, Col)->GetLineType())
				{
					AllMatchMesheType.Emplace(this->M_Owner->GetMesheControllComponent()->GetMapMesh(Row, Col));
				}
			}
		}
	}

	//判断是否有地形可用
	if (AllMatchMesheType.Num())
	{
		//随机获取一个地形当作出口
		int32 Mesh_Index = UGameSystemFunction::GetRandomRange(0, AllMatchMesheType.Num() - 1);
		AMapMeshe* TargetMeshe = AllMatchMesheType[Mesh_Index];
		//设置原来的地形
		this->M_Channel_MesheType_Out = TargetMeshe->GetLineType();
		//设置新的地形
		TargetMeshe->SetLineType(ELineType::Channel);
		//设置行列
		this->M_CurrentLine = TargetMeshe->GetLine();
		this->M_TargetLine = _CurrentLine;
		//应用
		this->Apply();

		return true;
	}

	return false;
}

void ADJ_Channel::SetLinkCol(AMapMeshe* _AMapMeshe)
{
	this->M_AMapMeshe = _AMapMeshe;
}

void ADJ_Channel::Apply()
{
	//将当前管道添加给洞君并且设置当前使用的管道
	this->M_Owner->GetCurrentLinkChannels().Emplace(this);
	this->M_Owner->SetCurrentChannel(this);
	//添加地形
	this->M_Owner->GetCurrentLinkCol().Emplace(
		this->M_Owner->GetMesheControllComponent()->GetMapMesh(this->GetChannel_In().Row,
			this->GetChannel_In().Col));
	this->SetLinkCol(this->M_Owner->GetMesheControllComponent()->GetMapMesh(this->GetChannel_In().Row,
		this->GetChannel_In().Col));
	//设置管道默认动画(因为是刚刚创建的管道->使用管道默认动画)


	//根据地形创建管道动画的形态(陆地<->水路)


	//设置地形类型
	//创建对应的连接入口
	this->M_Channel_In = this->GetWorld()->SpawnActor<AGameActorFlipbookBase>(LoadClass<AGameActorFlipbookBase>(0,
		TEXT("Blueprint'/Game/Resource/BP/GameStart/Item/Mouse/boss/dongjun/DJ_Channel_In.DJ_Channel_In_C'")));

	this->M_Channel_In->InitRotation();

	this->M_Channel_In->SetTranslucentSortPriority(
		this->M_Owner->GetMesheControllComponent()->
		GetMapMesh(this->GetChannel_In().Row, this->GetChannel_In().Col)->
		GetTranslucency() + 19
	);

	//判断当前地形
	if (this->M_Channel_MesheType_In == ELineType::OnWater)
	{
		this->M_Channel_In->GetMyActor()->SetPlayAnimation(LoadObject<UPaperFlipbook>(0,
			TEXT("PaperFlipbook'/Game/Resource/Texture/Sprite/VS/Sprite/Mouse/Boss/0/Anim/F_M_B_0_Water_1.F_M_B_0_Water_1'")));
	}

	//设置位置
	this->M_Channel_In->SetActorLocation(
		this->M_Owner->GetMesheControllComponent()->GetMapMesh(this->GetChannel_In().Row,
			this->GetChannel_In().Col)->GetActorLocation() + FVector(0.f, 0.f, 0.f));

	//设置行列
	this->SetMouseLine(this->GetChannel_Out());

	//设置老鼠线路类型
	this->SetMouseLineType(ELineType::All);

	//添加到老鼠管理器
	UMouseManagerComponent::AddMouse(this, this->M_CurrentLine.Row);

	//创建一个检测->将检测到的卡片全部销毁
	TArray<FHitResult> Hits;
	if (UGameSystemFunction::AddSphereTraceMulti(this->M_Channel_In, Hits, FVector::ZeroVector, FVector::ZeroVector, 15.f, ETraceTypeQuery::TraceTypeQuery3))
	{
		for (auto& CardActor : Hits)
		{
			if (Cast<ACardActor>(CardActor.GetActor()))
			{
				Cast<ACardActor>(CardActor.GetActor())->KillCard();
				UResourceManagerComponent::ResourceAddBadCard();
			}
		}
	}
}

void ADJ_Channel::HPReset()
{
	this->SetHP(this->GetTotalHP());

	if (this->M_Channel_MesheType_In == ELineType::OnWater)
	{
		this->GetMyActor()->SetPlayAnimation(LoadObject<UPaperFlipbook>(0, TEXT("PaperFlipbook'/Game/Resource/Texture/Sprite/VS/Sprite/Mouse/Boss/0/Anim/F_M_B_0_Water_Def.F_M_B_0_Water_Def'")));
	}
	else {
		this->GetMyActor()->SetPlayAnimation(LoadObject<UPaperFlipbook>(0, TEXT("PaperFlipbook'/Game/Resource/Texture/Sprite/VS/Sprite/Mouse/Boss/0/Anim/F_M_B_0_Ground_Def.F_M_B_0_Ground_Def'")));
	}
}

void ADJ_Channel::MesheReset()
{
	//还原通道地形
	AGameMapInstance::GetGameMapInstance()->M_MesheControllComponent->GetMapMesh(this->GetChannel_In().Row, this->GetChannel_In().Col)->SetLineType(this->M_Channel_MesheType_In);
	AGameMapInstance::GetGameMapInstance()->M_MesheControllComponent->GetMapMesh(this->GetChannel_Out().Row, this->GetChannel_Out().Col)->SetLineType(this->M_Channel_MesheType_Out);
}

bool ADJ_Channel::BeHit(float _HurtValue)
{
	if (!Super::BeHit(_HurtValue))
		return false;

	this->UpdateHP(_HurtValue);

	//当前血量的40%设置状态
	if (this->GetCurrentHP() < this->GetTotalHP() * 0.4f)
	{
		this->M_DongJun_Channel_State = EDongJun_Channel_State::End;

		if (this->M_Channel_MesheType_In == ELineType::OnWater)
		{
			this->GetMyActor()->SetPlayAnimation(LoadObject<UPaperFlipbook>(0, TEXT("PaperFlipbook'/Game/Resource/Texture/Sprite/VS/Sprite/Mouse/Boss/0/Anim/F_M_B_0_Water_Low.F_M_B_0_Water_Low'")));
		}
		else {
			this->GetMyActor()->SetPlayAnimation(LoadObject<UPaperFlipbook>(0, TEXT("PaperFlipbook'/Game/Resource/Texture/Sprite/VS/Sprite/Mouse/Boss/0/Anim/F_M_B_0_Ground_Low.F_M_B_0_Ground_Low'")));
		}

	}

	return true;
}

void ADJ_Channel::MouseDeathed()
{
	Super::MouseDeathed();

	//销毁对象->移除管道
	if (this->M_Owner && !this->M_Owner->IsPendingKillPending())
	{
		this->M_Owner->GetCurrentLinkCol().Remove(this->M_AMapMeshe);
		//移除通道
		this->M_Owner->GetCurrentLinkChannels().Remove(this);
	}

	//销毁对象
	if (this->M_Channel_In && !this->M_Channel_In->IsPendingKillPending())
	{
		this->M_Channel_In->Destroy();
	}

	//还原通道地形
	this->MesheReset();

	//销毁自己
	this->MouseKill();
	this->Destroy();
}

void ADJ_Channel::SlowDownBuff(FMouseProperty_RecvImformation& _buff)
{
	Super::SlowDownBuff(_buff);

	this->RemoveBuff(_buff.M_SlowDown);
}

void ADJ_Channel::SlowDownBuffOver(FMouseProperty_RecvImformation& _buff)
{
	Super::SlowDownBuffOver(_buff);
}

void ADJ_Channel::SolidificationBuff(FMouseProperty_RecvImformation& _buff)
{
	Super::SolidificationBuff(_buff);

	this->RemoveBuff(_buff.M_Solidification);
}

void ADJ_Channel::SolidificationBuffOver(FMouseProperty_RecvImformation& _buff)
{
	Super::SolidificationBuffOver(_buff);
}

void ADJ_Channel::FreezeBuff(FMouseProperty_RecvImformation& _buff)
{
	Super::FreezeBuff(_buff);

	this->RemoveBuff(_buff.M_Freeze);
}

void ADJ_Channel::FreezeBuffOver(FMouseProperty_RecvImformation& _buff)
{
	Super::FreezeBuffOver(_buff);
}
