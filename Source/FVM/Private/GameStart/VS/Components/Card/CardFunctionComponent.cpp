// Fill out your copyright notice in the Description page of Project Settings.


#include "GameStart/VS/Components/Card/CardFunctionComponent.h"
#include "GameStart/Flipbook/GameActor/Card/FunctionCardActor.h"
#include "GameStart/VS/Components/Item/ShootLineComponent.h"
#include "GameStart/VS/Components/CardManagerComponent.h"
#include "GameStart/VS/Components/MesheControllComponent.h"

#include "GameStart/VS/MapBaseType.h"
#include "GameStart/VS/GameMapInstance.h"

#include "GameStart/VS/UI/UI_MapMeshe.h"
#include "GameStart/VS/UI/Meshe/UIMesheWidthAll.h"
#include "GameStart/VS/UI/UI_Card.h"

#include "GameSystem/Tools/GameSystemFunction.h"

#include  <Kismet/GameplayStatics.h>
#include  <Kismet/KismetSystemLibrary.h>
// Sets default values for this component's properties
UCardFunctionComponent::UCardFunctionComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UCardFunctionComponent::BeginPlay()
{
	Super::BeginPlay();

	//获取卡片拥有者
	this->FunctionCardActor = Cast<AFunctionCardActor>(this->GetOwner());

	if (!this->FunctionCardActor)
	{
		this->SetComponentTickEnabled(false);
		return;
	}

	//功能卡片-开启了碰撞重叠的功能【绑定事件】
	const FItemCardFunction& Data = this->FunctionCardActor->GetFunctionCardData();
	if (Data.M_Enable_CollisionFunc)
	{
		//组件绑定
		if (IsValid(this->FunctionCardActor->GetOverlapBoxComponent()))
		{
			this->FunctionCardActor->GetOverlapBoxComponent()->
				OnComponentBeginOverlap.AddDynamic(this, &UCardFunctionComponent::EventTrigger);
		}
	}

	//获取上一次使用的卡片
	this->M_PlayerLastSelectCardActorClass = AGameMapInstance::GetGameMapInstance()->
		M_CardManagerComponent->M_LastSelectCardClass;
	this->M_PlayerLastSelectCardActorName = AGameMapInstance::GetGameMapInstance()->
		M_CardManagerComponent->M_LastSelectCardName;

	//获取卡片格子
	this->M_CardMapMeshe = this->FunctionCardActor->GetUIMapMesh();

	//初始化生长功能
	this->M_bCardGrowFunc = this->FunctionCardActor->GetFunctionCardData().M_Function_Grow.M_Grow_Func;

	//加载动画
	if (!this->FunctionCardActor->CardActor_DefAniml.Get())
	{
		this->FunctionCardActor->CardActor_DefAniml.LoadSynchronous();
	}
	if (!this->FunctionCardActor->CardActor_BeActiveAnim.Get())
	{
		this->FunctionCardActor->CardActor_BeActiveAnim.LoadSynchronous();
	}
	if (!this->FunctionCardActor->CardActor_GrowAnim.Get())
	{
		this->FunctionCardActor->CardActor_GrowAnim.LoadSynchronous();
	}
	if (!this->FunctionCardActor->CardActor_GrowFinishAnim.Get())
	{
		this->FunctionCardActor->CardActor_GrowFinishAnim.LoadSynchronous();
	}

	//播放默认动画
	if (this->FunctionCardActor->CardActor_DefAniml.Get())
	{
		this->FunctionCardActor->SetPlayAnimation(this->FunctionCardActor->CardActor_DefAniml.Get());
	}

	//绑定动画播放完毕函数
	this->FunctionCardActor->GetRenderComponent()->OnAnimationPlayEnd.BindUObject(
		this, &UCardFunctionComponent::OnAnimationPlayEnd
	);
}

void UCardFunctionComponent::Func_Delegate(FFlyItem_Influence_Tag _Tag, AActor* OtherActor, UCardFunctionComponent* _Class, void(UCardFunctionComponent::* _Func)(AActor* _A))
{
	for (auto Tag : Cast<AFlyItemActor>(OtherActor)->M_FlyItem_Influence_Tag)
	{
		if (Tag == _Tag)
		{
			(_Class->*_Func)(OtherActor);
			return;
		}
	}
}

