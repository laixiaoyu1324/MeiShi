// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GameSystem/Item/Card/CardBaseStruct.h"
#include "CardFunctionStruct.generated.h"

//功能结构-效果只会执行一次-爆炸效果
USTRUCT(BlueprintType)
struct FFunction_Bomb_Once {
	GENERATED_USTRUCT_BODY();
public:
	//爆炸效果
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool M_Bomb = false;
	//默认3*3范围
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float Radius = 90.f;
	//开始位置
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FVector2D BeginPosition;
	//结束位置
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FVector2D EndPosition;
	//攻击力
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float ATK = 10.f;
	//攻击力加成比例
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float ATKRate = 0.5f;
	//卡片等级提示的额外攻击力加成比例
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float CardGradeATKRate = 1.f;
	//额外的buff
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FItem_Buff M_Buffs;
};

//功能结构-效果只会执行一次
USTRUCT(BlueprintType)
struct Function_Once {
	GENERATED_USTRUCT_BODY();
public:
	//黑夜转换白天状态
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool M_TransfromNigntToDay = false;
	//白天转换黑天状态
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool M_TransfromDayToNignt = false;
	//刷新当前卡片冷却
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool M_RefreshCurrentCardCoodDown = false;
	//刷新所有卡片冷却(冷却功能卡除外)
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool M_RefreshAllCardCoodDown = false;
	//使用上一次放置的卡片(在卡片死亡时触发)
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool M_UseBackCard = false;
	//随机放置一张卡片
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool M_RandomPlayCard = false;
	//爆炸效果
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FFunction_Bomb_Once M_Bomb;
};

//功能结构-通过碰撞执行的
USTRUCT(BlueprintType)
struct Function_Collision {
	GENERATED_USTRUCT_BODY();
public:
	//反弹物体
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool M_Rebound_Item = false;

	//增加伤害(ATK_Up)
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool M_UpATK = false;
	//伤害增加比例(M_UpATK影响)
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float M_UpATK_Rate = 1.f;

	//携带(Buff)
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool M_Buff_Func = false;
	//携带buff
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TMap<Buff_Infor, float> M_Buff_Set;

	//穿过
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool M_Across_Func = false;
	//子弹穿越【火盆】特殊功能(直线冰冻子弹转换)
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool M_Across_Brazier_Func = false;
	//更正的动画资源(当子弹穿过时可以更正外观)
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FSoftClassPath M_Across_BulletClass;
};

//功能结构-生长功能
USTRUCT(BlueprintType)
struct Function_Grow {
	GENERATED_USTRUCT_BODY();
public:
	//是否开启生长功能(需要设置卡片的动画资源(def,grow,finish))
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool M_Grow_Func = false;
	//生长时间（time-time*rate*grade*grade_rate/time）
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float M_GrowTime = 12.f;
	//生长时间缩短比率（等级和等级提升比率的影响）
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float M_GrowTimeRate = 0.3f;
	//生长音效
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString M_GrowAudioName;
};

//功能结构-受到伤害后自毁爆炸(被攻击或者碰撞都会触发)
USTRUCT(BlueprintType)
struct Function_BeHurtBomb {
	GENERATED_USTRUCT_BODY();
public:
	//受到伤害后爆炸(被攻击或者碰撞都会触发)并且会自动开启(使用一次则自毁)
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool M_BeHurtBomb_Func = false;
	//连接生长功能（生长完成后执行此功能，如果不关联则动画播放完毕自毁后执行此功能）
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool M_LinkGrowFunc = false;
	//爆炸伤害
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float M_BombATK = 50.f;
	//爆炸的半径
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float M_Radius = 45.f;
	//额外的buff
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FItem_Buff M_Buffs;
	//开始位置
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FVector BeginPosition;
	//结束位置
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FVector EndPosition;
	//检测老鼠的类型
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		EMouseCollisionType M_MouseCollisionType;
	//爆炸翻书动画路径
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FSoftObjectPath M_BombAnimPath;
	//爆炸音效
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString M_BombAudioName;
};

//功能结构-卡片激活时
USTRUCT(BlueprintType)
struct Function_Active {
	GENERATED_USTRUCT_BODY();
public:
	//卡片激活时
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool M_CardActive = false;
	//是否关闭碰撞
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool M_CollisionEnable = false;
	//是否切换动画资源
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool M_CardAnimChange = false;
	//更正的动画资源(当子弹穿过时可以更正外观)
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FSoftObjectPath M_ChangeAnim;
};

//功能类型防御卡
USTRUCT(BlueprintType)
struct FItemCardFunction : public FItemCard {
	GENERATED_USTRUCT_BODY();
public:
	//激活状态-当卡片处于激活状态时就会触发
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		Function_Active M_Function_Active;
	//开启碰撞功能(开启之后会自动绑定碰撞功能,关闭之后不做任何处理)
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool M_Enable_CollisionFunc = false;
	//碰撞-每一次碰撞都会触发
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		Function_Collision M_Function_Collision;
	//功能-只会触发一次
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		Function_Once M_Function_Once;
	//【一次性】碰撞爆炸自毁功能(受到攻击会触发，受到碰撞功能影响)
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		Function_BeHurtBomb M_Function_BeHurtBomb;
	//使用一次就自毁
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool M_UseOnceKill = false;
	//【持续】生长功能-一定时候后卡片进行形态上的变化(需要自己设置卡片形态(def,grow,finish))
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		Function_Grow M_Function_Grow;
};

UCLASS()
class FVM_API UCardFunctionStruct : public UObject
{
	GENERATED_BODY()

};
