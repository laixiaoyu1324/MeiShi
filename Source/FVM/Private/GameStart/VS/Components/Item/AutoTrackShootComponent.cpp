// Fill out your copyright notice in the Description page of Project Settings.


#include "GameStart/VS/Components/Item/AutoTrackShootComponent.h"
#include "GameStart/Flipbook/GameActor/MouseActor.h"
#include "GameStart/Flipbook/GameActor/FlyItemActor.h"
#include "GameStart/VS/Components/MouseManagerComponent.h"
#include "GameStart/VS/GameMapInstance.h"
#include <Kismet/KismetMathLibrary.h>

// Sets default values for this component's properties
UAutoTrackShootComponent::UAutoTrackShootComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UAutoTrackShootComponent::BeginPlay()
{
	Super::BeginPlay();
	//初始化飞行物
	this->M_Owner = Cast<AFlyItemActor>(this->GetOwner());
	if (this->M_Owner)
	{
		// 初始化老鼠管理器
		this->M_MouseManager = AGameMapInstance::GetGameMapInstance()->M_MouseManagerComponent;
		//获取坐标
		this->M_OwnerLocation = this->M_Owner->GetActorLocation();
	}
	else {
		this->SetComponentTickEnabled(false);
		UE_LOG(LogTemp, Error, TEXT("[AutoTrackShootComponent.cpp UAutoTrackShootComponent::BeginPlay]飞行物Actor获取失败!"));
	}



}


// Called every frame
void UAutoTrackShootComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (bStop)
	{
		return;
	}

	// ...
	this->_TickTime += DeltaTime;
	if (this->_TickTime > 0.01f)
	{
		this->_TickTime = 0.f;

		AMouseActor* const CurTop = this->M_MouseManager->GetTopFirstMouse();

		if ((!IsValid(CurTop)) || (IsValid(CurTop) && CurTop->GetCurrentHP() <= 0.f))
		{
			_AutoTrackTickTime = 0.f;
			this->M_Owner->AddActorLocalOffset(FVector(0.f, DeltaTime * 85.f, 0.f));
			this->M_OwnerLocation = this->M_Owner->GetActorLocation();
			return;
		}

		if (CurTop != this->M_CurrentMouse)
		{
			this->M_CurrentMouse = CurTop;
		}

		if (this->M_CurrentMouse->GetCurrentHP() > 0.f)
		{
			//时间
			_AutoTrackTickTime += DeltaTime * 0.05f;

			//朝向
			const FRotator& __Angle = UKismetMathLibrary::FindLookAtRotation(
				this->M_OwnerLocation,
				this->M_CurrentMouse->GetActorLocation()
			);

			//追踪
			const FVector& _V = UKismetMathLibrary::VLerp(
				this->M_OwnerLocation,
				this->M_CurrentMouse->GetActorLocation() +
				FVector(0.f, DeltaTime * 3.5f, 0.f), _AutoTrackTickTime
			);

			//设置坐标
			this->M_Owner->SetActorLocation(_V);

			//更新旋转（飞行物图片显示旋转）
			this->M_Owner->SetFlipbookPitchRotation(__Angle.Roll);

			//重置时间
			if (_AutoTrackTickTime >= 0.5f)
			{
				_AutoTrackTickTime = 0.f;
			}
		}

		this->M_OwnerLocation = this->M_Owner->GetActorLocation();
	}
}

void UAutoTrackShootComponent::Stop()
{
	this->bStop = true;
}

