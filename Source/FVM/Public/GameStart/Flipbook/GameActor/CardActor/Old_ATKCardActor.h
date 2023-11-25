// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除

#pragma once

#include "CoreMinimal.h"
#include "GameStart/Flipbook/GameActor/CardActor/ATK/ATKCardActorBase.h"
#include "Data/CardData/CardDataStruct.h"
#include "Old_ATKCardActor.generated.h"

/**
 *
 */

class UPaperFlipbook;
class AFlyItemActor;
class AMouseActor;


//攻击类数据
USTRUCT(BlueprintType)
struct FCard_ATK {
	GENERATED_USTRUCT_BODY()
public:
	//是否已经设置属性
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool M_bATK = false;
	//攻击力
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float M_ATK = 2.f;
	//第二攻击力
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float M_SecondATK = 0.f;
	//攻击速度(延迟)
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float M_ShootSpeed = 3.5f;
	//首次投射时间 {延迟}
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float M_FirstShootSpeed = 0.5f;
	//投射延迟
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float M_ShootDelay = 0.1f;
	//投射多少发
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 M_ProjectionItemCount = 1;
	//射线开始偏移(起点)
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FVector M_TraceBeginOffset = FVector(-1.f, 35.f, -30.f);
	//射线设置
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<FLineTraceSetting> M_SourceLineTraceSettings;
	//射线设置
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<FLineTraceSetting> M_LineTraceSettings;
};

UCLASS()
class FVM_API AOld_ATKCardActor : public AATKCardActorBase
{
	GENERATED_BODY()
public:
	//设置攻击类型的卡片数据
	UFUNCTION(BlueprintCallable)
		void SetCardData_ATK(const FItemCardATK& _ATK_Data);
public:
	//卡片攻击力设置
	UFUNCTION(BlueprintCallable)
		void SetCardATK(const float& _BaseATK, const float& _ATKRate, const int32& _CardGrade);
	//获取卡片攻击力
	UFUNCTION(BlueprintCallable)
		float GetCardATK();
	//设置第二攻击力
	UFUNCTION(BlueprintCallable)
		void SetCardSecondATK(const float& _ATKRate);
	//获取第二攻击力
	UFUNCTION(BlueprintCallable)
		float GetCardSecondATK();
	//设置攻击速度
	UFUNCTION(BlueprintCallable)
		void SetCardAttackSpeed(float _Value);
	//获取攻击速度
	UFUNCTION(BlueprintCallable)
		float GetCardAttackSpeed();
	//设置首次攻击速度
	UFUNCTION(BlueprintCallable)
		void SetCardFirstAttackSpeed(float _Value);
	//获取首次攻击速度
	UFUNCTION(BlueprintCallable)
		float GetCardFirstAttackSpeed();
	//设置连续攻击速度
	UFUNCTION(BlueprintCallable)
		void SetCardAttackProjectionSpeed(float _Value);
	//获取连续攻击速度
	UFUNCTION(BlueprintCallable)
		float GetCardAttackProjectionSpeed();
	//设置攻击个数
	UFUNCTION(BlueprintCallable)
		void SetCardAttackItemCount(int32 _Value);
	//获取攻击个数
	UFUNCTION(BlueprintCallable)
		int32 GetCardAttackItemCount();
	//获取射线偏移
	UFUNCTION(BlueprintCallable)
		const TArray<FLineTraceSetting>& GetLineTraceSetting();

	//初始化
	virtual void BeginPlay() override;
	//Tick
	virtual void Tick(float DeltaSeconds) override;
public:
	//卡片默认动画
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "卡片预设属性 | 动画")
		TSoftObjectPtr<UPaperFlipbook> CardActor_DefAnim;
	//卡片攻击动画
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "卡片预设属性 | 动画")
		TSoftObjectPtr<UPaperFlipbook> CardActor_AttackAnim;
	//卡片攻击的子弹
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "卡片预设属性 | 子弹对象类")
		TSoftClassPtr<AFlyItemActor> CardActor_BulletClassObj;
private:
	//卡片数据
	UPROPERTY()
		FItemCardATK ItemCardATK;
	//攻击类
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		FCard_ATK M_FCardActor_ATK;
};
