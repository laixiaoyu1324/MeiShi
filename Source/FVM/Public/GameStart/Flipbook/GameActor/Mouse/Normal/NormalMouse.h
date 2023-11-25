// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除

#pragma once

#include "CoreMinimal.h"
#include "GameStart/Flipbook/GameActor/Mouse/Normal/NormalBase.h"
#include "Data/CardData/MouseDataStruct.h"
#include "GameSystem/Tools/GameSystemFunction.h"
#include "NormalMouse.generated.h"

/**
 *
 */

 //普通老鼠资源结构(陆地普通无任何装饰)->一般作为老鼠class的一个属性使用
USTRUCT(BlueprintType)
struct FNormalMouseResourceStruct {
	GENERATED_USTRUCT_BODY()
public:
	//老鼠资源的路径
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString M_MousePath = FString(TEXT(""));
	//正常形态(No Attack)
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString M_MouseNomalFlipbookName = FString(TEXT("Normal"));
	//残血形态(No Attack)
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString M_MouseResidualBloodFlipbookName = FString(TEXT("ResidualBlood"));
	//正常形态(Attack)
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString M_MouseAttackNomalFlipbookName = FString(TEXT("AttackNormal"));
	//残血形态(Attack)
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString M_MouseAttackResidualBloodFlipbookName = FString(TEXT("AttackResidualBlood"));
	//死亡形态
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString M_MouseDeathedFlipbookName = FString(TEXT("Deathed"));
	//是否有初始动画(被什么对象生成(生成动画))
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool M_bEnableCreateAnim = false;
	//被创建形态
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString M_MouseCreateFlipbookName = FString(TEXT("Create"));
};

class UBoxComponent;

UCLASS()
class FVM_API ANormalMouse : public ANormalBase
{
	GENERATED_BODY()
public:
	//网格碰撞组件
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UBoxComponent* M_MesheComponent = nullptr;
	//身体碰撞组件
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UBoxComponent* M_BodyComponent = nullptr;
public:
	//老鼠的动画资源
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FNormalMouseResourceStruct M_MouseResource;
	//可以攻击卡片的什么类型
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		ECardCollisionType M_ECardCollisionType = ECardCollisionType::E_CardActor2;
private:
	//是否启动攻击线
	UPROPERTY()
		bool M_bEnableAttakLine = true;
	//是否处于创建动画状态
	UPROPERTY()
		bool M_bCreatting = false;
public:
	//构造
	ANormalMouse();
	//初始化
	virtual void BeginPlay() override;
	//更新
	virtual void Tick(float DeltaTime) override;
	//动画播放完毕
	void OnAnimationPlayEnd();
public:
	//移动逻辑函数
	virtual void MoveingUpdate(float DeltaTime) override;
public:
	//当老鼠被命中时受到的伤害数值
	virtual bool BeHit(float _HurtValue) override;
	//当老鼠被命中时结束之后(未受到攻击时)
	virtual void UnHit() override;
	//老鼠开始攻击
	virtual void AttackedBegin() override;
	//老鼠攻击结束
	virtual void AttackedEnd() override;
	//当老鼠死亡时
	virtual void MouseDeathed() override;
public:
	//状态
	void State();
};
