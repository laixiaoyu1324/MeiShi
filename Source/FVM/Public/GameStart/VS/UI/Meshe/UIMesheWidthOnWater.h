// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除

#pragma once

#include "CoreMinimal.h"
#include "GameStart/VS/UI/UI_MapMeshe.h"
#include "UIMesheWidthOnWater.generated.h"

/**
 *
 */
UCLASS()
class FVM_API UUIMesheWidthOnWater : public UUI_MapMeshe
{
	GENERATED_BODY()

public:

	//初始化
	virtual bool Initialize() override;

	virtual bool CreateCard(UUI_MapMeshe* _Meshe, UCardManagerComponent* _CardMComp, UClass* _CardRes, const FString& _CardActorName, int32 _Price, bool ResetCoolDown,bool _bCheck) override;

private:
	//线路不匹配：
	// 1.陆地卡放水上
	bool GroundCardToOnWater(FItemCard& _Card, UCardManagerComponent* _CardMComp);
};
