
#include "GameStart/Flipbook/GameActor/CardActor.h"
#include "GameStart/Flipbook/GameActor/MouseActor.h"

#include "GameStart/VS/GameMapInstance.h"

#include "GameStart/VS/MapMeshe.h"

#include "GameStart/VS/UI/UI_MapMeshe.h"
#include "GameStart/VS/UI/UI_Card.h"

#include <Paper2D/Classes/PaperFlipbook.h>
#include <Paper2D/Classes/PaperFlipbookComponent.h>

#include <Components/BoxComponent.h>
#include "Engine/StaticMesh.h"
#include "Materials/MaterialInstanceDynamic.h"
#include <Components/StaticMeshComponent.h>

#include "GameStart/VS/Components/CardManagerComponent.h"
#include "GameStart/VS/Components/ResourceManagerComponent.h"
#include "GameStart/VS/Components/VSManagerComponent.h"
#include "GameStart/VS/Components/MesheControllComponent.h"

//卡片功能组件
#include "GameStart/VS/Components/Card/CardFunctionComponent.h"
#include "GameStart/VS/GameMapInstance.h"

//玩家背包数据
#include "GameSystem/PlayerStructManager.h"
#include "GameSystem/FVMGameInstance.h"
#include "GameSystem/Tools/GameSystemFunction.h"


void ACardActor::SetCardActor(const FItemCard& _CardData)
{
	this->SetCardHP(_CardData.M_CardHP, 0.f);
	this->SetCardType(_CardData.M_ELineType, _CardData.M_CardLayer);
	this->SetCardGrade(this->GetCardGrade(_CardData.ItemName));
	this->M_CardProperty.M_CardName = _CardData.ItemName;
	this->M_CardProperty.M_CardDay = _CardData.M_CardDay;
	this->M_CardProperty.M_CardID = _CardData.M_CardLayer;
	this->M_SourceCardDataBase = _CardData;
}

UUI_Card* const ACardActor::GetCardUI()
{
	return this->M_UUI_Card;
}

void ACardActor::SetCardUI(UUI_Card* _card_ui)
{
	this->M_UUI_Card = _card_ui;
}

void ACardActor::SetFloatModeEnable(bool _Value)
{
	this->M_bFloatMode = _Value;
}

FLine ACardActor::GetLine()
{
	return this->M_CardProperty.M_Line;
}

int32 ACardActor::GetCardID()
{
	return this->M_CardProperty.M_CardID;
}

const ELineType& ACardActor::GetLineType() const
{
	return this->M_CardProperty.M_ELineType;
}

void ACardActor::SetCardGrade(const int32& _CardGrade)
{
	if (_CardGrade > 3 && _CardGrade < 17)
	{
		UDataTable* CurGradeDataTable = LoadObject<UDataTable>(this,
			TEXT("DataTable'/Game/Resource/BP/Data/CardData/1_CardGradeAnim.1_CardGradeAnim'")
			);

		if (FCard_GradeAnim_Data* CurGradeData = CurGradeDataTable->FindRow<FCard_GradeAnim_Data>(FName(FString::FromInt(_CardGrade)), TEXT("Grade")))
		{
			UStaticMesh* CurStaticMesh = LoadObject<UStaticMesh>(this, TEXT("StaticMesh'/Game/Resource/BP/GameStart/Item/Mesh/Plane.Plane'"));
			this->M_CardGradeStaticMesh->SetStaticMesh(CurStaticMesh);

			UMaterialInstance* CurMaterial = Cast<UMaterialInstance>(CurGradeData->CardGradeResource.TryLoad());
			this->M_CardGradeStaticMesh->SetMaterial(0, CurMaterial);
			this->M_CardGradeStaticMesh->SetRelativeLocation(FVector(CurGradeData->RelativeLocation.X, 0.f, CurGradeData->RelativeLocation.Y));
			this->M_CardGradeStaticMesh->SetWorldScale3D(FVector(CurGradeData->Scale.X, CurGradeData->Scale.Y, 1.f));
		}
	}
	else {
		this->M_CardGradeStaticMesh->SetHiddenInGame(true);
	}
}

