// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除


#include "GameStart/VS/UI/UI_MapMeshe_Cloud.h"
#include "GameStart/VS/MapMeshe_Cloud.h"

#include "GameStart/Flipbook/GameActor/CardActor.h"
#include "GameStart/VS/Components/CardManagerComponent.h"
#include "GameStart/VS/CardCoolDownManager.h"
#include "GameStart/VS/GameMapInstance.h"
#include "GameStart/VS/MapMeshe.h"

#include "GameStart/VS/Components/ResourceManagerComponent.h"
#include "GameStart/VS/Components/MesheControllComponent.h"

#include "GameSystem/FVMGameInstance.h"

#include "GameStart/VS/CreateCardBar.h"
#include "GameStart/VS/UI/UI_Card.h"
#include <Kismet/GameplayStatics.h>

bool UUI_MapMeshe_Cloud::Initialize()
{
	if (!Super::Initialize())
		return false;



	return true;
}


void UUI_MapMeshe_Cloud::CreateNewCard(UCardManagerComponent* _CardMComp)
{
	//获取当前选择的卡片
	if (_CardMComp->GetCurrentSelectActor())
	{
		//获取卡片数据
		FItemCard& CardData = _CardMComp->GetCardData(_CardMComp->M_CurrentSelectCardName);

		if (CardData.ItemName.Equals(TEXT("棉花糖")))
		{
			if (_CardMComp->GetCurrentCoolDownManager() && !_CardMComp->GetCurrentCoolDownManager()->IsPendingKillPending())
			{
				//重置冷却
				_CardMComp->GetCurrentCoolDownManager()->SetCoolDownFinish(false);
			}

			//减去费用
			AGameMapInstance::GetGameMapInstance()->M_ResourceManagerComponent->SubtractFlameNum(_CardMComp->GetCardFlameNum(_CardMComp->M_CurrentSelectCardName));

			_CardMComp->CancelSelect();

			//播放音效
			UFVMGameInstance::GetFVMGameInstance()->PlayBGM_S("PlayCardToGround", "ItemAudio");

			if (this->M_AMapMeshe_Cloud && !this->M_AMapMeshe_Cloud->IsPendingKillPending())
			{
				//移除云朵图片标记
				this->M_AMapMeshe_Cloud->RemoveCloudTag();
				//销毁云朵Actor对象
				this->M_AMapMeshe_Cloud->Destroy();
			}

		}
	}
}

void UUI_MapMeshe_Cloud::SetMapMesheCloud(AMapMeshe_Cloud* _Meshe)
{
	this->M_AMapMeshe_Cloud = _Meshe;
}
