// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除


#include "GameStart/Flipbook/GameActor/Mouse/Normal/NormalMouse.h"
#include "GameStart/Flipbook/GameActor/FlyItemActor.h"
#include "GameStart/Flipbook/GameActor/CardActor.h"

#include "GameStart/VS/MapMeshe.h"

#include <Components/BoxComponent.h>

ANormalMouse::ANormalMouse()
{
	this->M_MesheComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("NormalMouseMesheComponent"));
	this->M_BodyComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("NormalMouseBodyComponent"));

	//设置依附
	this->M_MesheComponent->SetupAttachment(this->GetRootComponent());
	this->M_BodyComponent->SetupAttachment(this->M_MesheComponent);
}

void ANormalMouse::BeginPlay()
{
	Super::BeginPlay();

	this->GetRenderComponent()->OnAnimationPlayEnd.BindUObject(this, &ANormalMouse::OnAnimationPlayEnd);

	if (this->M_MouseResource.M_bEnableCreateAnim)
	{
		this->MoveStop();
		this->M_bEnableAttakLine = false;
		this->M_bCreatting = true;

		UPaperFlipbook* Anim1 = LoadObject<UPaperFlipbook>(0,
			*this->GetPath(this->M_MouseResource.M_MousePath, this->M_MouseResource.M_MouseCreateFlipbookName));

		UPaperFlipbook* Anim2 = LoadObject<UPaperFlipbook>(0,
			*this->GetPath(this->M_MouseResource.M_MousePath, this->M_MouseResource.M_MouseNomalFlipbookName));

		if (Anim1 && Anim2)
		{
			this->SetPlayAnimationOnce(Anim1, Anim2);
		}
		else {
			this->M_bEnableAttakLine = true;
			this->M_bCreatting = false;
			this->MoveStart();
		}
	}
	else {
		//开始移动
		this->MoveStart();
	}
}

void ANormalMouse::MoveingUpdate(float DeltaTime)
{
	Super::MoveingUpdate(DeltaTime);

	this->AddActorLocalOffset(FVector(0.f, DeltaTime * -this->M_Proper_State.M_CurrentMoveSpeed, 0.f));
}

bool ANormalMouse::BeHit(float _HurtValue)
{
	if (!Super::BeHit(_HurtValue))
		return false;

	this->UpdateHP(_HurtValue);

	//更新状态
	this->State();

	return true;
}

void ANormalMouse::UnHit()
{
	Super::UnHit();

	//更新状态
	this->State();
}

void ANormalMouse::AttackedBegin()
{
	Super::AttackedBegin();

	//更新状态
	this->State();

}

void ANormalMouse::AttackedEnd()
{
	Super::AttackedEnd();


	//更新状态
	this->State();
}

void ANormalMouse::MouseDeathed()
{
	Super::MouseDeathed();

	//更新状态
	this->State();

	//关闭碰撞
	this->ClosedBoxComponent(this->M_MesheComponent);
	this->ClosedBoxComponent(this->M_BodyComponent);
}

void ANormalMouse::State()
{
	//更新状态[当生命值大于总生命值的40%]
	if (this->GetCurrentHP() > this->GetTotalHP() * 0.4f)
	{
		if (this->GetbIsAttack())
		{
			this->SetPlayAnimation(LoadObject<UPaperFlipbook>(this, *this->GetPath(this->M_MouseResource.M_MousePath, this->M_MouseResource.M_MouseAttackNomalFlipbookName)));
		}
		else
		{
			if (!this->M_bCreatting)
			{
				this->SetPlayAnimation(LoadObject<UPaperFlipbook>(this, *this->GetPath(this->M_MouseResource.M_MousePath, this->M_MouseResource.M_MouseNomalFlipbookName)));
			}
		}
	}//[当生命值小于等于总生命值的40% && 老鼠生命值大于0]
	else if (this->GetCurrentHP() <= this->GetTotalHP() * 0.4f && this->GetCurrentHP() > 0.f)
	{
		if (this->GetbIsAttack())
		{
			this->SetPlayAnimation(LoadObject<UPaperFlipbook>(this, *this->GetPath(this->M_MouseResource.M_MousePath, this->M_MouseResource.M_MouseAttackResidualBloodFlipbookName)));
		}
		else
		{
			if (!this->M_bCreatting)
			{
				this->SetPlayAnimation(LoadObject<UPaperFlipbook>(this, *this->GetPath(this->M_MouseResource.M_MousePath, this->M_MouseResource.M_MouseResidualBloodFlipbookName)));
			}
		}
	}//老鼠死亡
	else {
		this->SetPlayAnimation(LoadObject<UPaperFlipbook>(this, *this->GetPath(this->M_MouseResource.M_MousePath, this->M_MouseResource.M_MouseDeathedFlipbookName)));
	}
}

void ANormalMouse::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (this->M_bEnableAttakLine)
	{
		this->AddAttackLineFunc(UGameSystemFunction::GetCardCollisionBoxType(this->M_ECardCollisionType),DeltaTime);
	}
}

void ANormalMouse::OnAnimationPlayEnd()
{
	if (this->M_bCreatting)
	{
		this->M_bCreatting = false;
		this->M_bEnableAttakLine = true;
		this->MoveStart();
	}

	//如果老鼠处于攻击状态
	if (this->GetbAttackState())
	{
		//攻击防御卡
		if (!this->AttackCard())
		{
			//取消攻击
			this->SetbIsAttack(false);
		}
	}
}
