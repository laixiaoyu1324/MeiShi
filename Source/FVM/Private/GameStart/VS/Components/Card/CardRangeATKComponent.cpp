// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除


#include "GameStart/VS/Components/Card/CardRangeATKComponent.h"

#include "GameStart/Flipbook/GameActor/FlyItemActor.h"
#include "GameStart/VS/Components/MesheControllComponent.h"
#include "GameStart/VS/GameMapInstance.h"

#include <Paper2D/Classes/PaperSprite.h>
#include <Paper2D/Classes/PaperFlipbook.h>
#include <Paper2D/Classes/PaperFlipbookComponent.h>

#include <Components/BoxComponent.h>

#include "GameSystem/Tools/GameSystemFunction.h"

#include <Kismet/GameplayStatics.h>
#include <Kismet/KismetSystemLibrary.h>

void UCardRangeATKComponent::SetCardRange(UBoxComponent* M_BoxRange)
{
	this->M_CardRange_Box = M_BoxRange;

	if (this->M_CardRange_Box)
	{
		this->M_CardRange_Box->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		this->M_CardRange_Box->SetCollisionObjectType(ECollisionChannel::ECC_Pawn);
		this->M_CardRange_Box->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
		this->M_CardRange_Box->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);

		//debug模式显示碰撞
	//	if (UFVMGameInstance::GetDebug())
		//	this->M_CardRange_Box->SetHiddenInGame(false);

		//绑定碰撞
		this->M_CardRange_Box->OnComponentBeginOverlap.AddDynamic(this, &UCardRangeATKComponent::RangeOverlapBegin);
		this->M_CardRange_Box->OnComponentEndOverlap.AddDynamic(this, &UCardRangeATKComponent::RangeOverlapEnd);
	}
}

void UCardRangeATKComponent::RangeOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//如果开启了自动攻击->则无需检测
	if (this->RangeATKCardActor->GetRangeATKCardData().M_AutoAttack)
	{
		return;
	}

	ETraceTypeQuery Type = ETraceTypeQuery::TraceTypeQuery1;

	switch (this->RangeATKCardActor->GetRangeATKCardData().M_ELineTraceType)
	{
	case ELineTraceType::E_MouseGround:Type = ETraceTypeQuery::TraceTypeQuery4; break;
	case ELineTraceType::E_MouseSky:Type = ETraceTypeQuery::TraceTypeQuery5; break;
	case ELineTraceType::E_MouseUnder:Type = ETraceTypeQuery::TraceTypeQuery6; break;
	}

	bool BResult = false;

	if (AMouseActor* Mouse = Cast<AMouseActor>(OtherActor))
	{
		for (auto& Comp : Mouse->GetComponents())
		{
			if (UBoxComponent* Box = Cast<UBoxComponent>(Comp))
			{
				//判断是否是实体(任何)
				if (Type == ETraceTypeQuery::TraceTypeQuery1)
					if (Box->GetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility) == ECollisionResponse::ECR_Block)
						BResult = true;

				//判断是否是实体(陆地)
				if (Type == ETraceTypeQuery::TraceTypeQuery4 || Type == ETraceTypeQuery::TraceTypeQuery1)
					if (Box->GetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel2) == ECollisionResponse::ECR_Block)
						BResult = true;

				//判断是否是实体(飞行)
				if (Type == ETraceTypeQuery::TraceTypeQuery5 || Type == ETraceTypeQuery::TraceTypeQuery1)
					if (Box->GetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel3) == ECollisionResponse::ECR_Block)
						BResult = true;

				//判断是否是实体(地下)
				if (Type == ETraceTypeQuery::TraceTypeQuery6 || Type == ETraceTypeQuery::TraceTypeQuery1)
					if (Box->GetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel4) == ECollisionResponse::ECR_Block)
						BResult = true;

				//如果成功则跳出
				if (BResult)
					break;
			}
		}
	}

	if (BResult)
	{
		//添加老鼠
		this->M_Mouses.Emplace(Cast<AMouseActor>(OtherActor));
		//设置攻击模式开
		this->SetAttackModEnabled(true);
		//进入
		if (UFVMGameInstance::GetDebug())
			UGameSystemFunction::FVMLog(__FUNCTION__, TEXT("老鼠进入检测范围"));
	}
}

void UCardRangeATKComponent::RangeOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	//如果开启了自动攻击->则无需检测
	if (this->RangeATKCardActor->GetRangeATKCardData().M_AutoAttack)
		return;

	if (Cast<AMouseActor>(OtherActor))
	{
		if (UFVMGameInstance::GetDebug())
			UGameSystemFunction::FVMLog(__FUNCTION__, TEXT("老鼠退出检测范围"));

		this->M_Mouses.Remove(Cast<AMouseActor>(OtherActor));

		//如果数量为0 则关闭攻击模式
		if (this->M_Mouses.Num() == 0)
		{
			this->SetAttackModEnabled(false);

			if (UFVMGameInstance::GetDebug())
				UGameSystemFunction::FVMLog(__FUNCTION__, TEXT("当前范围没有任何老鼠，关闭攻击模式"));
		}
	}
}

UCardRangeATKComponent::UCardRangeATKComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UCardRangeATKComponent::BeginPlay()
{
	Super::BeginPlay();

	this->RangeATKCardActor = Cast<ARangeATKCardActor>(this->GetOwner());

	if (!this->RangeATKCardActor)
	{
		UE_LOG(LogTemp, Error, TEXT("获取卡片ARangeATKCardActor失败!"));
		this->SetComponentTickEnabled(false);
		return;
	}
}

