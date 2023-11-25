// Fill out your copyright notice in the Description page of Project Settings.


#include "GameStart/VS/UI/UI_MapMeshe.h"
#include "GameStart/Flipbook/GameActor/CardActor.h"
#include "GameStart/Flipbook/GameActor/MouseActor.h"
#include "GameStart/Flipbook/GameActor/GamePlayer.h"

#include "GameStart/VS/Components/CardManagerComponent.h"
#include "GameStart/VS/Components/VSManagerComponent.h"
#include "GameStart/VS/CardCoolDownManager.h"
#include "GameStart/VS/GameMapInstance.h"
#include "GameStart/VS/MapMeshe.h"

#include "GameStart/VS/Components/ResourceManagerComponent.h"
#include "GameStart/VS/Components/MesheControllComponent.h"

#include "GameStart/VS/CreateCardBar.h"
#include "GameStart/VS/UI/UI_Card.h"

#include "GameSystem/FVMGameInstance.h"

#include <Kismet/GameplayStatics.h>

#include <Components/Image.h>

void UUI_MapMeshe::PlayCard_PlayBGM()
{
	if (this->M_ELineType == ELineType::OnWater)
	{
		UFVMGameInstance::GetFVMGameInstance()->PlayBGM_S("PlayCardToOnWater", "ItemAudio");
	}
	else {
		UFVMGameInstance::GetFVMGameInstance()->PlayBGM_S("PlayCardToGround", "ItemAudio");
	}
}

void UUI_MapMeshe::PlayCard(UCardManagerComponent* _CardMComp, UClass* _CardRes, FItemCard& _CardData, int32 _Price, bool ResetCoolDown)
{

	FTransform Trans = this->M_MapMesheTransform;
	FVector _Location = Trans.GetLocation();
	Trans.SetLocation(_Location);

	ACardActor* Card = Cast <ACardActor>(UGameplayStatics::BeginDeferredActorSpawnFromClass(this->M_MapMeshe->GetWorld(), _CardRes, Trans));
	//设置线路
	Card->SetLine(this->GetLine());
	//设置UIMapMesh
	Card->SetUIMapMesh(this);
	//设置UICard
	Card->SetCardUI(_CardMComp->GetUICard(_CardData.ItemName));
	//产生阶段结束
	UGameplayStatics::FinishSpawningActor(Card, Trans);

	//卡片无效
	if (Card->IsPendingKillPending())
	{
		if (UFVMGameInstance::GetDebug())
			UGameSystemFunction::FVMLog(__FUNCTION__, TEXT("当前卡片资源无效"));
		return;
	}

	//初始化绑定移动函数(绑定网格移动)
	if (this->GetMapMeshe())
	{
		this->GetMapMeshe()->AddCardMoveModelBind(Card);
		//设置卡片排序优先级
		Card->SetTranslucentSortPriority(this->GetCharTranslucency() + _CardData.M_CardLayer + 1);
	}

	//设置当前放置的网格
	AGameMapInstance::GetCardManagerComponent_Static()->SetCurrentCardMapMeshe(this);

	//判断是否是夜间卡片  并且 开启了咖啡粉自动激活
	if (!_CardData.M_CardDay && _CardData.M_AutoEnableCardDay)
	{
		//寻找卡片列表的咖啡粉（如果找到则修改夜间卡片为白天） 
		if (_CardMComp->GetUICard(TEXT("咖啡粉")))
		{
			Card->CardDayTransform(true);
		}
	}

	//重置冷却时间
	if (ResetCoolDown)
		if (_CardMComp->GetCurrentCoolDownManager() && !_CardMComp->GetCurrentCoolDownManager()->IsPendingKillPending())
		{
			//重置冷却
			_CardMComp->GetCurrentCoolDownManager()->SetCoolDownFinish(false);
		}

	//减去费用
	AGameMapInstance::GetGameMapInstance()->M_ResourceManagerComponent->SubtractFlameNum(_Price);

	//判断卡片是否开启费用线性增加
	if (_CardData.M_CardPriceAutoUp)
	{
		_CardData.M_CardPrice += _CardData.M_CardPriceUpNum;
		UUI_Card* const UICard = _CardMComp->GetUICard(_CardData.ItemName);
		UICard->M_NeedFlame = _CardData.M_CardPrice;
	}

	//添加层级
	if (this->IdAdd(_CardData.M_CardLayer, Card))
	{
		//添加上一次卡片选择(限制到可以被复制的卡片Copy)
		if (_CardData.M_bEnableCopy)
		{
			_CardMComp->M_LastSelectCardClass = _CardData.CardActorResource.TryLoadClass<ACardActor>();
			_CardMComp->M_LastSelectCardName = _CardData.ItemName;
		}

		//播放音效
		this->PlayCard_PlayBGM();
	}

	if (ResetCoolDown)
	{
		_CardMComp->CancelSelect();
	}
}

