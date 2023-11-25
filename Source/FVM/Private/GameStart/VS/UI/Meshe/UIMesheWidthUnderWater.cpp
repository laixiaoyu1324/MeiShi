// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除


#include "GameStart/VS/UI/Meshe/UIMesheWidthUnderWater.h"

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

bool UUIMesheWidthUnderWater::Initialize()
{
	Super::Initialize();

	return true;
}

bool UUIMesheWidthUnderWater::CreateCard(UUI_MapMeshe* _Meshe, UCardManagerComponent* _CardMComp, UClass* _CardRes, const FString& _CardActorName, int32 _Price, bool ResetCoolDown, bool _bCheck)
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

				//获取卡片数据，判断卡片类型
				FItemCard& CardData = _CardMComp->GetCardData(_CardActorName);

				if (
					CardData.M_ELineType == ELineType::OnGround ||
					CardData.M_ELineType == ELineType::UnderwaterAndGround ||
					CardData.M_ELineType == ELineType::SkyAndGround ||
					CardData.M_ELineType == ELineType::Underwater
					)
				{
					//放置卡片，如果放置失败则什么都不管
					if (!this->BeginPlayCard(CardData, _CardMComp, _CardRes, CardData.M_CardPrice, ResetCoolDown))
					{
						return false;
					}

					//查询卡片 ->层级为3 （查询肥皂泡类型卡片）
					ACardActor** _Reult = this->M_Card_Data.Find(3);
					//如果查询成功并且这个卡片类型和当前网格类型匹配
					if (_Reult && IsValid(*_Reult))
					{
						if (
							(*_Reult)->GetCardData().ItemName.Equals(TEXT("苏打气泡")) ||
							(*_Reult)->GetCardData().ItemName.Equals(TEXT("五彩香皂泡泡"))
							)
						{
							//优先消耗肥皂泡的血量
							this->M_MapMeshUpdate.AddCardUpdateAt((*_Reult), 0);
							//设置MapMeshe网格更新
							this->M_MapMeshUpdate.M_IsbUpdate = true;
						}

						//隐藏泡泡
						if (IsValid(this->WaterBubbles))
						{
							this->WaterBubbles->SetActorHiddenInGame(true);
						}
					}
					else {
						uint8 CardID[] = { 1,0 };
						for (auto& ID : CardID)
						{
							//查询卡片
							ACardActor** LReult = this->M_Card_Data.Find(ID);
							//如果查询成功并且这个卡片类型和当前网格类型匹配
							if (LReult && IsValid(*LReult))
							{
								if (CardData.M_ELineType != ELineType::Underwater
									||
									CardData.M_ELineType != ELineType::UnderwaterAndGround
									)
								{
									this->M_MapMeshUpdate.AddCardUpdate((*LReult));
									//设置MapMeshe网格更新
									this->M_MapMeshUpdate.M_IsbUpdate = true;
								}

							}
						}

						if (
							CardData.M_ELineType != ELineType::Underwater
							||
							CardData.M_ELineType != ELineType::UnderwaterAndGround
							)
						{
							//显示泡泡
							if (IsValid(this->WaterBubbles))
							{
								this->WaterBubbles->SetActorHiddenInGame(false);
							}
							else {
								this->WaterBubbles = this->GetMapMeshe()->GetWorld()->SpawnActor<AActor>(
									LoadClass<AActor>(nullptr,
										TEXT("Blueprint'/Game/Resource/BP/GameStart/MapLauncher/5/t/BP_AWaterBubbles.BP_AWaterBubbles_C'")
										),
									this->GetMapMeshe()->GetActorTransform()
									);
							}
						}
					}

					return true;
				}

				//打印Log
				this->CardTypeDebug(CardData.ItemName, CardData.M_ELineType);

				return false;
			});
	}

	return false;
}

void UUIMesheWidthUnderWater::TickByAMapMeshe(float DeltaTime)
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
			if (IsValid(Card))
			{
				//更新卡片血量
				Card->UpdateCardState(1, 0);

				//更新最新状态
				if (Card->GetCurrentHP() <= 0.f)
				{
					Card->KillCard();
				}

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

			//隐藏泡泡
			if (IsValid(this->WaterBubbles))
			{
				this->WaterBubbles->SetActorHiddenInGame(true);
			}
		}
	}
}
