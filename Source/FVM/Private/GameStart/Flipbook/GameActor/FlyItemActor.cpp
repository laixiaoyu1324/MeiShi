// Fill out your copyright notice in the Description page of Project Settings.


#include "GameStart/Flipbook/GameActor/FlyItemActor.h"
#include "GameStart/Flipbook/GameActor/MouseActor.h"

#include "GameStart/VS/Components/Card/CardProjectionComponent.h"
#include "GameStart/VS/Components/Item/ShootLineComponent.h"
#include "GameStart/VS/Components/MesheControllComponent.h"
#include "GameStart/VS/MapMeshe.h"
#include "GameStart/VS/GameMapInstance.h"

#include "GameSystem/GameMapStructManager.h"
#include "GameSystem/FVMGameInstance.h"

#include <Components/SphereComponent.h>

#include <Kismet/GameplayStatics.h>



UActorComponent* const AFlyItemActor::GetActor_CardFunction_Component()
{
	return this->M_AActorComponent_CardFunction;
}

void AFlyItemActor::SetActor_CardFunction_Component(UActorComponent* _Value)
{
	this->M_AActorComponent_CardFunction = _Value;
}

AFlyItemActor::AFlyItemActor()
{
	this->M_SphereCollision = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	this->M_SphereCollision->SetupAttachment(this->GetRootComponent());

	//绑定事件
	this->M_SphereCollision->OnComponentBeginOverlap.AddDynamic(this, &AFlyItemActor::OnBoxOverlapBegin);
	this->M_SphereCollision->OnComponentEndOverlap.AddDynamic(this, &AFlyItemActor::OnBoxOverlapEnd);
}

void AFlyItemActor::InitCollision()
{
	this->M_SphereCollision->SetCollisionObjectType(ECC_Pawn);
	this->M_SphereCollision->SetCollisionResponseToAllChannels(ECR_Ignore);
	this->M_SphereCollision->SetCollisionResponseToChannel(ECC_Pawn, ECollisionResponse::ECR_Overlap);
	this->M_SphereCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void AFlyItemActor::BeginPlay()
{
	Super::BeginPlay();

	//设置渲染层8000
	this->GetRenderComponent()->SetTranslucency(8000);

	if (this->GetLifeSpan() > 5.f)
	{
		if (UFVMGameInstance::GetDebug())
		{
			UE_LOG(LogTemp, Error, TEXT("[AFlyItemActor::BeginPlay]:飞行物时间修正"));
		}
		this->SetLifeSpan(3.5f);
	}

	this->AddActorLocalOffset(FVector(-4.f, 0.f, 0.f));

	//初始化碰撞方式
	this->InitCollision();

	//初始化动画资源
	if (!this->M_FlyItem_Property_AnimRes.M_FlyItemAnim_Fly.Get())
	{
		this->M_FlyItem_Property_AnimRes.M_FlyItemAnim_Fly.LoadSynchronous();
	}
	this->M_FlyItem_Property_AnimRes.M_Begin = this->M_FlyItem_Property_AnimRes.M_FlyItemAnim_Fly.Get();


	if (!this->M_FlyItem_Property_AnimRes.M_FlyItemAnim_Split.Get())
	{
		this->M_FlyItem_Property_AnimRes.M_FlyItemAnim_Split.LoadSynchronous();
	}
	this->M_FlyItem_Property_AnimRes.M_End = this->M_FlyItem_Property_AnimRes.M_FlyItemAnim_Split.Get();

	//播放BGM
	UFVMGameInstance::PlayBGM_S_Static(this->M_FlyItem_Property_AudioBegin.M_AudioName, this->M_FlyItem_Property_AudioBegin.M_AudioRootPathName);

	//播放Split动画
	if (this->M_FlyItem_Property_AnimRes.M_Begin)
	{
		this->GetRenderComponent()->SetFlipbook(this->M_FlyItem_Property_AnimRes.M_Begin);
	}


	//获取指定的组件
	for (UActorComponent* Comp : this->GetComponents())
	{
		if (Cast<UShootLineComponent>(Comp))
		{
			//如果是浮动模式则不受行限制
			if (this->M_bFloatMode)
			{
				this->SetFlyConstraintLine(false);
			}

			break;
		}
	}
}

void AFlyItemActor::SetFloatModeEnable(bool _Value)
{
	this->M_bFloatMode = _Value;
}

void AFlyItemActor::SetFlyConstraintLine(bool _value)
{
	this->M_FlyCondition.M_bConstraintLine = _value;
}

void AFlyItemActor::SetFlyInfluence(bool _value)
{
	this->M_FlyCondition.M_bInfluence = _value;
}

void AFlyItemActor::SetATK(float _value)
{
	this->M_FlyData.ATK = _value;
	this->M_FlyData.CurATK = _value;
}

void AFlyItemActor::SetCurATK(float _value)
{
	this->M_FlyData.CurATK = _value;
}

void AFlyItemActor::SetSecondATK(float _value)
{
	this->M_FlyData._SecondATK = _value;
}

void AFlyItemActor::SetLine(int32 _line)
{
	this->M_FlyData.M_Line = _line;
}

FVector AFlyItemActor::GetMouseActorLocation()
{
	//老鼠对象失效,则让子弹全部失效

	if (!this->M_MouseActorLocation || this->M_MouseActorLocation->IsPendingKillPending())
	{
		return AGameMapInstance::GetGameMapInstance()->GetMesheControllComponent()->GetMapMeshCurrentRight(this->GetLine())->GetActorLocation();
	}

	//如果位置存在
	if (Cast<AMouseActor>(this->M_MouseActorLocation)->M_MousePosition)
		return  Cast<AMouseActor>(this->M_MouseActorLocation)->M_MousePosition->GetComponentLocation();
	else
		return this->M_MouseActorLocation->GetActorLocation();
}

FItem_Buff& AFlyItemActor::GetBuff()
{
	return this->M_FItem_Buff;
}

AActor* const AFlyItemActor::GetMouseActor()
{
	return this->M_MouseActorLocation;
}

TArray<ELineType>& AFlyItemActor::GetAttackType()
{
	return this->M_AttackType;
}

bool AFlyItemActor::GetDidTargetIsHit()
{
	return this->M_FlyCondition.M_bHitTarget;
}

void AFlyItemActor::SetCollisionEnabled(bool _Value)
{
	if (_Value)
		this->M_SphereCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	else
		this->M_SphereCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);

}

