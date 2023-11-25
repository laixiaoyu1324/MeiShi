// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除

#pragma once

#include "CoreMinimal.h"
#include "GameStart/VS/Components/Card/CardLauncherComponent.h"
#include "GameStart/VS/MapBaseType.h"
#include "CardAttackComponent.generated.h"

/**
 * 卡片攻击组件【一般直线攻击卡片】小笼包，三线，双向等这种类型的基础卡片
 */

class AAttackCardActor;
class AMapMouseMesheManager;
class UMesheControllComponent;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class FVM_API UCardAttackComponent : public UCardLauncherComponent
{
	GENERATED_BODY()
protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	//生成投射物并且设置属性
	virtual void Spawn() override;
	//播放攻击动画
	virtual void PlayAttackAnimation() override;
	//播放默认动画
	virtual void PlayIdleAnimation() override;
public:
	//资源加载
	UFUNCTION(BlueprintCallable)
		virtual void LoadResource() override;
public:
	UCardAttackComponent();

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	//攻击卡
	UPROPERTY(EditAnywhere)
		AAttackCardActor* AttackCardActor = nullptr;
	UPROPERTY()
		float time = 0.f;
};
