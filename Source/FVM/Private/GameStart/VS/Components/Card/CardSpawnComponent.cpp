// Fill out your copyright notice in the Description page of Project Settings.


#include "GameStart/VS/Components/Card/CardSpawnComponent.h"
#include "GameStart/Flipbook/GameActor/Card/SpawnCardActor.h"
#include "GameStart/Flipbook/GameActor/FlameActor.h"
#include "GameStart/VS/Components/ResourceManagerComponent.h"
#include <Paper2D/Classes/PaperSprite.h>
#include <Paper2D/Classes/PaperFlipbook.h>
#include <Paper2D/Classes/PaperFlipbookComponent.h>
#include "GameStart/VS/GameMapInstance.h"
#include "GameSystem/FVMGameInstance.h"
#include <Kismet/GameplayStatics.h>
// Sets default values for this component's properties
UCardSpawnComponent::UCardSpawnComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

void UCardSpawnComponent::LoadResource()
{
	Super::LoadResource();

	if (!this->CurSpawnCardActor)
	{
		return;
	}

	//加载生产卡片的所有动画
	if (!this->CurSpawnCardActor->CardActor_DefAnim1.Get())
	{
		this->CurSpawnCardActor->CardActor_DefAnim1.LoadSynchronous();
	}
	if (!this->CurSpawnCardActor->CardActor_DefAnim2.Get())
	{
		this->CurSpawnCardActor->CardActor_DefAnim2.LoadSynchronous();
	}
	if (!this->CurSpawnCardActor->CardActor_SpawnAnim1.Get())
	{
		this->CurSpawnCardActor->CardActor_SpawnAnim1.LoadSynchronous();
	}
	if (!this->CurSpawnCardActor->CardActor_SpawnAnim2.Get())
	{
		this->CurSpawnCardActor->CardActor_SpawnAnim2.LoadSynchronous();
	}
	if (!this->CurSpawnCardActor->CardActor_UpAnim.Get())
	{
		this->CurSpawnCardActor->CardActor_UpAnim.LoadSynchronous();
	}

	this->M_Resource.M_UPaperFlipbook_Idle = this->CurSpawnCardActor->CardActor_DefAnim1.Get();
	this->M_Resource.M_UPaperFlipbook_Spawn = this->CurSpawnCardActor->CardActor_SpawnAnim1.Get();
	this->M_Resource.M_UPaperFlipbook_Spawn_Up = this->CurSpawnCardActor->CardActor_UpAnim.Get();

	//播放默认动画
	this->CurSpawnCardActor->GetRenderComponent()->SetFlipbook(this->CurSpawnCardActor->CardActor_DefAnim1.Get());


	//初始化基础数据以及生成器
	const FItemCardSpawn& CurData = this->CurSpawnCardActor->GetSpawnCardData();
	//设置生产量
	this->M_SpawnFlameValue = CurData.M_FlameNum;
	//设置卡片生长
	this->M_SpawnCondition.M_CurrentCardIsWeekUp = CurData.M_EnableWeekUp;
	//设置卡片生长时间
	this->M_SpawnCondition.M_AutoUpTime = CurData.M_WeekUpTime;
	//当生长完毕时，当前火苗提升的倍数
	this->M_SpawnCondition.M_SpawnFlameNumRate = CurData.M_SpawnFlameNumRate;
	//初始化生成器
	this->InitLaunchProperty(
		//生产个数
		CurData.M_SpawnNum,
		//生产速度
		CurData.M_SpawnSpeed,
		//生产延迟【等待生产动画播放完毕的时间】
		CurData.M_FirstSpawnDelay == 0.f ? this->CurSpawnCardActor->CardActor_DefAnim1.Get()->GetTotalDuration() : CurData.M_FirstSpawnDelay,
		//生产多个时，每一个的延迟
		0.f
	);

	this->M_SpawnFlameValue = CurData.M_FlameNum;
}


// Called when the game starts
void UCardSpawnComponent::BeginPlay()
{
	Super::BeginPlay();

	this->CurSpawnCardActor = Cast<ASpawnCardActor>(this->GetOwner());

	if (!this->CurSpawnCardActor)
	{
		UE_LOG(LogTemp, Error, TEXT("获取生产卡片Actor失败!"));
		this->SetComponentTickEnabled(false);
		return;
	}
}