void UUI_MapMeshe::PlayPlayer()
{
	//添加ID 0
	this->IdAdd(0, nullptr);

	//生成角色
	if (IsValid(this->M_MapMeshe))
	{
		AGameMapInstance::GetGameMapInstance()->SpawnPlayerToMeshe(this->M_MapMeshe, this);

		this->bHasChar = true;
	}
}

const TMap<int32, ACardActor*>& UUI_MapMeshe::GetCardDatas()
{
	return this->M_Card_Data;
}

bool UUI_MapMeshe::Initialize()
{
	if (!Super::Initialize())
		return false;

	this->M_Gird = this->GetWidgetComponent<UImage>(this, "Meshe");

	//this->SetColor(FVector(1.f), 1.f);

	return true;
}

bool UUI_MapMeshe::IdAdd(const int32 _ID, ACardActor* _Card)
{

	if (!_Card && _ID != 0)
		return false;

	ACardActor** _Reult = this->M_Card_Data.Find(_ID);

	if (!_Reult)
	{
		this->M_Card_Data.Emplace(_ID, _Card);
		return true;
	}

	return false;
}

bool UUI_MapMeshe::TestID(const int32 _ID)
{
	ACardActor** _Reult = this->M_Card_Data.Find(_ID);
	//寻找卡片ID如果ID唯一则返回true（未找到）
	if (!_Reult)
	{
		if (UFVMGameInstance::GetDebug())
			UGameSystemFunction::FVMLog(__FUNCTION__, FString(TEXT("卡片ID测试成功")));

		return true;
	}

	if (UFVMGameInstance::GetDebug())
		UGameSystemFunction::FVMLog(__FUNCTION__, FString(TEXT("卡片ID测试失败")));

	return false;
}

FString UUI_MapMeshe::GetCardNameByID(const int32& _Id)
{

	ACardActor** Card = this->M_Card_Data.Find(_Id);


	if (Card && (*Card) && !(*Card)->IsPendingKillPending())
	{
		return (*Card)->GetCardName();
	}

	return FString(TEXT(""));
}

UUI_Card* UUI_MapMeshe::GetCardUIByID(const int32& _Id)
{
	ACardActor** Card = this->M_Card_Data.Find(_Id);

	if (Card && (*Card) && !(*Card)->IsPendingKillPending())
	{
		return (*Card)->GetCardUI();
	}

	return nullptr;
}

FString UUI_MapMeshe::GetCardActorNameByID(const int32& _Id)
{
	FItemCard Data;
	if (AGameMapInstance::GetCardManagerComponent_Static()->GetCardDataByName(this->GetCardNameByID(_Id), Data))
	{
		return Data.ItemName;
	}
	return FString(TEXT(""));
}

FLine UUI_MapMeshe::GetLine()
{
	return this->M_Line;
}

void UUI_MapMeshe::EradicateCard()
{
	//销毁顺序
	TArray<int8> M_Card_Layer = { 5,4,2,0,1,-1,3};

	for (auto PP = M_Card_Layer.CreateConstIterator(); PP; PP++)
	{
		ACardActor** _Reult = this->M_Card_Data.Find((*PP));

		if (_Reult && (*_Reult) && !(*_Reult)->IsPendingKillPending())
		{
			(*_Reult)->KillCard();
			break;
		}
		else {
			if (_Reult && !IsValid(*_Reult) && (*PP) == 0)
			{
				//这个是角色ID
			}
		}
	}
}

void UUI_MapMeshe::SetColor(FVector _Vec, float _Alpha)
{
	UWidgetBase::SetImageColor(this->M_Gird, _Vec, _Alpha);
}

void UUI_MapMeshe::SetMapMeshe(AMapMeshe* _Meshe)
{
	this->M_MapMeshe = _Meshe;
}

void UUI_MapMeshe::SetLine(FLine CurLine)
{
	this->M_Line = CurLine;
}

bool UUI_MapMeshe::GetCurMesheIsEnable()
{
	if (IsValid(this->M_MapMeshe))
	{
		return this->M_MapMeshe->GetbUse();
	}
	return false;
}

