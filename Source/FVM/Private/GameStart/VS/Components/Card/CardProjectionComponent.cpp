// Fill out your copyright notice in the Description page of Project Settings.


#include "GameStart/VS/Components/Card/CardProjectionComponent.h"

#include "GameStart/Flipbook/GameActor/FlyItemActor.h"
#include "GameStart/VS/Components/MesheControllComponent.h"
#include "GameStart/VS/GameMapInstance.h"

#include <Paper2D/Classes/PaperSprite.h>
#include <Paper2D/Classes/PaperFlipbook.h>
#include <Paper2D/Classes/PaperFlipbookComponent.h>

#include <Kismet/GameplayStatics.h>
#include <Kismet/KismetSystemLibrary.h>
// Sets default values for this component's properties
UCardProjectionComponent::UCardProjectionComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

void UCardProjectionComponent::LoadResource()
{
	Super::LoadResource();

	this->M_Owner = Cast<AOld_ATKCardActor>(this->GetOwner());

	if (!this->M_Owner)
		return;

	int32 ResourcePathCount = this->M_Owner->M_Resource.Num();

	if (ResourcePathCount > 0)
	{
		if (!this->M_Owner->M_Resource[0].Get())
		{
			this->M_Owner->M_Resource[0].LoadSynchronous();
		}

		this->M_UPaperFlipbook_Idle = this->M_Owner->M_Resource[0].Get();
	}

	if (ResourcePathCount > 1)
	{
		if (!this->M_Owner->M_Resource[1].Get())
		{
			this->M_Owner->M_Resource[1].LoadSynchronous();
		}

		this->M_UPaperFlipbook_Attack = this->M_Owner->M_Resource[1].Get();
	}


	if (this->M_Owner->M_BulletResource.Num() >= 1)
	{
		if (!this->M_Owner->M_BulletResource[0].Get())
		{
			this->M_Owner->M_BulletResource[0].LoadSynchronous();
		}

		this->M_FlyItemActor = this->M_Owner->M_BulletResource[0].Get();
	}

	if (!this->M_FlyItemActor)
	{
		UE_LOG(LogTemp, Error, TEXT("[CardProjectionComponent.cpp UCardProjectionComponent::LoadResource]卡片投射物加载失败!请检查路径和末尾的\"_C\""));
		return;
	}

	this->M_Owner->GetRenderComponent()->SetFlipbook(this->M_UPaperFlipbook_Idle);

	//获取攻击速度并且设置值
	this->InitLaunchProperty(this->M_Owner->GetCardAttackItemCount(), this->M_Owner->GetCardAttackSpeed(), this->M_Owner->GetCardFirstAttackSpeed(), this->M_Owner->GetCardAttackProjectionSpeed());
}

void UCardProjectionComponent::Spawn()
{
	Super::Spawn();

	//当前老鼠无效
	if (!this->M_Owner->GetCurrentMouse())
		return;

	//获取对象的变换位置
	const FTransform& _trans = this->M_Owner->GetActorTransform();
	FTransform NewTrans;
	NewTrans.SetLocation(_trans.GetLocation());

	AFlyItemActor* _TargetActor = Cast<AFlyItemActor>(
		UGameplayStatics::BeginDeferredActorSpawnFromClass(this->GetWorld(), this->M_FlyItemActor, NewTrans)
		);

	//新生成的对象设置自定义拥有者(CardActor)
	_TargetActor->M_CustomActorOwner = this->M_Owner;
	_TargetActor->SetMouseActorLocation(this->M_Owner->GetCurrentMouse());
	_TargetActor->SetATK(this->M_Owner->GetCardATK());
	_TargetActor->SetSecondATK(this->M_Owner->GetCardSecondATK());
	_TargetActor->SetFlipbookRotation(FRotator(0.f, 90.f, 0.f));
	_TargetActor->SetLine(this->M_Owner->GetLine().Row);
	_TargetActor->SetFloatModeEnable(this->M_Owner->GetFloatMode());
	Cast<AFlyItemActor>(UGameplayStatics::FinishSpawningActor(_TargetActor, NewTrans));
}

void UCardProjectionComponent::PlayAttackAnimation()
{
	Super::PlayAttackAnimation();

	this->M_Owner->SetPlayAnimation(this->M_UPaperFlipbook_Attack);

	//提示
	//UE_LOG(LogTemp, Error, TEXT("[CardProjectionComponent.cpp UCardProjectionComponent::PlayAttackAnimation]:切换攻击动画"));
}

void UCardProjectionComponent::PlayIdleAnimation()
{
	Super::PlayIdleAnimation();

	this->M_Owner->SetPlayAnimation(this->M_UPaperFlipbook_Idle);

	//提示
	//UE_LOG(LogTemp, Error, TEXT("[CardProjectionComponent.cpp UCardProjectionComponent::PlayIdleAnimation]:切换默认动画"));
}


// Called when the game starts
void UCardProjectionComponent::BeginPlay()
{
	Super::BeginPlay();

	this->M_Owner = Cast<AOld_ATKCardActor>(this->GetOwner());

	if (!this->M_Owner)
	{
		UE_LOG(LogTemp, Error, TEXT("获取卡片Actor失败!"));
		this->SetComponentTickEnabled(false);
		return;
	}
}

// Called every frame
void UCardProjectionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (FVM_VS_GamePause())
		return;

	if (!this->M_Owner)
		return;

	//卡片休息
	if (!this->M_Owner->GetCardDay() && this->M_Owner->GetMapDay())
		return;

	bool LResult = false;

	for (const auto& Line : this->M_Owner->GetLineTraceSetting())
	{
		DrawDebugLine(this->GetWorld(), Line.M_BeginLocation, Line.M_EndLocation, FColor::Red);

		this->M_Trace_Target = ECC_Visibility;

		switch (Line.M_ELineTraceType)
		{
		case ELineTraceType::E_MouseGround:this->M_Trace_Target = ECC_GameTraceChannel2; break;
		case ELineTraceType::E_MouseSky:this->M_Trace_Target = ECC_GameTraceChannel3; break;
		case ELineTraceType::E_MouseUnder:this->M_Trace_Target = ECC_GameTraceChannel4; break;
		}

		if (this->GetWorld()->LineTraceSingleByChannel(this->M_Trace_Result,
			Line.M_BeginLocation,
			Line.M_EndLocation,
			this->M_Trace_Target,
			this->M_Trace_Params,
			this->M_Trace_ResParams))
		{
			this->M_Owner->AnalysisActor(this->M_Trace_Result.GetActor());

			LResult = true;
		}
	}

	//所有的线扫描结束之后结果是false，则关闭当前攻击对象
	if (!LResult)
		this->M_Owner->SetCurrentMouse(nullptr);

	//如果老鼠存在
	if (this->M_Owner->GetCurrentMouse() && !this->M_Owner->GetCurrentMouse()->IsPendingKillPending())
	{
		this->SetAttackModEnabled(true);
	}
	else {
		this->SetAttackModEnabled(false);
	}
}