void AFlyItemActor::PlayAnim_Fly()
{
	//播放Fly动画
	if (this->M_FlyItem_Property_AnimRes.M_Begin)
	{
		this->GetRenderComponent()->SetPlayAnimation(this->M_FlyItem_Property_AnimRes.M_Begin);
	}
}

void AFlyItemActor::PlayAnim_Split()
{
	if (!this->M_FlyItem_Property_AnimRes.M_End)
	{
		this->Destroy();
	}

	//播放Split动画
	this->GetRenderComponent()->SetPlayAnimation(this->M_FlyItem_Property_AnimRes.M_End);
}

void AFlyItemActor::HitEnd(UPrimitiveComponent* _UBoxComp)
{
	if (_UBoxComp)
	{
		_UBoxComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

void AFlyItemActor::Hit()
{
	//设置击中状态
	this->SetTargetHitState(true);

	this->OnHit();

	//关闭碰撞
	M_SphereCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	//播放BGM
	UFVMGameInstance::PlayBGM_S_Static(
		this->M_FlyItem_Property_AudioEnd.M_AudioName,
		this->M_FlyItem_Property_AudioEnd.M_AudioRootPathName
	);


	//创建静态飞行物对象
	for (auto ResPath_C : this->M_StaticFlyItemClass)
	{
		this->CreateStaticItem(ResPath_C);
	}

	//播放Split动画
	this->PlayAnim_Split();

	//如果是非静态对象则不会被销毁
	if (this->M_FlyCondition.M_bStaticFlyItem)
	{
		this->Destroy();
	}
	else {
		this->SetAnimationPlayEndDestroy();
	}
}

void AFlyItemActor::CreateStaticItem(TSoftClassPtr<AFlyItemActor> CurFlyItemActor)
{
	if (!CurFlyItemActor.Get())
	{
		CurFlyItemActor.LoadSynchronous();
	}

	UClass* Obj = CurFlyItemActor.Get();

	if (!Obj)
	{
		return;
	}

	FTransform Trans = this->GetActorTransform();

	//限制到本行
	if (this->M_StaticFlyItemLocationConstLine)
	{
		Trans.SetLocation(FVector(Trans.GetLocation().X,
			Trans.GetLocation().Y,
			AGameMapInstance::GetGameMapInstance()->
			GetMesheControllComponent()->
			GetMapMeshCurrentRight(this->GetLine())->GetActorLocation().Z)
		);
	}

	AFlyItemActor* Item = Cast<AFlyItemActor>(
		UGameplayStatics::BeginDeferredActorSpawnFromClass(
			this->GetWorld(),
			TSubclassOf<AFlyItemActor>(Obj), Trans)
		);
	Item->M_FlyData.ATK = this->GetSecondATK();
	Item->M_FlyData._SecondATK = this->GetSecondATK();
	Item->M_FlyCondition.M_bStaticFlyItem = true;
	AFlyItemActor* Finish = Cast<AFlyItemActor>(UGameplayStatics::FinishSpawningActor(Item, Trans));

}

bool AFlyItemActor::GetFlyOutsideAutoDestroy()
{
	return this->M_OutsideAutoDestroy;
}

bool AFlyItemActor::GetFlyConstraintLine()
{
	return this->M_FlyCondition.M_bConstraintLine;
}

float AFlyItemActor::GetATK()
{
	return this->M_FlyData.ATK;
}

float AFlyItemActor::GetCurATK()
{
	return this->M_FlyData.CurATK;
}

float AFlyItemActor::GetSecondATK()
{
	return this->M_FlyData._SecondATK;
}

int32 AFlyItemActor::GetLine()
{
	return this->M_FlyData.M_Line;
}

bool AFlyItemActor::GetFlyInfluence()
{
	return this->M_FlyCondition.M_bInfluence;
}

void AFlyItemActor::CreateFlyActor_ShootLine(TSoftClassPtr<AFlyItemActor> _FlyActorPath_C, FTargetNode _Node, int32 _LineOffset, float _Time, FVector Offset, bool _IsbConstaintLine)
{

	//线路判断 不能小于0 不能大于最大值
	if (this->M_FlyData.M_Line + _LineOffset < 0 ||
		this->M_FlyData.M_Line + _LineOffset >=
		AGameMapInstance::GetGameMapInstance()->
		M_MesheControllComponent->
		GetGameMapStructManager()->
		GameMapStruct.M_Meshe.Num()
		)
		return;

	if (!_FlyActorPath_C.Get())
	{
		_FlyActorPath_C.LoadSynchronous();
	}

	//坐标
	FTransform Trans = this->GetTransform();
	Trans.SetLocation(Trans.GetLocation() + Offset);

	AFlyItemActor* _TargetActor = Cast<AFlyItemActor>(UGameplayStatics::BeginDeferredActorSpawnFromClass(this->GetWorld(), _FlyActorPath_C.Get(), Trans));
	//新生成的对象设置自定义拥有者(CardActor)
	_TargetActor->M_CustomActorOwner = this->M_CustomActorOwner;
	_TargetActor->SetMouseActorLocation(this->M_MouseActorLocation);
	_TargetActor->SetATK(this->M_FlyData.ATK);
	_TargetActor->SetSecondATK(this->M_FlyData._SecondATK);
	_TargetActor->SetLine(this->M_FlyData.M_Line + _LineOffset);
	_TargetActor->SetFlipbookRotation(FRotator(0.f, 90.f, 0.f));
	_TargetActor->SetFlyConstraintLine(_IsbConstaintLine);
	_TargetActor->SetLifeSpan(_Time);
	AFlyItemActor* _Actor = Cast<AFlyItemActor>(UGameplayStatics::FinishSpawningActor(_TargetActor, Trans));

	//通过node节点更具Direction设置Pitch旋转
	switch (_Node.M_EShootDirection)
	{
	case EShootDirection::EUp: {_Actor->SetFlipbookPitchRotation(90.f); } break;
	case EShootDirection::EDown: {_Actor->SetFlipbookPitchRotation(-90.f); }break;
	case EShootDirection::ELeft: {
		_Actor->SetFlipbookPitchRotation(180.f);
		_Actor->GetMyActor()->SetRelativeLocation(_Actor->GetMyActor()->GetRelativeLocation() * -1.f);
	}break;
	default:; break;
	}

	//获取指定的组件
	for (UActorComponent* Comp : _Actor->GetComponents())
	{
		if (Cast<UShootLineComponent>(Comp))
		{
			Cast<UShootLineComponent>(Comp)->SetTargetNode(_Node);

			//如果是浮动模式则不受行限制
			if (this->M_bFloatMode)
			{
				_Actor->SetFlyConstraintLine(false);
			}

			break;
		}
	}
}

void AFlyItemActor::CreateFlyActor_ShootLine_Slash(TSoftClassPtr<AFlyItemActor> _FlyActorPath_C, float _RotationAngle, float _Time, FVector Offset)
{
	if (!_FlyActorPath_C.Get())
	{
		_FlyActorPath_C.LoadSynchronous();
	}

	//坐标
	FTransform Trans = this->GetTransform();
	Trans.SetLocation(Trans.GetLocation() + Offset);

	AFlyItemActor* _TargetActor = Cast<AFlyItemActor>(UGameplayStatics::BeginDeferredActorSpawnFromClass(this->GetWorld(), _FlyActorPath_C.Get(), Trans));
	//新生成的对象设置自定义拥有者(CardActor)
	_TargetActor->M_CustomActorOwner = this->M_CustomActorOwner;
	_TargetActor->SetMouseActorLocation(this->M_MouseActorLocation);
	_TargetActor->SetATK(this->M_FlyData.ATK);
	_TargetActor->SetSecondATK(this->M_FlyData._SecondATK);
	_TargetActor->SetFlipbookRotation(FRotator(0.f, 90.f, 0.f));
	_TargetActor->SetFlyConstraintLine(false);
	_TargetActor->SetLifeSpan(_Time);
	_TargetActor->AddActorLocalOffset(Offset);
	_TargetActor->SetActorRelativeRotation(FRotator(0.f, 0.f, _RotationAngle));
	AFlyItemActor* _Actor = Cast<AFlyItemActor>(UGameplayStatics::FinishSpawningActor(_TargetActor, Trans));

	//通过node节点更具Direction设置Pitch旋转
	//_Actor->SetFlipbookPitchRotation(45.f);

	FTargetNode Node;
	Node.M_Speed = 4.f;
	Node.M_EShootDirection = EShootDirection::ERotation;
	//获取指定的组件
	for (UActorComponent* Comp : _TargetActor->GetComponents())
	{
		if (Cast<UShootLineComponent>(Comp))
		{
			Cast<UShootLineComponent>(Comp)->SetTargetNode(Node);
			break;
		}
	}

}

void AFlyItemActor::SetFlipbook(TSubclassOf<UPaperFlipbook>& _Res)
{
	this->GetRenderComponent()->SetFlipbook(_Res.GetDefaultObject());
}

AFlyItemActor* AFlyItemActor::FlyItemActorSwap(AFlyItemActor* _FlyActor)
{
	if (_FlyActor && !_FlyActor->IsPendingKillPending())
	{
		//新生成的对象设置自定义拥有者(CardActor)
		_FlyActor->M_CustomActorOwner = this->M_CustomActorOwner;
		_FlyActor->M_MouseActorLocation = this->M_MouseActorLocation;
		_FlyActor->M_FlyData.ATK = this->M_FlyData.ATK;
		_FlyActor->M_FlyData.CurATK = this->M_FlyData.CurATK;
		_FlyActor->M_FlyData._SecondATK = this->M_FlyData._SecondATK;
		_FlyActor->M_FlyData.M_Line = this->M_FlyData.M_Line;
		_FlyActor->M_AttackType = this->M_AttackType;
		_FlyActor->SetLifeSpan(this->GetLifeSpan());
		_FlyActor->InitRotation();
		_FlyActor->SetFlyConstraintLine(this->M_FlyCondition.M_bConstraintLine);
		return  _FlyActor;
	}

	return nullptr;
}

void AFlyItemActor::UpdateRatation360(float _deltaTime, USceneComponent* _Comp)
{

	if (_Comp->GetComponentRotation().Pitch >= 360.f)
		_Comp->SetRelativeRotation(FRotator(0.f, 90.f, 0.f));

	_Comp->AddLocalRotation(FRotator(1.f * _deltaTime, 90.f, 0.f));

}

void AFlyItemActor::HitMouse_OverlapBegin(AActor* _Mouse)
{
	//老鼠对象
	if (AMouseActor* Mouse = Cast<AMouseActor>(_Mouse))
	{

		//判断是（约束本行）并且 当前行和老鼠行不匹配
		if (this->GetFlyConstraintLine() && Mouse->GetMouseLine().Row != this->GetLine())
			return;

		//开启范围伤害并且开启了静态对象
		if (this->M_FlyCondition.M_bAOE && this->M_FlyCondition.M_bStaticFlyItem)
		{
			if (UFVMGameInstance::GetDebug())
				UE_LOG(LogTemp, Error, TEXT("%s 当前静态飞行物开启了AOE范围伤害"), *FString(__FUNCTION__));
		}
		else {
			//当前用于老鼠对象则跳出
			if (this->M_CurrentHitMouseActor)
				return;
		}

		bool _bHit = false;

		//判断是否可以击中当前老鼠
		for (auto& Type : this->GetAttackType())
		{
			if (Mouse->GetMouseLineType() == Type)
			{
				_bHit = true;
				break;
			}
		}

		if (Mouse->GetMouseLineType() == ELineType::All)
		{
			_bHit = true;
		}

		if (_bHit)
		{
			//设置当前击中的老鼠
			this->M_CurrentHitMouseActor = Mouse;
			//飞行物击中
			this->Hit();
			//设置老鼠状态->被击中
			Mouse->SetbIsHurt(true);
			//被命中传入伤害数值
			Mouse->BeHit(this->GetCurATK());
			//解析Buff信息
			Mouse->ParseBuff_Information(this->M_FItem_Buff);

			if (UFVMGameInstance::GetDebug())
				UE_LOG(LogTemp, Error, TEXT("[FlyItemActor.cpp AFlyItemActor::HitMouseOverlapBegin]子弹击中老鼠造成伤害 %.2f"), this->GetCurATK());
		}
	}
}

void AFlyItemActor::OnBoxOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//与老鼠重叠
	this->HitMouse_OverlapBegin(OtherActor);
}

void AFlyItemActor::OnBoxOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	//UGameSystemFunction::Log(FString(__FUNCTION__), );
		//开启范围伤害并且开启了静态对象
}

void AFlyItemActor::AddBuff(TMap<Buff_Infor, float>& _buffers)
{
	this->M_FItem_Buff.M_bCondition = true;
	this->M_FItem_Buff.M_Buffers.Append(_buffers);
}

void AFlyItemActor::AddBuff(Buff_Infor& _buff, const float& _time)
{
	this->M_FItem_Buff.M_bCondition = true;
	this->M_FItem_Buff.M_Buffers.Emplace(_buff, _time);
}

void AFlyItemActor::SetMouseActorLocation(AActor* _MouseActor)
{
	this->M_MouseActorLocation = _MouseActor;
}

void AFlyItemActor::AddAttackType(ELineType _type)
{
	this->M_AttackType.Emplace(_type);
}

void AFlyItemActor::SetTargetHitState(bool _Value)
{
	this->M_FlyCondition.M_bHitTarget = _Value;
}

void AFlyItemActor::AddAttackType(TArray<ELineType>& _types)
{
	this->M_AttackType.Append(_types);
}