bool UUI_MapMeshe::EradicateCard(const int32& _CardLayer)
{

	//UE_LOG(LogTemp, Error, TEXT("%d"), _CardLayer);

	ACardActor** _Reult = this->M_Card_Data.Find(_CardLayer);

	if (_Reult && (*_Reult) && !(*_Reult)->IsPendingKillPending())
	{

		//判断卡片是否有线性加价(如果有则减少加价)
		if (FItemCard* const CardData = AGameMapInstance::GetCardManagerComponent_Static()->GetCardDataByName((*_Reult)->GetCardName()))
		{
			if (CardData->M_CardPriceAutoUp && AGameMapInstance::GetCardManagerComponent_Static()->GetUICard(CardData->ItemName))
			{
				AGameMapInstance::GetCardManagerComponent_Static()->GetUICard(CardData->ItemName)->M_NeedFlame -= CardData->M_CardPriceUpNum;
				CardData->M_CardPrice -= CardData->M_CardPriceUpNum;
			}
		}

		this->M_Card_Data.Remove(_CardLayer);

		(*_Reult)->CardDeath();

		return true;
	}

	return false;
}

bool UUI_MapMeshe::CreateNewCard(UCardManagerComponent* _CardMComp)
{
	//调用卡片创建虚函数【子类也实现】
	return  this->CreateCard(
		this,
		_CardMComp,
		_CardMComp->GetCurrentSelectActor(),
		_CardMComp->M_CurrentSelectCardName,
		_CardMComp->GetCardFlameNum(_CardMComp->M_CurrentSelectCardName),
		true,
		true);
}

bool UUI_MapMeshe::CreateCard(UUI_MapMeshe* _Meshe, UCardManagerComponent* _CardMComp, UClass* _CardRes, const FString& _CardActorName, int32 _Price, bool ResetCoolDown, bool _bCheck)
{
	if (_bCheck)
	{
		//如果玩家没有放置角色
		if (!AGameMapInstance::GetGameMapInstance()->GetIsPlayPlayer())
		{
			this->PlayPlayer();

			return false;
		}

		//如果选择了铲卡
		if (_CardMComp->M_CurrentSelectEradicate)
		{
			this->EradicateCard();
			_CardMComp->CancelEradicate();

			AGameActorFlipbookBase* Eradicate_ = this->GetWorld()->SpawnActor<AGameActorFlipbookBase>(
				LoadClass<AGameActorFlipbookBase>(0,
					TEXT("Blueprint'/Game/Resource/BP/GameStart/VS/BPEradicate.BPEradicate_C'")
					),
				this->M_MapMesheTransform);

			Eradicate_->SetAnimationPlayEndDestroy();
			Eradicate_->SetFlipbookRotation(FRotator(0.f, 90.f, 0.f));

			switch (this->M_ELineType)
			{
			case ELineType::OnWater:
			case ELineType::Underwater: {UFVMGameInstance::PlayBGM_S_Static("PlayCardToOnWater", "ItemAudio"); }break;
			default: {UFVMGameInstance::PlayBGM_S_Static("PlayCardToGround", "ItemAudio"); }break;

			}
			return false;
		}

		//如果当前玩家选择了卡牌
		if (_CardMComp->GetCurrentSelectActor())
		{
			return true;
		}

		return false;
	}

	return true;
}

void UUI_MapMeshe::BoxOverlapBeginByAMapMeshe(AActor* _Actor)
{
	if (!IsValid(_Actor))
		return;

	//当老鼠进入网格时设置老鼠排序
	if (Cast<AMouseActor>(_Actor))
	{
		this->OnMouseEnter(Cast<AMouseActor>(_Actor));
	}
}

void UUI_MapMeshe::BoxOverlapEndByAMapMeshe(AActor* _Actor)
{
}

void UUI_MapMeshe::TickByAMapMeshe(float DeltaTime)
{
}

AMapMeshe* UUI_MapMeshe::GetMapMeshe()
{
	return this->M_MapMeshe;
}

FVector UUI_MapMeshe::GetMapMeshCurrentTop(int32 _Col)
{
	AMapMeshe* Meshe = AGameMapInstance::GetGameMapInstance()->M_MesheControllComponent->GetMapMeshCurrentTop(_Col);
	return Meshe->GetActorLocation();
}

FVector UUI_MapMeshe::GetMapMeshCurrentBottom(int32 _Col)
{
	AMapMeshe* Meshe = AGameMapInstance::GetGameMapInstance()->M_MesheControllComponent->GetMapMeshCurrentBottom(_Col);
	return Meshe->GetActorLocation();
}

FVector UUI_MapMeshe::GetMapMeshCurrentLeft(int32 Row)
{
	AMapMeshe* Meshe = AGameMapInstance::GetGameMapInstance()->M_MesheControllComponent->GetMapMeshCurrentLeft(Row);
	return Meshe->GetActorLocation();
}