void UCardFunctionComponent::CardFunc_TransfromDayToNight()
{
	if (this->FunctionCardActor->GetFunctionCardData().M_Function_Once.M_TransfromDayToNignt)
	{
		for (auto& Data : this->FunctionCardActor->GetUIMapMesh()->GetCardDatas())
		{
			if (Data.Value->GetCardDay())
			{
				Data.Value->CardDayTransform(false);
			}
		}
	}
}

void UCardFunctionComponent::CardFunc_TransfromNightToDay()
{
	if (this->FunctionCardActor->GetFunctionCardData().M_Function_Once.M_TransfromNigntToDay)
	{
		for (auto& Data : this->FunctionCardActor->GetUIMapMesh()->GetCardDatas())
		{
			if (!Data.Value->GetCardDay())
			{
				Data.Value->CardDayTransform(true);
			}
		}
	}
}

void UCardFunctionComponent::CardFunc_CurrentMesheCoodDownFinish()
{
	if (this->FunctionCardActor->GetFunctionCardData().M_Function_Once.M_RefreshCurrentCardCoodDown)
	{
		TArray<int32> ID = { 0,1 ,-1 };

		for (auto id : ID)
		{
			if (UUI_Card* UICard = this->M_CardMapMeshe->GetCardUIByID(id))
			{
				if (UFVMGameInstance::GetDebug())
				{
					UE_LOG(LogTemp, Error,
						TEXT("[UCardFunctionComponent::CardFunc_CurrentMesheCoodDownFinish]当前加速冷却的卡片：%s"),
						*UICard->GetCardName());
				}

				UICard->SetCoolDownFinish();

				return;
			}
		}
	}
}

void UCardFunctionComponent::CardFunc_AllCardCoodDownFinish()
{
	if (this->FunctionCardActor->GetFunctionCardData().M_Function_Once.M_RefreshAllCardCoodDown)
	{
		for (const auto& Item : AGameMapInstance::GetCardManagerComponent_Static()->GetUICardArrays())
		{
			if (
				Item->GetCardName().Equals(TEXT("冰淇淋")) ||
				Item->GetCardName().Equals(TEXT("果蔬冰淇淋")) ||
				Item->GetCardName().Equals(TEXT("极寒冰沙"))
				)
			{
				continue;
			}
			else
			{
				Item->SetCoolDownFinish();
			}
		}
	}
}

void UCardFunctionComponent::CardFunc_CopyNextCard()
{
	//如果开启了复制上一次放置的卡片
	if (!this->FunctionCardActor->GetFunctionCardData().M_Function_Once.M_UseBackCard)
	{
		return;
	}

	//判断资产是否有效
	if (IsValid(this->M_PlayerLastSelectCardActorClass))
	{
		//创建卡片
		this->FunctionCardActor->GetUIMapMesh()->CreateCard(this->FunctionCardActor->GetUIMapMesh(),
			AGameMapInstance::GetGameMapInstance()->M_CardManagerComponent,
			this->M_PlayerLastSelectCardActorClass,
			this->M_PlayerLastSelectCardActorName,
			0,
			false,
			false);
	}
	else {
		if (UFVMGameInstance::GetDebug())
		{
			UE_LOG(LogTemp, Error, TEXT("[UCardFunctionComponent::CardFunc_CopyNextCard][%s]资产无效，无法创建卡片")
				, *this->M_PlayerLastSelectCardActorName);
		}
	}
}

