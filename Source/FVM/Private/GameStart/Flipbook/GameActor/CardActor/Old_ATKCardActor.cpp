// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除


#include "GameStart/Flipbook/GameActor/CardActor/Old_ATKCardActor.h"


void AOld_ATKCardActor::BeginPlay()
{
	Super::BeginPlay();
}

void AOld_ATKCardActor::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);


	//计算射线位置
	if (this->GetFloatMode() && this->M_FCardActor_ATK.M_bATK)
	{
		this->M_FCardActor_ATK.M_LineTraceSettings.Empty();

		for (const FLineTraceSetting& Settings : this->M_FCardActor_ATK.M_SourceLineTraceSettings)
		{
			UGameSystemFunction::CalculateLineTracePosition(this->GetActorLocation(), this->GetLine(), this->GetUIMapMesh(), Settings, this->M_FCardActor_ATK.M_LineTraceSettings);
		}
	}
}

void AOld_ATKCardActor::SetCardData_ATK(const FItemCardATK& _ATK_Data)
{
	//_BaseATK + (_BaseATK / 2.f) * _ATKRate * ((float)(_CardGrade));

	this->ItemCardATK = _ATK_Data;

	//设置了ATK->表示当前卡片是攻击类型卡片
	this->M_FCardActor_ATK.M_bATK = true;

	//设置基础攻击力
	float LCurATK = _ATK_Data.M_CardATKBase;

	//设置射线（用于板块移动时，实时计算设置的位置）
	this->M_FCardActor_ATK.M_SourceLineTraceSettings = _ATK_Data.M_LineTraceSettings;

	//计算射线位置
	for (const FLineTraceSetting& Settings : _ATK_Data.M_LineTraceSettings)
	{
		UGameSystemFunction::CalculateLineTracePosition(this->GetActorLocation(), this->GetLine(), this->GetUIMapMesh(), Settings, this->M_FCardActor_ATK.M_LineTraceSettings);
	}

	if (UFVMGameInstance::GetDebug())
		UE_LOG(LogTemp, Error, TEXT("[CardActor.cpp AOld_ATKCardActor::SetCardData_ATK]攻击卡片属性: [基础攻击]%.2f  [攻击倍率]%.2f  [二次伤害]%.2f "), _ATK_Data.M_CardATKBase, _ATK_Data.M_CardATKRate, _ATK_Data.M_SputteringATKRate);

	//判断是否有攻击力提升功能
	if (_ATK_Data.M_ECardUpGradeType == ECardUpGradeType::E_Atk_Up)
	{
		//this->SetCardATK(_ATK_Data.M_CardATKBase, _ATK_Data.M_CardATKRate, this->GetCardGrade(_ATK_Data.ItemName));

		//攻击力计算
		LCurATK = _ATK_Data.M_CardATKBase + (_ATK_Data.M_CardATKBase / 2.f) * _ATK_Data.M_CardATKRate * ((float)(this->GetCardGrade(_ATK_Data.ItemName))) * _ATK_Data.M_M_ECardUpGradeUpRate;

		if (UFVMGameInstance::GetDebug())
			UE_LOG(LogTemp, Error, TEXT("[CardActor.cpp AOld_ATKCardActor::SetCardData_ATK]攻击卡片强化技能:|| [最终攻击力](%.2f) || 计算:[BaseATK (%.2f)] + [BaseATK / 2 (%.2f)] * [ATKRate (%.2f)] * [Grade (%d)] * [UpGradeRate (%.2f)]"), LCurATK, _ATK_Data.M_CardATKBase, (_ATK_Data.M_CardATKBase / 2.f), _ATK_Data.M_CardATKRate, this->GetCardGrade(_ATK_Data.ItemName), _ATK_Data.M_M_ECardUpGradeUpRate);
	}

	//设置最终攻击力
	this->SetCardATK(LCurATK, _ATK_Data.M_CardATKRate, this->GetCardGrade(_ATK_Data.ItemName));
	this->SetCardSecondATK(_ATK_Data.M_SputteringATKRate);

	//攻击速度提升  //设置攻击速度
	if (_ATK_Data.M_ECardSkillType == ECardSkillType::E_ATTACKSPEED)
	{
		this->SetCardAttackSpeed(_ATK_Data.M_ShootSpeed - _ATK_Data.M_ShootSpeed * UGameSystemFunction::GetPlayerSkillBooksLv(_ATK_Data.M_CardSkillBookName) * 0.1f);

		if (UFVMGameInstance::GetDebug())
		{
			FString Content = FString(TEXT("当前防御卡[") + _ATK_Data.ItemName + TEXT("]提升攻击速度=>") + FString::SanitizeFloat(this->GetCardAttackSpeed()));
			UGameSystemFunction::FVMLog(__FUNCTION__, Content);
		}
	}
	else {
		this->SetCardAttackSpeed(_ATK_Data.M_ShootSpeed);
	}

	this->SetCardFirstAttackSpeed(_ATK_Data.M_FirstShootSpeed);
	this->SetCardAttackProjectionSpeed(_ATK_Data.M_ShootDelay);
	this->SetCardAttackItemCount(_ATK_Data.M_ProjectionItemCount);

	this->SetCardActor(_ATK_Data);
}

