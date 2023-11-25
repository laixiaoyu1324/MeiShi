// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除


#include "GameStart/VS/UI/Meshe/UIMesheWidthMagma.h"
#include "GameStart/VS/UI/Meshe/UIMesheWidthAll.h"

#include "GameStart/Flipbook/GameActor/CardActor.h"
#include "GameStart/Flipbook/GameActor/GamePlayer.h"
#include "GameStart/Flipbook/GameActor/FlyItemActor.h"

#include "GameStart/VS/Components/CardManagerComponent.h"
#include "GameStart/VS/Components/VSManagerComponent.h"
#include "GameStart/VS/CardCoolDownManager.h"
#include "GameStart/VS/GameMapInstance.h"
#include "GameStart/VS/MapMeshe.h"

#include "GameStart/VS/Components/ResourceManagerComponent.h"
#include "GameStart/VS/Components/MesheControllComponent.h"
#include "GameStart/VS/Components/Item\ShootLineComponent.h"

#include "GameStart/VS/CreateCardBar.h"
#include "GameStart/VS/UI/UI_Card.h"

#include "GameSystem/FVMGameInstance.h"

#include <Kismet/GameplayStatics.h>

#include <Components/Image.h>

bool UUIMesheWidthMagma::Initialize()
{
	return Super::Initialize();
}

bool UUIMesheWidthMagma::CreateCard(UUI_MapMeshe* _Meshe, UCardManagerComponent* _CardMComp, UClass* _CardRes, const FString& _CardActorName, int32 _Price, bool ResetCoolDown, bool _bCheck)
{
	if (Super::CreateCard(_Meshe, _CardMComp, _CardRes, _CardActorName, _Price, ResetCoolDown, _bCheck))
	{
		return UUIMesheWidthAll::CreateCard(
			_Meshe,
			_CardMComp,
			_CardRes,
			_CardActorName,
			_Price,
			ResetCoolDown,
			[&](UCardManagerComponent* _MComp)->bool {
				//获取卡片数据，判断卡片是否属于陆地类型
				FItemCard& CardData = _CardMComp->GetCardData(_CardActorName);

				//如果卡片是岩浆类型
				if (CardData.M_ELineType == ELineType::Magma)
				{
					//放置卡片
					if (!this->BeginPlayCard(CardData, _CardMComp, _CardRes, CardData.M_CardPrice, ResetCoolDown))
						return false;

					//查询卡片 ->层级为-1 （查询承载类型卡片）
					ACardActor** _Reult = this->M_Card_Data.Find(-1);

					//如果查询成功并且这个卡片类型和当前网格类型匹配
					if (_Reult && (*_Reult) && !(*_Reult)->IsPendingKillPending())
					{
						//设置MapMeshe网格更新
						this->M_MapMeshUpdate.M_IsbUpdate = true;

						if ((*_Reult)->GetCardData().ItemName.Equals(TEXT("棉花糖")))
						{
							//优先消耗棉花糖的血量
							this->M_MapMeshUpdate.AddCardUpdateAt((*_Reult), 0);
						}
					}
					else {
						uint8 CardID[] = { 1,0 };
						for (auto& ID : CardID)
						{
							//查询卡片 ->层级为-1 （查询水上承载类型卡片）
							ACardActor** LReult = this->M_Card_Data.Find(ID);
							//如果查询成功并且这个卡片类型和当前网格类型匹配
							if (LReult && (*LReult) && !(*LReult)->IsPendingKillPending())
							{
								this->M_MapMeshUpdate.AddCardUpdate((*LReult));
								//设置MapMeshe网格更新
								this->M_MapMeshUpdate.M_IsbUpdate = true;
							}
						}
					}

					return true;
				}
				else {
					//如果是其他类型
					if (
						CardData.M_ELineType == ELineType::OnGround
						|| CardData.M_ELineType == ELineType::Sky
						|| CardData.M_ELineType == ELineType::UnderwaterAndGround
						)
					{
						if (!this->BeginPlayCard(CardData, _CardMComp, _CardRes, CardData.M_CardPrice, ResetCoolDown))
							return false;

						uint8 CardID[] = { 1,0 };
						for (auto& ID : CardID)
						{
							//查询卡片 ->层级为-1 （查询水上承载类型卡片）
							ACardActor** LReult = this->M_Card_Data.Find(ID);
							//如果查询成功并且这个卡片类型和当前网格类型匹配
							if (LReult && (*LReult) && !(*LReult)->IsPendingKillPending())
							{
								this->M_MapMeshUpdate.AddCardUpdate((*LReult));
								//设置MapMeshe网格更新
								this->M_MapMeshUpdate.M_IsbUpdate = true;
							}
						}
						
						return true;
					}
				}

				this->CardTypeDebug(CardData.ItemName, CardData.M_ELineType);

				return false;
			});
	}

	return false;
}

void UUIMesheWidthMagma::BoxOverlapBeginByAMapMeshe(AActor* _Actor)
{
	Super::BoxOverlapBeginByAMapMeshe(_Actor);

	if (this->M_ELineType != ELineType::Magma)
		return;

	for (auto& Card : this->M_MapMeshUpdate.GetUpdateCards())
	{
		if (Card && !Card->IsPendingKillPending())
		{
			if (Card->GetCardData().ItemName.Equals(TEXT("棉花糖")))
			{
				return;
			}
		}
	}

	//获取直线类型子弹对象
	AFlyItemActor* FlyActor = Cast<AFlyItemActor>(_Actor);

	if (!FlyActor)
		return;

	bool LResult = false;

	for (auto& Comp : FlyActor->GetComponents())
	{
		if (Cast<UShootLineComponent>(Comp))
		{
			LResult = true;
			break;
		}
	}

	//直接销毁
	if (LResult)
		FlyActor->Destroy();
}

void UUIMesheWidthMagma::BoxOverlapEndByAMapMeshe(AActor* _Actor)
{
	Super::BoxOverlapEndByAMapMeshe(_Actor);
}

void UUIMesheWidthMagma::TickByAMapMeshe(float DeltaTime)
{
	Super::TickByAMapMeshe(DeltaTime);

	if (!this->M_MapMeshUpdate.M_IsbUpdate)
	{
		return;
	}

	this->M_MapMeshUpdate.M_Time += DeltaTime;

	if (this->M_MapMeshUpdate.M_Time > 1.f)
	{
		this->M_MapMeshUpdate.M_Time = 0.f;

		bool LResult = false;

		for (auto& Card : this->M_MapMeshUpdate.GetUpdateCards())
		{
			if (Card && !Card->IsPendingKillPending())
			{
				//更新卡片血量
				Card->UpdateCardState(1, 0);

				//更新最新状态
				if (Card->GetCurrentHP() <= 0.f)
					Card->KillCard();

				LResult = true;

				break;
			}
		}

		//如果列表为空
		if (!LResult)
		{
			this->M_MapMeshUpdate.GetUpdateCards().Empty();
			this->M_MapMeshUpdate.M_Time = 0.f;
			this->M_MapMeshUpdate.M_IsbUpdate = false;
		}
	}
}