void UCardFunctionComponent::CardFunc_BombEffect()
{
	if (this->FunctionCardActor->GetFunctionCardData().M_Function_Once.M_Bomb.M_Bomb)
	{
		//播放音效
		UFVMGameInstance::PlayBGM_S_Static(TEXT("Bomb"), TEXT("ItemAudio"));

		FVector Begin(this->FunctionCardActor->GetFunctionCardData().M_Function_Once.M_Bomb.BeginPosition.X, this->FunctionCardActor->GetFunctionCardData().M_Function_Once.M_Bomb.BeginPosition.Y, 0.f);
		FVector End(this->FunctionCardActor->GetFunctionCardData().M_Function_Once.M_Bomb.EndPosition.X, this->FunctionCardActor->GetFunctionCardData().M_Function_Once.M_Bomb.EndPosition.Y, 0.f);

		Begin += Cast<ACardActor>(this->FunctionCardActor)->GetActorLocation();
		End += Cast<ACardActor>(this->FunctionCardActor)->GetActorLocation();

		TArray<AActor*> Obj;
		TArray<FHitResult> Hits;

		//计算攻击力
		float ATKBase = this->FunctionCardActor->GetFunctionCardData().M_Function_Once.M_Bomb.ATK;
		float ATKRate = this->FunctionCardActor->GetFunctionCardData().M_Function_Once.M_Bomb.ATKRate;
		float CardGradeATKBase = this->FunctionCardActor->GetFunctionCardData().M_Function_Once.M_Bomb.CardGradeATKRate;
		int32 CardGrade = this->FunctionCardActor->GetCardGrade(this->FunctionCardActor->GetFunctionCardData().ItemName);

		float ATK = ATKBase + (ATKBase / 2.f) * ATKRate * CardGrade * CardGradeATKBase;

		if (UFVMGameInstance::GetDebug())
		{
			UGameSystemFunction::FVMLog(__FUNCTION__, FString(TEXT("爆炸造成的伤害：") + FString::SanitizeFloat(ATK)));
		}

		if (UKismetSystemLibrary::SphereTraceMulti(this->GetWorld(), Begin, End, this->FunctionCardActor->GetFunctionCardData().M_Function_Once.M_Bomb.Radius, UGameSystemFunction::GetMouseCollisionTraceType(EMouseCollisionType::MouseActor), false, Obj, EDrawDebugTrace::ForDuration, Hits, false))
		{
			FItem_Buff Buff = this->FunctionCardActor->GetFunctionCardData().M_Function_Once.M_Bomb.M_Buffs;
			for (auto Hit : Hits)
			{
				if (AMouseActor* Mouse = Cast<AMouseActor>(Hit.GetActor()))
				{
					UGameSystemFunction::HitMouse(ATK,
						Mouse,
						Buff
					);
				}
			}
		}
	}
}

void UCardFunctionComponent::CardFunc_CardActive()
{
	//如果选择关闭碰撞
	if (!this->FunctionCardActor->GetFunctionCardData().M_Function_Active.M_CollisionEnable)
	{
		for (auto& Comp : this->FunctionCardActor->GetComponents())
		{
			if (Cast<UBoxComponent>(Comp))
			{
				Cast<UBoxComponent>(Comp)->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			}
		}
	}



	//如果选择更正动画
	if (this->FunctionCardActor->GetFunctionCardData().M_Function_Active.M_CardAnimChange)
	{
		this->FunctionCardActor->GetMyActor()->SetPlayAnimation(
			Cast<UPaperFlipbook>(this->FunctionCardActor->GetFunctionCardData().
				M_Function_Active.M_ChangeAnim.TryLoad())
		);
	}
}

void UCardFunctionComponent::Bullet_ATK_Up(AActor* OtherActor)
{
	//是否有穿越属性->如果是因为穿越属性导致的攻击力增加
	if (this->FunctionCardActor->GetFunctionCardData().M_Function_Collision.M_Across_Func)
	{
		//因为穿越属性并且开启火盆特殊转换功能
		if (this->FunctionCardActor->GetFunctionCardData().M_Function_Collision.M_Across_Brazier_Func)
		{
			//如果遇到当前飞行物携带减速buff -> 攻击力不会提升
			for (auto Buff : Cast<AFlyItemActor>(OtherActor)->GetBuff().M_Buffers)
			{
				if (Buff.Key == Buff_Infor::E_SlowDown)
				{
					return;
				}
			}
		}
	}

	//提升攻击力
	(Cast<AFlyItemActor>(OtherActor))->SetCurATK(
		this->FunctionCardActor->GetFunctionCardData().M_Function_Collision.M_UpATK_Rate
		* (Cast<AFlyItemActor>(OtherActor))->GetATK()
	);
}

