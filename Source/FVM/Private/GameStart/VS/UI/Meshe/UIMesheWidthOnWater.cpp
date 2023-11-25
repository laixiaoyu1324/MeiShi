// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除


#include "GameStart/VS/UI/Meshe/UIMesheWidthOnWater.h"
#include "GameStart/VS/UI/Meshe/UIMesheWidthAll.h"
#include "GameStart/Flipbook/GameActor/CardActor.h"
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

bool UUIMesheWidthOnWater::Initialize()
{
	return Super::Initialize();
}

bool UUIMesheWidthOnWater::CreateCard(UUI_MapMeshe* _Meshe, UCardManagerComponent* _CardMComp, UClass* _CardRes, const FString& _CardActorName, int32 _Price, bool ResetCoolDown,bool _bCheck)
{
	if (Super::CreateCard(_Meshe, _CardMComp, _CardRes, _CardActorName, _Price, ResetCoolDown,_bCheck))
	{
		return UUIMesheWidthAll::CreateCard(
			_Meshe,
			_CardMComp,
			_CardRes,
			_CardActorName,
			_Price,
			ResetCoolDown,
			[&](UCardManagerComponent* _CardMComp)->bool {

				//获取卡片数据
				FItemCard& CardData = _CardMComp->GetCardData(_CardActorName);

				//卡片属于水上类型
				if (CardData.M_ELineType == ELineType::OnWater)
				{
					return this->BeginPlayCard(CardData, _CardMComp, _CardRes, CardData.M_CardPrice, ResetCoolDown);
				}
				else {
					//卡片属于非水上类型
					if (this->GroundCardToOnWater(CardData, _CardMComp))
					{
						return this->BeginPlayCard(CardData, _CardMComp, _CardRes, CardData.M_CardPrice, ResetCoolDown);
					}
				}

				this->CardTypeDebug(CardData.ItemName, CardData.M_ELineType);

				return false;
			}
		);
	}

	return false;
}

bool UUIMesheWidthOnWater::GroundCardToOnWater(FItemCard& _Card, UCardManagerComponent* _CardMComp)
{
	//如果卡片是陆地类型
	if (_Card.M_ELineType == ELineType::OnGround || _Card.M_ELineType == ELineType::UnderwaterAndGround)
	{
		//查询卡片 ->层级为-1 （查询水上承载类型卡片）
		ACardActor** _Reult = this->M_Card_Data.Find(-1);

		//如果查询成功并且这个卡片类型和当前网格类型匹配
		if (_Reult && (*_Reult) && !(*_Reult)->IsPendingKillPending() && (*_Reult)->GetLineType() == this->M_ELineType)
		{
			//卡片必须是木盘子类型
			FString _Card_[2] = { TEXT("木盘子"),TEXT("友情木盘子") };
			for (auto Name : _Card_)
			{
				if ((*_Reult)->GetCardName().Equals(Name))
				{
					return true;
				}
			}
		}
	}

	this->CardTypeDebug(_Card.ItemName, _Card.M_ELineType);

	return false;
}
