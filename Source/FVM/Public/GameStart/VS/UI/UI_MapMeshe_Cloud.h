// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetBase.h"
#include "UI_MapMeshe_Cloud.generated.h"


 //这个板块只能放置 棉花糖

class UImage;
class ACardActor;
class AMapMeshe;
class UCardManagerComponent;
class UUI_Card;

class AMapMeshe_Cloud;

UCLASS()
class FVM_API UUI_MapMeshe_Cloud : public UWidgetBase
{
	GENERATED_BODY()
private:
	UPROPERTY()
		AMapMeshe_Cloud* M_AMapMeshe_Cloud = nullptr;
public:
	//初始化
	virtual bool Initialize() override;
public:
	//创建卡片
	UFUNCTION(BlueprintCallable)
		void CreateNewCard(UCardManagerComponent* _CardMComp);
	//设置云朵对象
	UFUNCTION(BlueprintCallable)
		void SetMapMesheCloud(AMapMeshe_Cloud* _Meshe);
};