void ACardActor::SetLine(const FLine& _Line)
{
	this->M_CardProperty.M_Line = _Line;
}

void ACardActor::SetCardType(const ELineType& _Type, const int32& _CardID)
{
	this->M_CardProperty.M_ELineType = _Type;
	this->M_CardProperty.M_CardID = _CardID;
}

float ACardActor::GetCurrentHP()
{
	return this->M_FCardActor_HP.M_Life;
}

FString ACardActor::GetCardName()
{
	return this->M_CardProperty.M_CardName;
}

float ACardActor::GetTotalHP()
{
	return this->M_FCardActor_HP.M_TotalLife;
}

bool ACardActor::GetFloatMode() const
{
	return this->M_bFloatMode;
}

const FItemCard& ACardActor::GetCardData()
{
	return this->M_SourceCardDataBase;
}

void ACardActor::SetCardHP(const float& _HP, float _HPUpRate)
{
	this->M_FCardActor_HP.M_TotalLife = _HP;
	this->M_FCardActor_HP.M_Life = this->M_FCardActor_HP.M_TotalLife;
}

void ACardActor::SetCardCurrentHP(const float& _HP)
{
	this->M_FCardActor_HP.M_Life = _HP;
}

bool ACardActor::GetMapDay()
{
	return this->M_CardProperty.M_Day;
}

bool ACardActor::GetCardDay()
{
	return this->M_CardProperty.M_CardDay;
}

void ACardActor::CardDayTransform(bool _Day)
{
	this->M_CardProperty.M_CardDay = _Day;
}

bool ACardActor::GamePause()
{
	return this->M_AGameMapInstance->GetGamePause();
}

AMouseActor* ACardActor::GetCurrentMouse()
{
	return this->M_CardProperty.M_CurrentMouse;
}

AMouseActor* ACardActor::GetCurrentAttackSelfMouse()
{
	return this->M_CardProperty.M_CurrentAttackSelfMouse;
}

void ACardActor::SetCurrentMouse(AMouseActor* _MouseActor)
{
	this->M_CardProperty.M_CurrentMouse = _MouseActor;
}

void ACardActor::SetCurrentAttackSelfMouse(AMouseActor* _MouseActor)
{
	this->M_CardProperty.M_CurrentAttackSelfMouse = _MouseActor;
}

void ACardActor::SetTranslucentSortPriority(int32 _Layer)
{
	// 木盘子 -1  0  0,1
	// 小笼包 0   1  2,3
	// 西瓜皮 1   2  4,5

	int32 CurLayer = _Layer - TranslucencyConst::GetLayer(TranslucencyConst::ETranslucency::EMeshe);
	switch ((CurLayer - 1))
	{
	case -1:
		CurLayer = _Layer;
		; break;
	case 0:
		CurLayer = _Layer + 1;
		; break;
	case 1:
		CurLayer = _Layer + 2;
		; break;
	default:
		CurLayer = _Layer + 3;
		break;
	}

	Super::SetTranslucentSortPriority(_Layer);
	this->M_CardGradeStaticMesh->SetTranslucentSortPriority(_Layer + 1);
}

int32 ACardActor::GetCardGrade(const FString& _Name)
{
	if (AGameMapInstance::GetGameMapInstance())
	{
		for (auto Datas : AGameMapInstance::GetGameMapInstance()->M_CardManagerComponent->M_CardData)
		{
			if (Datas.ItemName.Equals(_Name))
			{
				return Datas.M_CardGrade;
			}
		}
	}

	return int32();
}