void UCardFunctionComponent::Bullet_Rebound(AActor* OtherActor)
{
	//被标记之后直接退出
	if (OtherActor && Cast<AFlyItemActor>(OtherActor)->IsPendingKillPending())
		return;

	for (auto Comp : Cast<AFlyItemActor>(OtherActor)->GetComponents())
	{
		if (Cast<UShootLineComponent>(Comp))
		{
			UShootLineComponent* _Comp = Cast<UShootLineComponent>(Comp);

			//修改方向
			switch (_Comp->GetMoveDirection())
			{
			case EShootDirection::EDown:
			{
				_Comp->SetMoveDirection(EShootDirection::EUp);
				Cast<AFlyItemActor>(OtherActor)->SetFlipbookPitchRotation(90.f);
				return;
			}
			case EShootDirection::ELeft:
			{
				_Comp->SetMoveDirection(EShootDirection::ERight);
				Cast<AFlyItemActor>(OtherActor)->SetFlipbookPitchRotation(-180);
				Cast<AFlyItemActor>(OtherActor)->GetMyActor()->SetRelativeLocation(Cast<AFlyItemActor>(OtherActor)->GetMyActor()->GetRelativeLocation() * -1.f);
				return;
			}
			case EShootDirection::ERight:
			{
				_Comp->SetMoveDirection(EShootDirection::ELeft);
				Cast<AFlyItemActor>(OtherActor)->SetFlipbookPitchRotation(180);
				Cast<AFlyItemActor>(OtherActor)->GetMyActor()->SetRelativeLocation(Cast<AFlyItemActor>(OtherActor)->GetMyActor()->GetRelativeLocation() * -1.f);
				return;
			}
			case EShootDirection::EUp:
			{
				_Comp->SetMoveDirection(EShootDirection::EDown);
				Cast<AFlyItemActor>(OtherActor)->SetFlipbookPitchRotation(-90.f);
				return;
			}
			case EShootDirection::ERotation:
			{
				//设置位移方向
				Cast<AFlyItemActor>(OtherActor)->SetActorRelativeRotation(FRotator(Cast<AFlyItemActor>(OtherActor)->GetActorRotation().Pitch, Cast<AFlyItemActor>(OtherActor)->GetActorRotation().Yaw, Cast<AFlyItemActor>(OtherActor)->GetActorRotation().Roll + 180.f));
				//设置角度
				//Cast<AFlyItemActor>(OtherActor)->SetFlipbookPitchRotation(Cast<AFlyItemActor>(OtherActor)->GetMyActor()->GetRelativeRotation().Pitch + 180.f);
				return;
			}
			}
		}
	}
}