FVector UUI_MapMeshe::GetMapMeshCurrentRight(int32 Row)
{
	AMapMeshe* Meshe = AGameMapInstance::GetGameMapInstance()->M_MesheControllComponent->GetMapMeshCurrentRight(Row);
	//FVector A = Meshe->GetWidgetComponentTransform().GetLocation();
	//UE_LOG(LogTemp, Error, TEXT("%d %.2f %.2f %.2f"), Row,A.X, A.Y, A.Z);
	return Meshe->GetActorLocation();
}

bool UUI_MapMeshe::BeginPlayCard(FItemCard& _Card, UCardManagerComponent* _CardMComp, UClass* _CardRes, int32 _Price, bool ResetCoolDown)
{
	//判断卡片是否重叠并且查询是否存在相同索引
	if (_Card.M_CardOverlap && !this->TestID(_Card.M_CardLayer))
	{
		//判断当前卡片重叠范围的名称是否匹配			
		//查询卡片 
		ACardActor** _Reult = this->M_Card_Data.Find(_Card.M_CardLayer);

		FString LOverlapCardName;

		//如果查询成功并且这个卡片类型和当前网格类型匹配
		if (_Reult && (*_Reult) && !(*_Reult)->IsPendingKillPending())
		{
			for (auto Name : _Card.M_CardOverlapName)
			{
				if ((*_Reult)->GetCardName().Equals(Name))
				{
					//匹配成功
					//寻找当前索引所对应的卡片(销毁)
					(*_Reult)->KillCard();


					//放置卡片
					this->PlayCard(_CardMComp, _CardRes, _Card, _Price, ResetCoolDown);

					return true;
				}

				LOverlapCardName += Name + TEXT(" ");
			}
		}

		AGameMapInstance::GetVSManagerComponent_Static()->ShowText(FString(TEXT("当前卡片需要在{")) + LOverlapCardName + TEXT("} 这些卡的基础上放置"));
	}
	else {
		//卡片无需重叠
		if (!_Card.M_CardOverlap && this->TestID(_Card.M_CardLayer))
		{
			this->PlayCard(_CardMComp, _CardRes, _Card, _Price, ResetCoolDown);

			return true;
		}
	}

	AGameMapInstance::GetVSManagerComponent_Static()->ShowText(TEXT("无法放置当前防御卡"));

	return false;
}

void UUI_MapMeshe::CardTypeDebug(const FString& _CardName, ELineType _LineType)
{
	switch (_LineType)
	{
	case ELineType::OnGround:
		AGameMapInstance::GetVSManagerComponent_Static()->ShowText(TEXT("当前卡片适合放在-陆地上"));
		break;
	case ELineType::Sky:
		AGameMapInstance::GetVSManagerComponent_Static()->ShowText(TEXT("当前卡片适合放在-空中"));
		break;
	case ELineType::OnWater:
		AGameMapInstance::GetVSManagerComponent_Static()->ShowText(TEXT("当前卡片适合放在-水上"));
		break;
	case ELineType::Underwater:
		AGameMapInstance::GetVSManagerComponent_Static()->ShowText(TEXT("当前卡片适合放在-水下"));
		break;
	case ELineType::Magma:
	{
		if (_CardName.Equals(TEXT("棉花糖")))
			AGameMapInstance::GetVSManagerComponent_Static()->ShowText(TEXT("当前卡片适合放在-岩浆或者填补缺失的云朵"));
		else
			AGameMapInstance::GetVSManagerComponent_Static()->ShowText(TEXT("当前卡片适合放在-岩浆"));
	}
	break;
	case ELineType::UnderwaterAndGround:
		AGameMapInstance::GetVSManagerComponent_Static()->ShowText(TEXT("当前卡片适合放在-陆地或者水下"));
		break;
	case ELineType::SkyAndGround:
		AGameMapInstance::GetVSManagerComponent_Static()->ShowText(TEXT("当前卡片适合放在-空中或者陆地"));
		break;
	default:
		break;
	}
}

int32 UUI_MapMeshe::GetTranslucency()
{
	return this->M_Translucency;
}

int32 UUI_MapMeshe::GetCharTranslucency()
{
	if (this->bHasChar)
	{
		return AGameMapInstance::GetGameMapInstance()->GetPlayer()->GetTranslucency();
	}

	return this->GetTranslucency();
}

void UUI_MapMeshe::SetTranslucency(const int32& _Translucency)
{
	this->M_Translucency = _Translucency;
	this->TranslucencyChange(_Translucency);
}

void UUI_MapMeshe::OnMouseEnter(AMouseActor* Mouse)
{
	if (Mouse->GetMouseLine().Row == this->M_Line.Row)
	{
		Mouse->SetTranslucentSortPriority(this->GetTranslucency() + 19);
	}
}

void UUI_MapMeshe::OnMouseEnd(AMouseActor* Mouse)
{

}