/*FMouseLineNotify& ACardActor::GetMouseLineNotify()
{
	//UE_LOG(LogTemp, Warning, TEXT("%d  %d"), this->M_Line.Row, this->M_Line.Col);
	return this->M_AGameMapInstance->M_MouseManagerComponent->M_FMouseLineNotify[this->M_CardProperty.M_Line.Row];
}
*/
bool ACardActor::UpdateCardState(const float& _ATK_Value, const float& _UpHP_Value)
{
	if (this->M_FCardActor_HP.M_Life <= 0.f)
	{
		return true;
	}

	if (_UpHP_Value + this->M_FCardActor_HP.M_Life >= this->M_FCardActor_HP.M_TotalLife)
	{
		this->M_FCardActor_HP.M_Life = this->M_FCardActor_HP.M_TotalLife;
	}
	else {
		this->M_FCardActor_HP.M_Life += _UpHP_Value;
	}

	//更新生命值
	this->M_FCardActor_HP.M_Life = this->M_FCardActor_HP.M_Life - _ATK_Value;

	if (UFVMGameInstance::GetDebug())
	{
		FString Content = FString(TEXT("【")) + this->GetCardName() + FString(TEXT("】受到伤害:")) + FString::SanitizeFloat(_ATK_Value) + FString(TEXT("当前生命值:")) + FString::SanitizeFloat(this->M_FCardActor_HP.M_Life);
		UGameSystemFunction::FVMLog(__FUNCTION__, Content);
	}

	if (this->M_FCardActor_HP.M_Life <= 0)
		return true;

	return false;
}

void ACardActor::PrintMouseLineNotify()
{
	//this->M_AGameMapInstance->M_MouseManagerComponent->PrintMouseLineNotify(this->GetMouseLineNotify());
}

void ACardActor::OnCardMovedUpdate(const float& DeltaTime, const int32& _Layer, const FVector& _Location, const FLine& _Line)
{
	this->SetActorLocation(_Location - this->M_OffsetPosition);
	this->SetTranslucentSortPriority(_Layer + this->M_SourceCardDataBase.M_CardLayer + 1);
	this->SetLine(_Line);
}

void ACardActor::OnCardMoveBegin(const int32& _Layer, const FLine& _Line)
{
	this->M_bFloatMode = true;
	this->SetLine(_Line);
	this->SetTranslucentSortPriority(_Layer + this->M_SourceCardDataBase.M_CardLayer + 1);
}

void ACardActor::OnCardMoveEnd(const int32& _Layer, const FLine& _Line)
{
	this->M_bFloatMode = false;
	this->SetLine(_Line);
	this->SetTranslucentSortPriority(_Layer + this->M_SourceCardDataBase.M_CardLayer + 1);
}

void ACardActor::BeginPlay()
{
	Super::BeginPlay();

	this->M_AGameMapInstance = AGameMapInstance::GetGameMapInstance();

	if (!this->M_AGameMapInstance)
	{
		if (UFVMGameInstance::GetDebug())
			UE_LOG(LogTemp, Error, TEXT("[ACardActor::BeginPlay]:游戏地图实例对象获取失败"));
		this->SetActorTickEnabled(false);
		return;
	}

	this->SetActorRotation(FRotator(0.f, 90.f, 0.f));

	//获取白天和夜晚
	this->M_CardProperty.M_Day = this->M_AGameMapInstance->M_ResourceManagerComponent->GetDay();

	this->M_CardGradeStaticMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	//设置卡片碰撞类型
	this->M_CardTypeBoxCollision->SetCollisionResponseToAllChannels(ECR_Ignore);
	this->M_CardTypeBoxCollision->SetCollisionResponseToChannel(UGameSystemFunction::GetCardCollisionBoxType(this->GetCardData().M_ECardCollisionType), ECollisionResponse::ECR_Block);
	this->M_CardTypeBoxCollision->SetCollisionResponseToChannel(UGameSystemFunction::GetCardCollisionBoxType(ECardCollisionType::E_CardActor), ECollisionResponse::ECR_Block);
	this->M_CardTypeBoxCollision->SetBoxExtent(FVector(this->GetCardData().M_CardCollsionWidth));
	this->M_CardTypeBoxCollision->SetRelativeLocation(FVector::ZeroVector);


	//如果选择关闭碰撞
	if (!this->GetCardData().M_bEnableCardCollision)
	{
		//获取卡片的碰撞box
		for (auto& Box : this->GetComponents())
		{
			//如果检测到盒子碰撞（设置射线检测）
			if (UBoxComponent* BoxComp = Cast<UBoxComponent>(Box))
			{
				//设置卡片碰撞类型
				BoxComp->SetCollisionResponseToAllChannels(ECR_Ignore);
				break;
			}
		}
	}

	//卡片激活(白天卡)
	if (this->GetCardDay() && this->GetMapDay() || this->GetCardDay() && !this->GetMapDay())
	{
		this->CardActive();
	}
	else if (!this->GetCardDay() && !this->GetMapDay())//(夜间卡)
	{
		this->CardActive();
	}
}