void UCardFunctionComponent::Bullet_Across(AActor* OtherActor)
{
	//获取射击组件（原来的子弹）
	UShootLineComponent* _Comp = nullptr;

	for (auto Comp : Cast<AFlyItemActor>(OtherActor)->GetComponents())
	{
		if (Cast<UShootLineComponent>(Comp))
		{
			_Comp = Cast<UShootLineComponent>(Comp);
			break;
		}
	}

	//没有该组件
	if (!_Comp)
	{
		return;
	}

	//飞行物子弹对象
	UClass* FlyActor_Class = LoadClass<AFlyItemActor>(0,
		*this->FunctionCardActor->GetFunctionCardData().M_Function_Collision.M_Across_BulletClass.ToString()
		);

	//（子弹的资源替换）路径不为空
	if (FlyActor_Class)
	{
		//如果开启火盆特殊转换功能
		if (this->FunctionCardActor->GetFunctionCardData().M_Function_Collision.M_Across_Brazier_Func)
		{
			//如果遇到当前飞行物携带减速buff
			for (auto Buff : Cast<AFlyItemActor>(OtherActor)->GetBuff().M_Buffers)
			{
				if (Buff.Key == Buff_Infor::E_SlowDown)
				{
					//重新替换飞行物
					FlyActor_Class =
						LoadClass<AFlyItemActor>(0,
							TEXT("Blueprint'/Game/Resource/BP/GameStart/Item/Items/小笼包子弹_无偏移.小笼包子弹_无偏移_C'")
							);
				}
			}
		}

		//预创建飞行物
		AFlyItemActor* _FlyActor = Cast<AFlyItemActor>(
			UGameplayStatics::BeginDeferredActorSpawnFromClass(this->GetWorld(),
				FlyActor_Class, Cast<AFlyItemActor>(OtherActor)->GetActorTransform())
			);
		//获取数据
		Cast<AFlyItemActor>(OtherActor)->FlyItemActorSwap(_FlyActor);
		//设置功能组件数据(防止某些功能重复使用)
		_FlyActor->SetActor_CardFunction_Component(Cast<UActorComponent>(this));
		//完成生成
		AFlyItemActor* Finish_Class = Cast<AFlyItemActor>(
			UGameplayStatics::FinishSpawningActor(_FlyActor,
				Cast<AFlyItemActor>(OtherActor)->GetActorTransform())
			);


		bool LResult = false;
		//获取射击组件（复制给新的子弹）
		for (auto CurComp : Finish_Class->GetComponents())
		{
			if (Cast<UShootLineComponent>(CurComp))
			{
				switch (_Comp->GetMoveDirection())
				{
					//如果是上下或者带有角度的对象  统一右边发射并且统一 位置
				case EShootDirection::EDown:
				case EShootDirection::EUp:
				{
					//设置旋转
					Finish_Class->SetFlipbookPitchRotation(0.f);
					Finish_Class->SetActorLocation(this->FunctionCardActor->GetActorLocation());
					Cast<UShootLineComponent>(CurComp)->SetMoveDirection(EShootDirection::ERight);
				}
				break;
				case EShootDirection::ELeft:
				{
					FVector LocalPos = Finish_Class->GetMyActor()->GetRelativeLocation();
					LocalPos.Y *= -1.f;

					Finish_Class->GetMyActor()->SetRelativeLocation(LocalPos);
					Finish_Class->GetMyActor()->SetRelativeScale3D(FVector(-1.f, 1.f, 1.f));

					Cast<UShootLineComponent>(CurComp)->SetMoveDirection(EShootDirection::ELeft);
				}
				break;
				//其他则排除按照原方向
				default:Cast<UShootLineComponent>(CurComp)->SetMoveDirection(_Comp->GetMoveDirection()); break;
				}

				Cast<UShootLineComponent>(CurComp)->M_MoveNode.M_Speed = _Comp->M_MoveNode.M_Speed;
				//步骤结束(成功)
				LResult = true;
				break;
			}
		}



		//销毁原来的飞行物
		Cast<AFlyItemActor>(OtherActor)->Destroy();

		//如果步骤失败则销毁
		if (!LResult)
		{
			Finish_Class->Destroy();
		}
	}
}

void UCardFunctionComponent::Bullet_Buff_Set(AActor* OtherActor)
{
	//添加buff
	if ((Cast<AFlyItemActor>(OtherActor)) && !(Cast<AFlyItemActor>(OtherActor))->IsPendingKillPending())
	{
		TMap<Buff_Infor, float> Buffs = this->FunctionCardActor->GetFunctionCardData().M_Function_Collision.M_Buff_Set;
		(Cast<AFlyItemActor>(OtherActor))->AddBuff(Buffs);
	}
}

void UCardFunctionComponent::CardGrowFunction(const float& Tick)
{
	//开启了生长功能
	if (this->M_bCardGrowFunc)
	{
		if (this->FunctionCardActor->GetFunctionCardData().M_Function_Grow.M_GrowTime > 0.f)
		{
			//切换默认生长动画
			if (!this->M_bCardGrowBegin)
			{
				this->M_bCardGrowBegin = true;

				//计算生长时间
				float SourceTime = this->FunctionCardActor->GetFunctionCardData().M_Function_Grow.M_GrowTime;

				float ResultTime = SourceTime -
					SourceTime * this->FunctionCardActor->GetFunctionCardData().M_Function_Grow.M_GrowTimeRate *
					this->FunctionCardActor->GetCardGrade(this->FunctionCardActor->GetFunctionCardData().ItemName) *
					this->FunctionCardActor->GetFunctionCardData().M_M_ECardUpGradeUpRate / SourceTime;

				this->M_fCardGrowTime = ResultTime;
			}

			//开始生长倒计时
			if (this->M_fCardGrowTime > 0.f && this->M_bCardGrowBegin)
			{
				this->M_fCardGrowTime -= Tick;
			}
			else if (this->M_fCardGrowTime <= 0.f && this->M_bCardGrowBegin)
			{
				//开始播放生长动画
				this->FunctionCardActor->SetPlayAnimationOnce(
					this->FunctionCardActor->CardActor_GrowAnim.Get(),
					this->FunctionCardActor->CardActor_GrowFinishAnim.Get()
				);

				//播放BGM
				if (!this->FunctionCardActor->GetFunctionCardData().M_Function_Grow.M_GrowAudioName.Equals(""))
				{
					UFVMGameInstance::PlayBGM_S_Static(
						this->FunctionCardActor->GetFunctionCardData().M_Function_Grow.M_GrowAudioName,
						TEXT("ItemAudio")
					);
				}

				this->M_fCardGrowTime = 0.f;
				this->M_bCardGrowFinish = true;
				this->M_bCardGrowFunc = false;

				//检查周围老鼠
				this->CheckOverlapMouseCreateBomb();
			}
		}
	}
}

