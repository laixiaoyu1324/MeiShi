// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除

#pragma once

#include "CoreMinimal.h"
#include "GameStart/Flipbook/GameActor/CardActor.h"
#include "ATKCardActorBase.generated.h"

/**
 *  带有攻击力的卡片
 */

UCLASS()
class FVM_API AATKCardActorBase : public ACardActor
{
	GENERATED_BODY()
public:

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;

public:
	UFUNCTION(BlueprintCallable, Category = "攻击|攻击力")
		float GetCurrentATK();
	UFUNCTION(BlueprintCallable, Category = "攻击|二段攻击力")
		float GetCurrentSecondATK(const float& TAKRate);
	UFUNCTION(BlueprintCallable, Category = "攻击|攻击速度")
		float GetCurrentAttackSpeed();
	UFUNCTION(BlueprintCallable, Category = "攻击|攻击次数")
		int32 GetCurrentAttackCount();
	UFUNCTION(BlueprintCallable, Category = "攻击|攻击延迟1")
		float GetCurrentFristAttackDelay();
	UFUNCTION(BlueprintCallable, Category = "攻击|攻击延迟2")
		float GetCurrentSecondAttackDelay();
protected:
	//攻击力
	inline void SetATK(const float& ATKBase);
	inline float GetATK() const;

	inline void SetCurATK(const float& ATKBase);
	inline float GetCurATK() const;

	//攻击速度
	inline void SetAttackSpeed(const float& Speed);
	inline float GetAttackSpeed() const;

	inline void SetCurAttackSpeed(const float& Speed);
	inline float GetCurAttackSpeed() const;

	//攻击次数
	inline void SetAttackCount(const int32& Count);
	inline int32 GetAttackCount() const;

	//投射延迟
	inline void SetAttackDelay(const float& First, const float& Second);
	inline float GetFristAttackDelay() const;
	inline float GetSecondAttackDelay() const;
private:
	//卡片攻击力
	UPROPERTY()
		float ATK = 0.f;
	UPROPERTY()
		float CurATK = 0.f;
	//卡片攻击速度
	UPROPERTY()
		float AttackSpeed = 0.f;
	UPROPERTY()
		float CurAttackSpeed = 0.f;
	//投射次数
	UPROPERTY()
		int32 AttackCount = 0;
	//首次投射时间
	UPROPERTY()
		float FirstAttackTime = 0.f;
	//每次投射间隔
	UPROPERTY()
		float AttackEveryTime = 0.f;
};
