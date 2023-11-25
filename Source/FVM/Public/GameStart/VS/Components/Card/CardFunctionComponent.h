// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include <Components/BoxComponent.h>
#include "GameStart/Flipbook/GameActor/FlyItemActor.h"
#include "CardFunctionComponent.generated.h"

class AFunctionCardActor;
class AFlyItemActor;
class UUI_MapMeshe;


//卡片功能
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class FVM_API UCardFunctionComponent : public UActorComponent
{
	GENERATED_BODY()
private:
	//卡片组件
	UPROPERTY()
		AFunctionCardActor* FunctionCardActor = nullptr;
	//格子组件(当卡片放下时->获取格子)
	UPROPERTY()
		UUI_MapMeshe* M_CardMapMeshe = nullptr;
private:
	//一些条件
	//当卡片用于激活时触发功能（此条件才会在tick生效）
	bool M_bCardActive = false;
	//-------------------------------------------------------
	//在一定条件下激活动画播放结束时死亡
	bool M_AnimPlayEndDeath = false;
	//-------------------------------------------------------
	//当卡片用于生长时触发功能（此条件才会在tick生效）
	//开启生长功能
	bool M_bCardGrowFunc = false;
	//开始生长
	bool M_bCardGrowBegin = false;
	//卡片是否生长
	bool M_bCardGrow = false;
	//生长完成
	bool M_bCardGrowFinish = false;
	//卡片所需生长事件
	float M_fCardGrowTime = 0.f;
	//-------------------------------------------------------
private:
	//当前上一次选择的卡片每次
	UPROPERTY()
		FString M_PlayerLastSelectCardActorName;
	UPROPERTY()
		UClass* M_PlayerLastSelectCardActorClass = nullptr;
public:
	// Sets default values for this component's properties
	UCardFunctionComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
private:
	void Func_Delegate(FFlyItem_Influence_Tag _Tag, AActor* OtherActor = nullptr, UCardFunctionComponent* _Class = nullptr, void (UCardFunctionComponent::* _Func)(AActor* _A) = nullptr);
private:
	//白天转黑夜
	void CardFunc_TransfromDayToNight();
	//黑夜转白天
	void CardFunc_TransfromNightToDay();
	//当前网格的防御卡快速冷却完成
	void CardFunc_CurrentMesheCoodDownFinish();
	//将所有的防御卡全部快速冷却完成(除开冷却卡)
	void CardFunc_AllCardCoodDownFinish();
	//复制上一张卡片
	void CardFunc_CopyNextCard();
	//炸弹效果
	void CardFunc_BombEffect();
	//卡片激活效果
	void CardFunc_CardActive();
	//攻击力提升
	void Bullet_ATK_Up(AActor* OtherActor = nullptr);
	//子弹反弹
	void Bullet_Rebound(AActor* OtherActor = nullptr);
	//子弹穿过
	void Bullet_Across(AActor* OtherActor = nullptr);
	//子弹buff携带
	void Bullet_Buff_Set(AActor* OtherActor = nullptr);
private:
	//卡片生长
	void CardGrowFunction(const float& Tick);
public:
	//当动画播放完毕
	void OnAnimationPlayEnd();

	//事件触发器
	UFUNCTION()
		void EventTrigger(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
public:
	//事件触发->飞行物
	void Trigger_FlyItem(AActor* OtherActor);
	//事件触发->老鼠
	void Trigger_MouseActor(AActor* OtherActor);
public:
	//创建一个受到伤害就会爆炸的效果(需要开启受伤就爆炸的模式才可以调用)
	void CreateBeHurtBombHittMouse();
	//检查范围内是否有老鼠，如果有触发爆炸
	void CheckOverlapMouseCreateBomb();
public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
};
