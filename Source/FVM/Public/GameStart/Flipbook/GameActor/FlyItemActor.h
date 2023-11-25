// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameStart/Flipbook/GameActorFlipbookBase.h"
#include "GameStart/Flipbook/GameActor/MouseActor.h"
#include "GameStart/VS/MapBaseType.h"
#include "GameStart/VS/Components/Item/ShootLineComponent.h"
#include "FlyItemActor.generated.h"

class AMouseActor;
class ACardActor;
class UActorComponent;
class UBoxComponent;
class USphereComponent;

//可以被其他组件影响的标签
UENUM(BlueprintType)
enum class FFlyItem_Influence_Tag : uint8
{
	//线性子弹可以反弹
	ERebound UMETA(Display = "Rebound"),
	//子弹可以提升攻击力
	EATK_Up UMETA(Display = "ATK_Up"),
	//线性子弹可以穿过
	EAcross UMETA(Display = "Across"),
	//可以添加Buff
	EBuff UMETA(Display = "Buff"),
};

//飞行物携带属性-BGM
USTRUCT(BlueprintType)
struct FlyItem_Property_AudioPath {
	GENERATED_USTRUCT_BODY()
public:
	//BGM的根目录名称
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString M_AudioRootPathName = FString("");
	//BGM的名称
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString M_AudioName = FString("");
};

//飞行物携带属性-动画资源(Fly & Split)
USTRUCT(BlueprintType)
struct FlyItem_Property_AnimRes {
	GENERATED_USTRUCT_BODY()
public:
	//飞行物动画资源Fly
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSoftObjectPtr<UPaperFlipbook> M_FlyItemAnim_Fly;
	//飞行物动画资源Split
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSoftObjectPtr<UPaperFlipbook> M_FlyItemAnim_Split;
public:
	UPROPERTY()
		UPaperFlipbook* M_Begin = nullptr;
	UPROPERTY()
		UPaperFlipbook* M_End = nullptr;
};


//飞行物数据
USTRUCT(BlueprintType)
struct FlyItem_PropertyData {
	GENERATED_USTRUCT_BODY()
public:
	//当前的攻击线路(如果需要用到该条件)
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 M_Line = 0;
	//攻击力
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float ATK = 1.f;
	//当前攻击力
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float CurATK = 1.f;
	//第二攻击力
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float _SecondATK = 0.f;
};

//飞行物条件
USTRUCT(BlueprintType)
struct FlyItem_PropertyCondition {
	GENERATED_USTRUCT_BODY()
public:
	//是否约束飞行物对行的重叠
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool M_bConstraintLine = false;
	//是否可以被其他组件影响(其他组件可以修改数据)
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool M_bInfluence = false;
	//是否击中目标
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool M_bHitTarget = false;
	//是否是静态飞行物
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool M_bStaticFlyItem = false;
	//是否开启AOE(范围同时击中)
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool M_bAOE = false;
};


UCLASS()
class FVM_API AFlyItemActor : public AGameActorFlipbookBase
{
	GENERATED_BODY()
private:
	//防止卡片的功能组件（某些功能）重复使用飞行物
	UPROPERTY()
		UActorComponent* M_AActorComponent_CardFunction = nullptr;
	//如果开启了浮动模式（子弹将不在受行限制）
	UPROPERTY()
		bool M_bFloatMode = false;
public:
	UActorComponent* const GetActor_CardFunction_Component();
	void SetActor_CardFunction_Component(UActorComponent* _Value);
private:
	//老鼠的位置
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		AActor* M_MouseActorLocation = nullptr;
	//当前击中的老鼠
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		AMouseActor* M_CurrentHitMouseActor = nullptr;
	//飞行物数据
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		FlyItem_PropertyData M_FlyData;
public:
	//可击打类型
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FlyItemActorProperty | AttackType")
		TArray<ELineType> M_AttackType;
	//携带的buff
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FlyItemActorProperty | AddBuffer")
		FItem_Buff M_FItem_Buff;
	//可以被其他组件影响的标签
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FlyItemActorProperty | Condition")
		TArray<FFlyItem_Influence_Tag> M_FlyItem_Influence_Tag;
	//飞行物条件
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FlyItemActorProperty | Condition")
		FlyItem_PropertyCondition M_FlyCondition;
	//处于边界外被销毁
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FlyItemActorProperty | Condition")
		bool M_OutsideAutoDestroy = true;
	//初始化BGM
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FlyItemActorProperty | Audio | Begin")
		FlyItem_Property_AudioPath M_FlyItem_Property_AudioBegin;
	//结束BGM
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FlyItemActorProperty | Audio | End")
		FlyItem_Property_AudioPath M_FlyItem_Property_AudioEnd;
	//静态对象池(线路约束，限制到本行)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FlyItemActorProperty | StaticItems")
		bool M_StaticFlyItemLocationConstLine = true;
	//静态对象池(有几个就会创建几个)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FlyItemActorProperty | StaticItems")
		TArray<TSoftClassPtr<AFlyItemActor>> M_StaticFlyItemClass;
	//飞行物动画资源
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FlyItemActorProperty | FlyItemAnimationRes")
		FlyItem_Property_AnimRes M_FlyItem_Property_AnimRes;
public:
	//飞行物碰撞
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		USphereComponent* M_SphereCollision = nullptr;
public:
	AFlyItemActor();
	void InitCollision();
	void BeginPlay() override;

public:
	//设置浮动模式
	UFUNCTION(BlueprintCallable)
		void SetFloatModeEnable(bool _Value);
	//设置飞行物约束
	UFUNCTION(BlueprintCallable)
		void SetFlyConstraintLine(bool _value);
	//设置可以被其他对象修改(影响)
	UFUNCTION(BlueprintCallable)
		void SetFlyInfluence(bool _value);
	//设置攻击力
	UFUNCTION(BlueprintCallable)
		void SetATK(float _value);
	//设置当前攻击力
	UFUNCTION(BlueprintCallable)
		void SetCurATK(float _value);
	//设置第二攻击力
	UFUNCTION(BlueprintCallable)
		void SetSecondATK(float _value);
	//设置线路
	UFUNCTION(BlueprintCallable)
		void SetLine(int32 _line);
	//设置翻书动画
	UFUNCTION(BlueprintCallable)
		void SetFlipbook(TSubclassOf<UPaperFlipbook>& _Res);
	//设置老鼠Actor
	UFUNCTION(BlueprintCallable)
		void SetMouseActorLocation(AActor* _MouseActor);
	//设置目标击中状态
	UFUNCTION(BlueprintCallable)
		void SetTargetHitState(bool _Value);
	//碰撞的开启与关闭
	UFUNCTION(BlueprintCallable)
		void SetCollisionEnabled(bool _Value);
public:
	//添加Buff
	void AddBuff(TMap<Buff_Infor, float>& _buffers);
	//添加可以攻击类型
	void AddAttackType(TArray<ELineType>& _types);