void ACardActor::KillCard()
{
	if (this->M_UUI_MapMeshe)
	{
		//卡片死亡，网格调用死亡函数
		if (this->M_UUI_MapMeshe->EradicateCard(this->M_CardProperty.M_CardID))
		{
			//网格必须存在
			if (this->M_UUI_MapMeshe->GetMapMeshe())
			{
				this->M_UUI_MapMeshe->GetMapMeshe()->RemoveCard(this);
				this->Destroy();
			}
		}
	}
}

void ACardActor::SetUIMapMesh(UUI_MapMeshe* _MapMesh)
{
	this->M_UUI_MapMeshe = _MapMesh;
}

UUI_MapMeshe* ACardActor::GetUIMapMesh()
{
	return this->M_UUI_MapMeshe;
}

void ACardActor::AnalysisActor(AActor* _Actor)
{
	if (Cast<AMouseActor>(_Actor) && IsValid(_Actor) && Cast<AMouseActor>(_Actor)->GetCurrentHP() > 0.f)
	{
		this->SetCurrentMouse(Cast<AMouseActor>(_Actor));
	}
}

void ACardActor::UpdateCardEnableState()
{
	//卡片休息->颜色设置（卡片夜晚并且地图白天）
	if (!this->GetCardDay() && this->GetMapDay())
	{
		//如果没有设置则设置卡片颜色
		if (!this->M_bColorSet)
		{
			this->SetFlipbookColor(FVector(0.2f), 1.f);
			this->M_bColorSet = true;
		}
	}
	else {
		if (this->M_bColorSet)
		{
			this->SetFlipbookColor(FVector(1.f), 1.f);
			this->M_bColorSet = false;
			this->CardActive();
		}
	}
}

void ACardActor::CardActive()
{
	//调用代理
	if (this->M_CardActiveDelegate.IsBind())
	{
		this->M_CardActiveDelegate.Call();
	}
}

void ACardActor::CardBeHurt(AMouseActor* CurMouseActor)
{
	//调用代理
	this->M_CardBeHurtDelegate.ExecuteIfBound(CurMouseActor);
}

void ACardActor::CardDeath()
{
	//调用代理
	if (this->M_CardDeathDelegate.IsBind())
	{
		this->M_CardDeathDelegate.Call();
	}
}

ACardActor::ACardActor()
{
	PrimaryActorTick.bCanEverTick = true;

	this->CurCheckMesheLine = FLine(-1);

	this->M_CardGradeStaticMesh = this->CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CardGradeMeshComponent"));
	this->M_CardGradeStaticMesh->SetupAttachment(this->GetGameLocation());
	this->M_CardGradeStaticMesh->SetWorldRotation(FRotator(0.f, 0.f, 90.f));

	this->M_CardTypeBoxCollision = this->CreateDefaultSubobject<UBoxComponent>(TEXT("CardTypeBox"));
	this->M_CardTypeBoxCollision->SetupAttachment(this->GetRootComponent());

	this->CurCardDataComponent = this->CreateDefaultSubobject<UCardDataComponent>(TEXT("CurCardDataComponent"));
}

void ACardActor::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (this->GamePause())
	{
		return;
	}

	//更新卡片激活状态
	this->UpdateCardEnableState();
}

UCardDataComponent* ACardActor::GetCardDataComponent()
{
	return this->CurCardDataComponent;
}

UBoxComponent* ACardActor::GetBoxComponent()
{
	return this->M_CardTypeBoxCollision;
}