void UCardSpawnComponent::Spawn()
{
	Super::Spawn();

	//获取火苗
	AFlameActor* CurFlame = UResourceManagerComponent::GetFlame();

	if (IsValid(CurFlame))
	{
		
		if (this->M_SpawnFlameValue < 25)
		{
			CurFlame->SetActorScale3D(FVector(0.7f));
		}
		else {
			CurFlame->SetActorScale3D(FVector(1.f));
		}

		CurFlame->SetActorLocation(this->CurSpawnCardActor->GetActorLocation());
		CurFlame->SetFlameValue(this->M_SpawnFlameValue);
		CurFlame->Run();
	}
	else {
		UResourceManagerComponent::GetResourceManagerComponent()->AddFlameNum(this->M_SpawnFlameValue);
	}
}

void UCardSpawnComponent::PlayAttackAnimation()
{
	Super::PlayAttackAnimation();

	this->CurSpawnCardActor->SetPlayAnimation(this->M_Resource.M_UPaperFlipbook_Spawn);
}

void UCardSpawnComponent::PlayIdleAnimation()
{
	Super::PlayIdleAnimation();

	this->CurSpawnCardActor->SetPlayAnimation(this->M_Resource.M_UPaperFlipbook_Idle);
}



// Called every frame
void UCardSpawnComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	//游戏暂停则不在生产
	if (this->CurSpawnCardActor->GamePause())
		return;

	//卡片休息
	if (!this->CurSpawnCardActor->GetCardDay() && this->CurSpawnCardActor->GetMapDay())
		return;

	//卡片生命值不为0
	if (this->CurSpawnCardActor->GetCurrentHP() > 0.f)
	{
		//自动生长时间计时
		if (this->M_SpawnCondition.M_CurrentCardIsWeekUp)
		{
			this->M_SpawnCondition.M_AutoUpCurrentTime += DeltaTime;
			if (this->M_SpawnCondition.M_AutoUpCurrentTime >= this->M_SpawnCondition.M_AutoUpTime)
			{
				this->M_SpawnCondition.M_CurrentCardIsWeekUp = false;

				//开启生长模式
				this->InWeekUp = true;
				//设置属性
				this->M_SpawnFlameValue = this->M_SpawnFlameValue * this->M_SpawnCondition.M_SpawnFlameNumRate;
				//播放生长动画
				this->CurSpawnCardActor->SetPlayAnimation(this->M_Resource.M_UPaperFlipbook_Spawn_Up);

				/*---动画重新赋值---*/
				//默认动画
				this->M_Resource.M_UPaperFlipbook_Idle = this->CurSpawnCardActor->CardActor_DefAnim2.Get();
				//生产动画
				this->M_Resource.M_UPaperFlipbook_Spawn = this->CurSpawnCardActor->CardActor_SpawnAnim2.Get();
				//初始化生产
				this->InitCondition();
				//播放生长音效
				UFVMGameInstance::PlayBGM_S_Static("WeekUp", "ItemAudio");
			}
		}

		//生长模式关闭状态
		if (!this->InWeekUp)
		{
			this->SetAttackModEnabled(true);
		}
	}
}

void UCardSpawnComponent::OnAnimationPlayEnd()
{
	Super::OnAnimationPlayEnd();


	//游戏暂停则不在生产
	if (this->CurSpawnCardActor->GamePause())
		return;

	//卡片休息
	if (!this->CurSpawnCardActor->GetCardDay() && this->CurSpawnCardActor->GetMapDay())
		return;

	//卡片生命值不为0
	if (this->CurSpawnCardActor->GetCurrentHP() > 0.f)
	{
		//进入生长模式
		if (this->InWeekUp)
		{
			//当生长动画播放完毕之后
			this->InWeekUp = false;
			this->CurSpawnCardActor->SetPlayAnimation(this->M_Resource.M_UPaperFlipbook_Idle);
		}
	}
}
