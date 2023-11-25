// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除


#include "GameStart/VS/Components/Card/CardAttackComponent.h"

#include "GameStart/Flipbook/GameActor/CardActor/AttackCardActor.h"
#include "GameStart/Flipbook/GameActor/FlyItemActor.h"

#include "GameStart/VS/MapMeshe.h"
#include "GameStart/VS/GameMapInstance.h"
#include "GameStart/VS/Components/MesheControllComponent.h"

#include <Paper2D/Classes/PaperSprite.h>
#include <Paper2D/Classes/PaperFlipbook.h>
#include <Paper2D/Classes/PaperFlipbookComponent.h>

#include <Kismet/GameplayStatics.h>
#include <Kismet/KismetSystemLibrary.h>

void UCardAttackComponent::BeginPlay()
{
	Super::BeginPlay();

	//初始化攻击卡片
	this->AttackCardActor = Cast<AAttackCardActor>(this->GetOwner());

	if (!this->AttackCardActor)
	{
		if (UFVMGameInstance::GetDebug())
		{
			UGameSystemFunction::FVMLog(TEXT("UCardAttackComponent"), TEXT("攻击卡片初始化失败"));
			this->SetComponentTickEnabled(false);
		}
	}



}

void UCardAttackComponent::LoadResource()
{
	Super::LoadResource();

	//初始化默认动画
	if (!this->AttackCardActor->CardActor_DefAnim.Get())
	{
		this->AttackCardActor->CardActor_DefAnim.LoadSynchronous();
	}
	//初始化攻击动画
	if (!this->AttackCardActor->CardActor_AttackAnim.Get())
	{
		this->AttackCardActor->CardActor_AttackAnim.LoadSynchronous();
	}

	//初始化条件
	this->InitLaunchProperty(
		this->AttackCardActor->GetCurrentAttackCount(),
		this->AttackCardActor->GetCurrentAttackSpeed(),
		this->AttackCardActor->GetCurrentFristAttackDelay(),
		this->AttackCardActor->GetCurrentSecondAttackDelay()
	);

	//播放动画
	this->AttackCardActor->SetPlayAnimation(this->AttackCardActor->CardActor_DefAnim.Get());
}

void UCardAttackComponent::Spawn()
{
	Super::Spawn();

	//当前老鼠无效
	if (!IsValid(this->AttackCardActor->GetCurrentMouse()))
	{
		//手动置空
		this->AttackCardActor->SetCurrentMouse(nullptr);
		return;
	}

	//获取对象的变换位置
	const FTransform& _trans = this->AttackCardActor->GetActorTransform();
	FTransform NewTrans;
	NewTrans.SetLocation(_trans.GetLocation());

	if (!this->AttackCardActor->CardActor_BulletClassObj.Get())
	{
		this->AttackCardActor->CardActor_BulletClassObj.LoadSynchronous();
	}

	//生成子弹
	AFlyItemActor* _TargetActor = Cast<AFlyItemActor>(
		UGameplayStatics::BeginDeferredActorSpawnFromClass(this->GetWorld(),
			this->AttackCardActor->CardActor_BulletClassObj.Get(),
			NewTrans));

	//新生成的对象设置自定义拥有者(CardActor)
	_TargetActor->M_CustomActorOwner = this->AttackCardActor;
	_TargetActor->SetMouseActorLocation(this->AttackCardActor->GetCurrentMouse());
	_TargetActor->SetATK(this->AttackCardActor->GetCurrentATK());
	_TargetActor->SetSecondATK(
		this->AttackCardActor->GetCurrentSecondATK(this->AttackCardActor->GetATKCardData().M_SputteringATKRate)
	);
	_TargetActor->SetFlipbookRotation(FRotator(0.f, 90.f, 0.f));
	_TargetActor->SetLine(this->AttackCardActor->GetLine().Row);
	_TargetActor->SetFloatModeEnable(this->AttackCardActor->GetFloatMode());
	Cast<AFlyItemActor>(UGameplayStatics::FinishSpawningActor(_TargetActor, NewTrans));

}

void UCardAttackComponent::PlayAttackAnimation()
{
	Super::PlayAttackAnimation();

	this->AttackCardActor->SetPlayAnimation(this->AttackCardActor->CardActor_AttackAnim.Get());
}

void UCardAttackComponent::PlayIdleAnimation()
{
	Super::PlayIdleAnimation();

	this->AttackCardActor->SetPlayAnimation(this->AttackCardActor->CardActor_DefAnim.Get());
}

UCardAttackComponent::UCardAttackComponent()
{

}

void UCardAttackComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	//老鼠检测
	if (FVM_VS_GamePause())
	{
		return;
	}

	//卡片休息
	if (!this->AttackCardActor->GetCardDay() && this->AttackCardActor->GetMapDay())
	{
		return;
	}

	if (time < 0.4f)
	{
		this->time += DeltaTime;
	}
	else {
		this->time = 0;

		//检测老鼠
		if (this->AttackCardActor->CheckMouse(DeltaTime))
		{
			this->SetAttackModEnabled(true);
		}
		else {
			this->SetAttackModEnabled(false);
		}
	}


}