void ACardActor::InitCheckMeshe()
{
	//老鼠检测
	if (FVM_VS_GamePause())
	{
		return;
	}

	//当线路相同时，则不会检测
	if (this->GetLine() == this->CurCheckMesheLine)
	{
		return;
	}

	//当线路不同时，重新检测范围
	this->CurCheckMesheLine = this->GetLine();
	//清除之前的线路
	this->CurMapMouseMesheManagers.Empty();

	//得到网格控制组件
	UMesheControllComponent* CurMesheCtr = AGameMapInstance::GetGameMapInstance()->M_MesheControllComponent;

	//范围检测
	for (const FLineCheckSetting& CurCheck : this->M_SourceCardDataBase.M_LineCheckSetting)
	{
		//线路检测
		switch (CurCheck.M_ELineTraceType)
		{
		case ELineTraceType::E_MouseGround:this->bCheckGround = true; break;
		case ELineTraceType::E_MouseSky:this->bCheckSky = true; break;
		default:this->bCheckUnderGround = true; break;
		}

		//当检测范围为-1
		if (CurCheck.CheckLength == -1)
		{
			this->CheckMeshe(CurCheck, 99999, CurMesheCtr);
		}
		else {
			//当检测范围大于0
			if (CurCheck.CheckLength > 0)
			{
				this->CheckMeshe(CurCheck, CurCheck.CheckLength, CurMesheCtr);
			}
		}
	}
}

void ACardActor::CheckMeshe(const FLineCheckSetting& CurCheck, const int32& CheckLength, UMesheControllComponent* CurMesheCtr)
{
	switch (CurCheck.M_ELineTracePosition)
	{
	case ELineTracePosition::E_Left:
	{
		//获取检测最小值
		int32 Min = this->CurCheckMesheLine.Col - CheckLength;

		if (Min < -1)
		{
			Min = -1;
		}

		AMapMouseMesheManager* CurMouseMeshe = nullptr;

		//获取当前所有左边的网格
		for (int32 i = this->CurCheckMesheLine.Col; i > Min; i--)
		{

			CurMouseMeshe = CurMesheCtr->GetMapMouseMesh(this->CurCheckMesheLine.Row + CurCheck.CheckOffset, i);
			if (!CurMouseMeshe)
			{
				continue;
			}

			if (this->CurMapMouseMesheManagers.Find(CurMouseMeshe) != INDEX_NONE)
			{
				this->CurMapMouseMesheManagers.Emplace(CurMouseMeshe);
			}
		}
	}break;
	case ELineTracePosition::E_Right:
	{
		//获取检测最大值
		int32 Max = this->CurCheckMesheLine.Col + CheckLength;

		if (Max > CurMesheCtr->GetMapMeshRowAndCol().Col)
		{
			Max = CurMesheCtr->GetMapMeshRowAndCol().Col;
		}

		AMapMouseMesheManager* CurMouseMeshe = nullptr;

		//获取当前所有右边的网格
		for (int32 i = this->CurCheckMesheLine.Col; i < Max; i++)
		{

			CurMouseMeshe = CurMesheCtr->GetMapMouseMesh(this->CurCheckMesheLine.Row + CurCheck.CheckOffset, i);
			if (!CurMouseMeshe)
			{
				continue;
			}

			if (this->CurMapMouseMesheManagers.Find(CurMouseMeshe) == INDEX_NONE)
			{
				this->CurMapMouseMesheManagers.Emplace(CurMouseMeshe);
			}
		}
	}break;
	case ELineTracePosition::E_LeftRight_M:
	{
		//检测左右所有【不会被检测长度影响】

		AMapMouseMesheManager* CurMouseMeshe = nullptr;

		//获取所有右边的网格
		for (int32 i = 0; i < CurMesheCtr->GetMapMeshRowAndCol().Col; i++)
		{
			CurMouseMeshe = CurMesheCtr->GetMapMouseMesh(this->CurCheckMesheLine.Row + CurCheck.CheckOffset, i);
			if (!CurMouseMeshe)
			{
				continue;
			}

			if (this->CurMapMouseMesheManagers.Find(CurMouseMeshe) == INDEX_NONE)
			{
				this->CurMapMouseMesheManagers.Emplace(CurMouseMeshe);
			}
		}
	}break;
	case ELineTracePosition::E_Top:
	{
		//获取检测最小值
		int32 Min = this->CurCheckMesheLine.Row - CheckLength;

		if (Min < -1)
		{
			Min = -1;
		}

		AMapMouseMesheManager* CurMouseMeshe = nullptr;

		//获取当前上边的所有网格
		for (int32 i = this->CurCheckMesheLine.Row; i > Min; i--)
		{
			CurMouseMeshe = CurMesheCtr->GetMapMouseMesh(i, this->CurCheckMesheLine.Col + CurCheck.CheckOffset);
			if (!CurMouseMeshe)
			{
				continue;
			}

			if (this->CurMapMouseMesheManagers.Find(CurMouseMeshe) == INDEX_NONE)
			{
				this->CurMapMouseMesheManagers.Emplace(CurMouseMeshe);
			}
		}
	}break;
	case ELineTracePosition::E_Bottom:
	{
		//获取检测最大值
		int32 Max = this->CurCheckMesheLine.Row + CheckLength;

		if (Max > CurMesheCtr->GetMapMeshRowAndCol().Row)
		{
			Max = CurMesheCtr->GetMapMeshRowAndCol().Row;
		}

		AMapMouseMesheManager* CurMouseMeshe = nullptr;

		//获取当前下边的所有网格
		for (int32 i = this->CurCheckMesheLine.Row; i < Max; i++)
		{
			CurMouseMeshe = CurMesheCtr->GetMapMouseMesh(i, this->CurCheckMesheLine.Col + CurCheck.CheckOffset);
			if (!CurMouseMeshe)
			{
				continue;
			}

			if (this->CurMapMouseMesheManagers.Find(CurMouseMeshe) == INDEX_NONE)
			{
				this->CurMapMouseMesheManagers.Emplace(CurMouseMeshe);
			}
		}
	}break;
	case ELineTracePosition::E_TopBottom_M:
	{
		//检测上下所有【不会被检测长度影响】
		AMapMouseMesheManager* CurMouseMeshe = nullptr;
		//获取当前下边的所有网格
		for (int32 i = 0; i < CurMesheCtr->GetMapMeshRowAndCol().Row; i++)
		{
			CurMouseMeshe = CurMesheCtr->GetMapMouseMesh(i, this->CurCheckMesheLine.Col + CurCheck.CheckOffset);
			if (!CurMouseMeshe)
			{
				continue;
			}

			if (this->CurMapMouseMesheManagers.Find(CurMouseMeshe) == INDEX_NONE)
			{
				this->CurMapMouseMesheManagers.Emplace(CurMouseMeshe);
			}
		}
	}break;
	default:break;
	}
}

