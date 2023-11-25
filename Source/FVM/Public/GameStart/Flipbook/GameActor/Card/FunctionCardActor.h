// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除

#pragma once

#include "CoreMinimal.h"
#include "GameStart/Flipbook/GameActor/CardActor.h"
#include "FunctionCardActor.generated.h"

/**
 * 功能卡片
 */

class UBoxComponent;

UCLASS()
class FVM_API AFunctionCardActor : public ACardActor
{
	GENERATED_BODY()
public:
	//获取功能卡片数据
	const FItemCardFunction& GetFunctionCardData();
	//获取重叠盒体
	UBoxComponent* GetOverlapBoxComponent();
public:
	UFUNCTION(BlueprintCallable)
		void SetOverlapBoxComponent(UBoxComponent* Box);
public:
	//默认动画
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "卡片预设属性 | 动画")
		TSoftObjectPtr<UPaperFlipbook> CardActor_DefAniml;
	//被激活动画
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "卡片预设属性 | 动画")
		TSoftObjectPtr<UPaperFlipbook> CardActor_BeActiveAnim;
	//生长动画【有生长功能的】
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "卡片预设属性 | 动画")
		TSoftObjectPtr<UPaperFlipbook> CardActor_GrowAnim;
	//生长完成动画【有生长功能的】
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "卡片预设属性 | 动画")
		TSoftObjectPtr<UPaperFlipbook> CardActor_GrowFinishAnim;
public:

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;

private:
	//功能卡片数据
	UPROPERTY()
		FItemCardFunction ItemCardFunction;
	//碰撞盒体
	UPROPERTY()
		UBoxComponent* OverlapBoxComponent = nullptr;
};