void UCardRangeATKComponent::Spawn()
{
	Super::Spawn();

	ETraceTypeQuery Type = ETraceTypeQuery::TraceTypeQuery1;

	switch (this->RangeATKCardActor->GetRangeATKCardData().M_ELineTraceType)
	{
	case ELineTraceType::E_MouseGround:Type = ETraceTypeQuery::TraceTypeQuery4; break;
	case ELineTraceType::E_MouseSky:Type = ETraceTypeQuery::TraceTypeQuery5; break;
	case ELineTraceType::E_MouseUnder:Type = ETraceTypeQuery::TraceTypeQuery6; break;
	}


	bool BResult = false;
	TArray<FHitResult> Hits;
	if (UGameSystemFunction::AddSphereTraceMulti(this->RangeATKCardActor,
		Hits,
		FVector::ZeroVector,
		FVector::ZeroVector,
		this->RangeATKCardActor->GetRangeATKCardData().M_ATKRadius,
		Type)
		)
	{
		FItem_Buff Buff = this->RangeATKCardActor->GetRangeATKCardData().M_FItem_Buff;

		for (auto& Mouse : Hits)
		{
			if (AMouseActor* LMouse = Cast<AMouseActor>(Mouse.GetActor()))
			{
				if (IsValid(LMouse) && LMouse->GetCurrentHP() > 0.f)
				{
					UGameSystemFunction::HitMouse(
						this->RangeATKCardActor->GetCurrentATK(),
						LMouse,
						Buff);
				}
			}
		}
	}

	//是否开启了矩阵
	if (this->RangeATKCardActor->GetRangeATKCardData().M_FCardRangeATK_Matrix.M_bEnable)
	{
		UGameSystemFunction::CreateMapMatrix(this->RangeATKCardActor->GetLine(),
			this->RangeATKCardActor->GetRangeATKCardData().M_FCardRangeATK_Matrix.M_RowCount,
			this->RangeATKCardActor->GetRangeATKCardData().M_FCardRangeATK_Matrix.M_ColCount,
			this->RangeATKCardActor->GetRangeATKCardData().M_FCardRangeATK_Matrix.M_Offset,
			this->RangeATKCardActor->GetRangeATKCardData().M_FCardRangeATK_Matrix.M_AnimPath
		);
	}

}

void UCardRangeATKComponent::PlayAttackAnimation()
{
	Super::PlayAttackAnimation();

	this->RangeATKCardActor->SetPlayAnimation(this->RangeATKCardActor->CardActor_AttackAnim.Get());

	if (this->RangeATKCardActor->GetRangeATKCardData().M_FCardRangeATK_Audio.M_bEnable)
	{
		//播放音效
		UFVMGameInstance::PlayBGM_S_Static(
			this->RangeATKCardActor->GetRangeATKCardData().M_FCardRangeATK_Audio.M_AudioName,
			TEXT("ItemAudio"));
	}
}

void UCardRangeATKComponent::PlayIdleAnimation()
{
	Super::PlayIdleAnimation();

	this->RangeATKCardActor->SetPlayAnimation(this->RangeATKCardActor->CardActor_DefAnim.Get());
}

void UCardRangeATKComponent::InitCondition()
{
	Super::InitCondition();
}

void UCardRangeATKComponent::LoadResource()
{
	Super::LoadResource();

	//加载动画
	if (!this->RangeATKCardActor->CardActor_DefAnim.Get())
	{
		this->RangeATKCardActor->CardActor_DefAnim.LoadSynchronous();
	}

	if (!this->RangeATKCardActor->CardActor_AttackAnim.Get())
	{
		this->RangeATKCardActor->CardActor_AttackAnim.LoadSynchronous();
	}

	//初始化攻击模式
	if (this->RangeATKCardActor->GetRangeATKCardData().M_AutoAttack)
	{
		this->SetAttackModEnabled(true);
	}

	//设置存活时间
	this->M_CurrentTime = this->RangeATKCardActor->GetRangeATKCardData().M_CardLifeTime;

	//初始化发射器
	this->InitLaunchProperty(
		this->RangeATKCardActor->GetCurrentAttackCount(),
		this->RangeATKCardActor->GetCurrentAttackSpeed(),
		this->RangeATKCardActor->GetCurrentFristAttackDelay(),
		this->RangeATKCardActor->GetCurrentSecondAttackDelay()
	);

	//播放默认动画
	this->RangeATKCardActor->SetPlayAnimation(this->RangeATKCardActor->CardActor_DefAnim.Get());


	//打印速度Log
	if (UFVMGameInstance::GetDebug())
	{
		UE_LOG(LogTemp, Error, TEXT("%.2f  %.2f  %.2f"),
			this->RangeATKCardActor->GetCurrentAttackSpeed(),
			this->RangeATKCardActor->GetCurrentFristAttackDelay(),
			this->RangeATKCardActor->GetCurrentSecondAttackDelay());
	}
}

void UCardRangeATKComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (FVM_VS_GamePause())
		return;

	if (!this->RangeATKCardActor)
		return;

	//卡片休息
	if (!this->RangeATKCardActor->GetCardDay() && this->RangeATKCardActor->GetMapDay())
	{
		this->SetAttackModEnabled(false);
		return;
	}

	//如果是自动攻击
	if (this->RangeATKCardActor->GetRangeATKCardData().M_AutoAttack)
	{
		this->SetAttackModEnabled(true);
	}

	//有时间限制
	if (this->RangeATKCardActor->GetRangeATKCardData().M_bEnabledTime)
	{
		this->M_CurrentTime -= DeltaTime;
		if (M_CurrentTime <= 0.f)
		{
			this->SetComponentTickEnabled(false);
			this->RangeATKCardActor->KillCard();
		}
	}
}