bool ACardActor::CheckMouse(const float& DeltaTime)
{
	//设置检测网格范围
	this->InitCheckMeshe();

	for (const AMapMouseMesheManager* const CurMouseMeshe : this->CurMapMouseMesheManagers)
	{
		for (AMouseActor* CurMouse : CurMouseMeshe->GetCurMouse())
		{
			if (IsValid(CurMouse) && !CurMouse->GetMouseIsDeath())
			{
				this->OnCheckMouse(CurMouse);

				switch (CurMouse->GetMouseLineType())
				{
					//空中
				case ELineType::Sky:
				{
					this->OnCheckMouseSky(CurMouse);
				}break;
				//陆地
				default:
				{
					this->OnCheckMouseGround(CurMouse);
				}break;
				}

				return true;
			}
		}
	}

	return false;
}

void ACardActor::OnCheckMouse(AMouseActor* Mouse) {
	UE_LOG(LogTemp, Error, TEXT("检测到老鼠"));
}

void ACardActor::OnCheckMouseGround(AMouseActor* Mouse) {
	UE_LOG(LogTemp, Error, TEXT("检测到【陆地】老鼠"));

	//检查地下老鼠
	if (Mouse->GetMouseLineType() == ELineType::Underground)
	{
		this->OnCheckMouseUnderground(Mouse);
	}
}

void ACardActor::OnCheckMouseSky(AMouseActor* Mouse) {
	UE_LOG(LogTemp, Error, TEXT("检测到【空中】老鼠"));
}

void ACardActor::OnCheckMouseUnderground(AMouseActor* Mouse) {
	UE_LOG(LogTemp, Error, TEXT("检测到【地下】老鼠"));
}
