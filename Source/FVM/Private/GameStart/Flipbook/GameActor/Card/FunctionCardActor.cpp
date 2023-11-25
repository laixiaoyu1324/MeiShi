// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除


#include "GameStart/Flipbook/GameActor/Card/FunctionCardActor.h"
#include "GameStart/VS/Components/Card/CardFunctionComponent.h"
#include <Components/BoxComponent.h>

const FItemCardFunction& AFunctionCardActor::GetFunctionCardData()
{
	return this->ItemCardFunction;
}

UBoxComponent* AFunctionCardActor::GetOverlapBoxComponent()
{
	return this->OverlapBoxComponent;
}

void AFunctionCardActor::SetOverlapBoxComponent(UBoxComponent* Box)
{
	this->OverlapBoxComponent = Box;

	if (IsValid(this->OverlapBoxComponent))
	{
		//设置卡片碰撞类型
		this->OverlapBoxComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		this->OverlapBoxComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
		this->OverlapBoxComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
	}
}

void AFunctionCardActor::BeginPlay()
{
	this->ItemCardFunction = this->GetCardDataComponent()->GetCardFunction(this->CardActor_Name);

	//强化提升生命值
	if (this->ItemCardFunction.M_ECardUpGradeType == ECardUpGradeType::E_Defence_Up)
	{
		//强化提升生命值
		float HP = this->ItemCardFunction.M_CardHP;

		HP = this->ItemCardFunction.M_CardHP +
			this->ItemCardFunction.M_CardHP * this->ItemCardFunction.M_M_ECardUpGradeUpRate *
			this->GetCardGrade(this->ItemCardFunction.ItemName);

		//写入血量数据
		this->ItemCardFunction.M_CardHP = HP;

		//重新设置卡片生命值
		this->SetCardHP(this->ItemCardFunction.M_CardHP, 0.f);
	}


	//兼容老框架
	this->SetCardActor(this->ItemCardFunction);

	Super::BeginPlay();
}

void AFunctionCardActor::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

}