void AOld_ATKCardActor::SetCardATK(const float& _BaseATK, const float& _ATKRate, const int32& _CardGrade)
{
	//后面两个是 技能书  真爱谱  神食谱 + 0.f + 0.f + 0.f;

	this->M_FCardActor_ATK.M_ATK = _BaseATK;// +(_BaseATK / 2.f) * _ATKRate * ((float)(_CardGrade));
	//UE_LOG(LogTemp, Error, TEXT("当前卡片攻击力:%.2f;基础攻击力%.2f;加成比率%.2f;等级%d"), this->M_ATK, _BaseATK, _ATKRate, _CardGrade);
}

float AOld_ATKCardActor::GetCardATK()
{
	return this->M_FCardActor_ATK.M_ATK;
}

void AOld_ATKCardActor::SetCardSecondATK(const float& _ATKRate)
{
	this->M_FCardActor_ATK.M_SecondATK = M_FCardActor_ATK.M_ATK * _ATKRate;
}

float AOld_ATKCardActor::GetCardSecondATK()
{
	return this->M_FCardActor_ATK.M_SecondATK;
}

void AOld_ATKCardActor::SetCardAttackSpeed(float _Value)
{
	this->M_FCardActor_ATK.M_ShootSpeed = _Value;
}

float AOld_ATKCardActor::GetCardAttackSpeed()
{
	return this->M_FCardActor_ATK.M_ShootSpeed;
}

void AOld_ATKCardActor::SetCardFirstAttackSpeed(float _Value)
{
	this->M_FCardActor_ATK.M_FirstShootSpeed = _Value;
}

float AOld_ATKCardActor::GetCardFirstAttackSpeed()
{
	return this->M_FCardActor_ATK.M_FirstShootSpeed;
}

void AOld_ATKCardActor::SetCardAttackProjectionSpeed(float _Value)
{
	this->M_FCardActor_ATK.M_ShootDelay = _Value;
}

float AOld_ATKCardActor::GetCardAttackProjectionSpeed()
{
	return this->M_FCardActor_ATK.M_ShootDelay;
}

void AOld_ATKCardActor::SetCardAttackItemCount(int32 _Value)
{
	this->M_FCardActor_ATK.M_ProjectionItemCount = _Value;
}

int32 AOld_ATKCardActor::GetCardAttackItemCount()
{
	return this->M_FCardActor_ATK.M_ProjectionItemCount;
}

const TArray<FLineTraceSetting>& AOld_ATKCardActor::GetLineTraceSetting()
{
	return this->M_FCardActor_ATK.M_LineTraceSettings;
}