void UCardFunctionComponent::OnAnimationPlayEnd()
{
	if (FVM_VS_GamePause())
	{
		return;
	}

	//卡片休息
	if (!this->FunctionCardActor->GetCardDay() && this->FunctionCardActor->GetMapDay())
	{
		return;
	}

	if (this->M_AnimPlayEndDeath)
	{

		this->FunctionCardActor->KillCard();

		return;
	}

	if (this->FunctionCardActor->GetFunctionCardData().M_UseOnceKill)
	{
		this->SetComponentTickEnabled(false);

		//调用功能
		this->CardFunc_TransfromDayToNight();
		this->CardFunc_TransfromNightToDay();
		this->CardFunc_CurrentMesheCoodDownFinish();
		this->CardFunc_AllCardCoodDownFinish();
		this->CardFunc_BombEffect();
		this->CardFunc_CopyNextCard();

		//销毁卡片
		this->FunctionCardActor->KillCard();

		return;
	}
}

void UCardFunctionComponent::EventTrigger(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//卡片休息
	if (!this->FunctionCardActor->GetCardDay() && this->FunctionCardActor->GetMapDay())
	{
		return;
	}

	if (IsValid(OtherActor))
	{
		//飞行物触发
		this->Trigger_FlyItem(OtherActor);

		//老鼠触发
		this->Trigger_MouseActor(OtherActor);
	}
}

void UCardFunctionComponent::Trigger_FlyItem(AActor* OtherActor)
{
	//子弹转换失败
	if (!Cast<AFlyItemActor>(OtherActor))
	{
		return;
	}

	//子弹不受影响
	if (!Cast<AFlyItemActor>(OtherActor)->GetFlyInfluence())
	{
		return;
	}

	//穿越组件判断当前对象是否拥有当前功能组件
	if (
		Cast<UActorComponent>(Cast<AFlyItemActor>(OtherActor)->GetActor_CardFunction_Component())
		== Cast<UActorComponent>(this)
		)
	{
		return;
	}

	//攻击力提升
	if (this->FunctionCardActor->GetFunctionCardData().M_Function_Collision.M_UpATK)
		this->Func_Delegate(FFlyItem_Influence_Tag::EATK_Up, OtherActor, this, &UCardFunctionComponent::Bullet_ATK_Up);
	//子弹穿越
	if (this->FunctionCardActor->GetFunctionCardData().M_Function_Collision.M_Across_Func)
		this->Func_Delegate(FFlyItem_Influence_Tag::EAcross, OtherActor, this, &UCardFunctionComponent::Bullet_Across);
	//子弹Buff
	if (this->FunctionCardActor->GetFunctionCardData().M_Function_Collision.M_Buff_Func)
		this->Func_Delegate(FFlyItem_Influence_Tag::EBuff, OtherActor, this, &UCardFunctionComponent::Bullet_Buff_Set);
	//开启弹射功能
	if (this->FunctionCardActor->GetFunctionCardData().M_Function_Collision.M_Rebound_Item)
		this->Func_Delegate(FFlyItem_Influence_Tag::ERebound, OtherActor, this, &UCardFunctionComponent::Bullet_Rebound);
}

void UCardFunctionComponent::Trigger_MouseActor(AActor* OtherActor)
{
	if (!Cast<AMouseActor>(OtherActor))
	{
		return;
	}

	if (Cast<AMouseActor>(OtherActor)->GetMouseIsDeath())
	{
		return;
	}

	//判断老鼠线路
	if (Cast<AMouseActor>(OtherActor)->GetMouseLine().Row != this->FunctionCardActor->GetLine().Row)
	{
		return;
	}

	this->CheckOverlapMouseCreateBomb();
}

