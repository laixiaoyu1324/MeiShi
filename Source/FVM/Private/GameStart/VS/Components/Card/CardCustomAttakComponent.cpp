// Fill out your copyright notice in the Description page of Project Settings.


#include "GameStart/VS/Components/Card/CardCustomAttakComponent.h"
#include "GameStart/VS/Components/MouseManagerComponent.h"
#include "GameStart/VS/GameMapInstance.h"
#include "GameStart/Flipbook/GameActor/CardActor/Old_ATKCardActor.h"
#include "GameStart/Flipbook/GameActor/FlyItemActor.h"
#include "Paper2D/Classes/PaperFlipbookComponent.h"

// Sets default values for this component's properties
UCardCustomAttakComponent::UCardCustomAttakComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UCardCustomAttakComponent::BeginPlay()
{
	Super::BeginPlay();

	this->M_CardActor = Cast<AOld_ATKCardActor>(this->GetFlipbookActor());
	if (!this->M_CardActor)
	{
		this->SetComponentTickEnabled(false);
	}

	//this->M_CardActor->GetMouseLineNotify();
}

void UCardCustomAttakComponent::LoadResource()
{
	Super::LoadResource();

	if (!this->M_CardActor->M_Resource[0].Get())
	{
		this->M_CardActor->M_Resource[0].LoadSynchronous();
	}

	if (!this->M_CardActor->M_Resource[1].Get())
	{
		this->M_CardActor->M_Resource[1].LoadSynchronous();
	}

	if (!this->M_CardActor->M_BulletResource[0].Get())
	{
		this->M_CardActor->M_BulletResource[0].LoadSynchronous();
	}

	this->M_Idle = this->M_CardActor->M_Resource[0].Get();
	this->M_Attack = this->M_CardActor->M_Resource[1].Get();
	TSubclassOf<AFlyItemActor> _FlyItem = this->M_CardActor->M_BulletResource[0].Get();
	this->M_AFlyItemActor = _FlyItem.GetDefaultObject();

	if (!this->M_AFlyItemActor)
	{
		UE_LOG(LogTemp, Error, TEXT("[CardCustomAttakComponent.cpp UCardCustomAttakComponent::LoadResource] 飞行物加载失败，请检查路径 或者 后缀_C"));
	}

	//设置攻击投射属性
	this->InitLaunchProperty(this->M_CardActor->GetCardAttackItemCount(), this->M_CardActor->GetCardAttackSpeed(), this->M_CardActor->GetCardFirstAttackSpeed(), this->M_CardActor->GetCardAttackProjectionSpeed());
}

void UCardCustomAttakComponent::Spawn()
{
	Super::Spawn();

	//获取对象的变换位置
	const FTransform& _trans = this->M_CardActor->GetActorTransform();
	FTransform NewTrans;
	NewTrans.SetLocation(_trans.GetLocation());

	AFlyItemActor* _TargetActor = Cast<AFlyItemActor>(UGameplayStatics::BeginDeferredActorSpawnFromClass(this->GetWorld(), this->M_AFlyItemActor->GetClass(), NewTrans));
	//新生成的对象设置自定义拥有者(CardActor)
	_TargetActor->M_CustomActorOwner = this->M_CardActor;
	_TargetActor->SetMouseActorLocation(this->M_CardActor->GetCurrentMouse());
	_TargetActor->SetATK(this->M_CardActor->GetCardATK());
	_TargetActor->SetSecondATK(this->M_CardActor->GetCardSecondATK());
	_TargetActor->SetFlipbookRotation(FRotator(0.f, 90.f, 0.f));
	_TargetActor->SetLine(this->M_CardActor->GetLine().Row);
	UGameplayStatics::FinishSpawningActor(_TargetActor, NewTrans);

}

void UCardCustomAttakComponent::PlayAttackAnimation()
{
	Super::PlayAttackAnimation();


	this->M_CardActor->GetRenderComponent()->SetFlipbook(this->M_Attack);
}

void UCardCustomAttakComponent::PlayIdleAnimation()
{
	Super::PlayIdleAnimation();

	this->M_CardActor->GetRenderComponent()->SetFlipbook(this->M_Idle);
}


// Called every frame
void UCardCustomAttakComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	//判断游戏是否暂停（游戏暂停则不能攻击）
	if (this->M_CardActor->GamePause())
		return;

	//每0.2m判断一次
	_tick_count += DeltaTime;
	if (_tick_count > 0.2f)
	{
		_tick_count = 0.f;
		this->SetAttackModEnabled(AGameMapInstance::GetGameMapInstance()->M_MouseManagerComponent->IsMouseExist());
	}
}