	//添加Buff
	UFUNCTION(BlueprintCallable)
		void AddBuff(Buff_Infor& _buff, const float& _time);
	//添加可以攻击类型
	UFUNCTION(BlueprintCallable)
		void AddAttackType(ELineType _type);
public:
	//获取是否处于边界之外被销毁
	UFUNCTION(BlueprintCallable)
		bool GetFlyOutsideAutoDestroy();
	//获取约束
	UFUNCTION(BlueprintCallable)
		bool GetFlyConstraintLine();
	//获取攻击力
	UFUNCTION(BlueprintCallable)
		float GetATK();
	//获取当前攻击力
	UFUNCTION(BlueprintCallable)
		float GetCurATK();
	//获取第二攻击力
	UFUNCTION(BlueprintCallable)
		float GetSecondATK();
	//获取线路
	UFUNCTION(BlueprintCallable)
		int32 GetLine();
	//当前飞行物是否可以被影响
	UFUNCTION(BlueprintCallable)
		bool GetFlyInfluence();
	//获取老鼠的位置
	UFUNCTION(BlueprintCallable)
		FVector GetMouseActorLocation();
	//获取buff
	UFUNCTION(BlueprintCallable)
		FItem_Buff& GetBuff();
	//获取老鼠ActorLocation
	UFUNCTION(BlueprintCallable)
		AActor* const GetMouseActor();
	//获取可以攻击的类型
	UFUNCTION(BlueprintCallable)
		TArray<ELineType>& GetAttackType();
	//目标是否被击中
	UFUNCTION(BlueprintCallable)
		bool GetDidTargetIsHit();
public:
	//播放开始动画
	UFUNCTION(BlueprintCallable)
		void PlayAnim_Fly();
	//播放结束动画
	UFUNCTION(BlueprintCallable)
		void PlayAnim_Split();
	//击中是否成功（bool 设置行约束->只能攻击本行单位   int32 是约束行）
	//UFUNCTION(BlueprintCallable)
	//	bool Hit(AActor* _mouse);
	//击中结果
	UFUNCTION(BlueprintCallable)
		void HitEnd(UPrimitiveComponent* _UBoxComp);
	//击中(由老鼠或者其他对象调用)
	UFUNCTION(BlueprintCallable)
		void Hit();
	//当击中目标时
	UFUNCTION(BlueprintImplementableEvent)
		void OnHit();
	//创建一个静态物品->作用可以进行二次伤害（范围伤害等）
	UFUNCTION(BlueprintCallable)
		void CreateStaticItem(TSoftClassPtr<AFlyItemActor> CurFlyItemActor);
	//创建一个flyActor直线
	UFUNCTION(BlueprintCallable)
		void CreateFlyActor_ShootLine(TSoftClassPtr<AFlyItemActor> _FlyActorPath_C, FTargetNode _Node, int32 _LineOffset, float _Time = 10.f, FVector Offset = FVector(0.f), bool _IsbConstaintLine = true);
	//创建一个flyActor斜线
	UFUNCTION(BlueprintCallable)
		void CreateFlyActor_ShootLine_Slash(TSoftClassPtr<AFlyItemActor> _FlyActorPath_C, float _RotationAngle = 45.f, float _Time = 10.f, FVector Offset = FVector(0.f));
	//飞行物交替
	UFUNCTION(BlueprintCallable)
		AFlyItemActor* FlyItemActorSwap(AFlyItemActor* _FlyActor);
public:
	//更新旋转360
	UFUNCTION(BlueprintCallable)
		void UpdateRatation360(float _deltaTime, USceneComponent* _Comp);
public:
	//与老鼠发生重叠
	UFUNCTION(BlueprintCallable)
		void HitMouse_OverlapBegin(AActor* _Mouse);
public:
	//当飞行物开始时与其他对象重叠
	UFUNCTION()
		void OnBoxOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	//当飞行物与其他对象重叠结束时
	UFUNCTION()
		void OnBoxOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};