void UCardFunctionComponent::CreateBeHurtBombHittMouse()
{
	TArray<FHitResult> Hits;
	if (UGameSystemFunction::AddSphereTraceMulti(this->FunctionCardActor, Hits,
		this->FunctionCardActor->GetFunctionCardData().M_Function_BeHurtBomb.BeginPosition,
		this->FunctionCardActor->GetFunctionCardData().M_Function_BeHurtBomb.EndPosition,
		this->FunctionCardActor->GetFunctionCardData().M_Function_BeHurtBomb.M_Radius,
		UGameSystemFunction::GetMouseCollisionTraceType(
			this->FunctionCardActor->GetFunctionCardData().M_Function_BeHurtBomb.M_MouseCollisionType
		)))
	{

		//播放爆炸动画
		this->FunctionCardActor->SetPlayAnimation(
			Cast<UPaperFlipbook>(this->FunctionCardActor->GetFunctionCardData().M_Function_BeHurtBomb.M_BombAnimPath.TryLoad())
		);

		//播放爆炸音效
		if (!this->FunctionCardActor->GetFunctionCardData().M_Function_BeHurtBomb.M_BombAudioName.Equals(""))
		{
			UFVMGameInstance::PlayBGM_S_Static(
				this->FunctionCardActor->GetFunctionCardData().M_Function_BeHurtBomb.M_BombAudioName,
				TEXT("ItemAudio")
			);
		}

		AMouseActor* MouseAc = nullptr;
		FItem_Buff CurBuff = this->FunctionCardActor->GetFunctionCardData().M_Function_BeHurtBomb.M_Buffs;
		for (auto& Mouse : Hits)
		{
			MouseAc = Cast<AMouseActor>(Mouse.GetActor());

			if (IsValid(MouseAc) && MouseAc->GetMouseLine().Row == this->FunctionCardActor->GetLine().Row)
			{
				if (UGameSystemFunction::HitMouse(
					this->FunctionCardActor->GetFunctionCardData().M_Function_BeHurtBomb.M_BombATK,
					MouseAc,
					CurBuff)
					)
				{
					if (UFVMGameInstance::GetDebug())
					{
						UGameSystemFunction::FVMLog(__FUNCTION__, TEXT("通过碰撞爆炸基于此范围所有的老鼠造成固定伤害"));
					}
				}
			}
		}

		this->M_AnimPlayEndDeath = true;
		this->FunctionCardActor->GetOverlapBoxComponent()->OnComponentBeginOverlap.RemoveAll(this);
		this->FunctionCardActor->GetOverlapBoxComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		this->FunctionCardActor->GetBoxComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

void UCardFunctionComponent::CheckOverlapMouseCreateBomb()
{
	//如果开启了生长并且开启了触发爆炸并且连接了生长（并且生长已经完成）
	if (this->FunctionCardActor->GetFunctionCardData().M_Function_Grow.M_Grow_Func
		&&
		this->M_bCardGrowFinish
		&&
		this->FunctionCardActor->GetFunctionCardData().M_Function_BeHurtBomb.M_BeHurtBomb_Func
		&&
		this->FunctionCardActor->GetFunctionCardData().M_Function_BeHurtBomb.M_LinkGrowFunc)
	{
		this->CreateBeHurtBombHittMouse();
	}
	//只开启了爆炸并且没有连接生长模式(碰撞及爆炸)
	else if (this->FunctionCardActor->GetFunctionCardData().M_Function_BeHurtBomb.M_BeHurtBomb_Func
		&&
		!this->FunctionCardActor->GetFunctionCardData().M_Function_BeHurtBomb.M_LinkGrowFunc)
	{
		this->CreateBeHurtBombHittMouse();
	}
}

// Called every frame
void UCardFunctionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (FVM_VS_GamePause())
	{
		return;
	}

	//卡片休息
	if (!this->FunctionCardActor->GetCardDay() && this->FunctionCardActor->GetMapDay())
	{
		return;
	}

	//卡片激活
	if (this->FunctionCardActor->GetFunctionCardData().M_Function_Active.M_CardActive)
	{
		//使用卡片激活时会触发的功能（只会执行一次）
		if (!this->M_bCardActive)
		{
			this->M_bCardActive = true;

			this->CardFunc_CardActive();
		}
	}

	//生长
	this->CardGrowFunction(DeltaTime);
}

