#pragma once

// #include "GameSystem/PlayerStructManager.h"

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"

#include "GameSystem/Tools/GameSystemFunction.h"

#include "GameSystem/Item/PlayerStructBase.h"
#include "Data/CardData/CardDataStruct.h"
#include "Data/EquipmentDataSturct.h"
#include "Data/MaterialDataStruct.h"
//数据表数据-技能书(由GameInstance自动检测更新)
#include "Data/CardSkillBookStruct.h"

#include "PlayerStructManager.generated.h"

/**
 *  角色存储结构
 */



UCLASS()
class FVM_API UPlayerStructManager : public USaveGame
{
	GENERATED_BODY()
		//-----------------------------------------------角色主要处理-----------------------------------------------
public:
	//角色版本
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString M_GameVersion = "";
	// 0是女生  1是男生
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		uint8 M_PlayerSex;
	//角色名称
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString M_PlayerName = FString(TEXT("面包干&芙蓉"));
	//角色等级
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FPlayerEx M_FPlayerEx;
	//角色货币
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FPlayerCoin M_FPlayerCoin;
	//角色VIP
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FPlayerVIP M_FPlayerVIP;
	//角色称号和描述
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FPlayerSign M_FPlayerSign;
	//背包界面中的背包容量
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FPlayerBagGirdGroup M_PlayerBagGirdGroup;
	//角色着装
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FPlayerSuit M_FPlayerSuit;
	//关卡评级
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TMap<FString, FPlayerLevelGrade> M_PlayerLevelScore;
	//道具分解兑换抽奖信息存储
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TMap<FString, FPlayerExchangeAndSplit> M_FPlayerExchangeAndSplitInfor;
public:
	//玩家当前进入的地图
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FPlayerInMap M_PlayerInMap;
	//玩家上一次的地图
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FPlayerInMap M_PlayerLastInMap;
public:
	//背包 _防御卡
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<FItemCard> M_PlayerItems_Card;
	//背包_装备
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<FEquipmentBase> M_PlayerItems_Equipment;

	/*-----------------------装备区域的ID存储库------------------------*/
	//背包_装备ID号(可以使用的)
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<int32> M_PlayerItems_Equip_ID;
	//背包_装备ID号(被占用的)
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<int32> M_PlayerItems_Equip_IDUse;
	/*----------------------------------------------------------------*/


	//背包_材料
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<FMaterialBase> M_PlayerItems_Material;
	//背包邮件
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<FMail> M_PlayerMails;
	//技能书数据(存储信息表)
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<FSkillBookData> M_SkillBookDatas;

	/*-----------------------装备区域的武器数据存储库------------------------*/
	//玩家武器数据
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<FPlayerWeaponBase> M_FPlayerWeaponDatas;
	/*-----------------------装备区域的武器数据存储库------------------------*/


	/*-----------------------装备区域的宝石据存储库--------------------------*/
	//玩家宝石数据
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<FWeaponGem> M_FPlayerWeaponGemDatas;
	/*-----------------------装备区域的宝石数据存储库------------------------*/

	//版本补偿
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<FPlayerVersionCompensate> M_GameVersionCompensate;
public:
	//获取背包道具数量(获取当前背包道具现有的总数量)
	int32 GetBagNum(int32 _Index);
	//获取背包最大个数(150+背包拓展容量，并非道具总数量)
	int32 GetBagMaxCount(int32 _Index);
public:
	//通过名称获取装备
	FEquipmentBase* GetEquipmentByName(const FString& _Name);
	//通过名称获取卡片
	FItemCard* GetCardByName(const FString& _Name);
	//通过名称获取材料
	FMaterialBase* GetMaterialByName(const FString& _Name);
public:
	//获取当前玩家的等级图片路径
	UFUNCTION(BlueprintCallable)
		FString GetGradeImagePath();
public:
	//增加硬币
	UFUNCTION(BlueprintCallable)
		bool AddCoin(int32 _Num, uint8 _Type = 0);
	//减少硬币
	UFUNCTION(BlueprintCallable)
		bool ReduceCoin(int32 _Num, uint8 _Type = 0);
	//为当前角色添加经验
	UFUNCTION(BlueprintCallable)
		static void AddPlayerEx(float _ExValue);
	//添加关卡评级
	UFUNCTION(BlueprintCallable)
		static void AddPlayerLevelScore(const FString& _LevelName, const FPlayerLevelGrade& _Grade);
public:
	/*---------------------------------ID库操作开始---------------------------------*/
	//返回一个可使用的ID
	int32 GetEquipID();
	//销毁一个ID
	void DestroyEquipID(int32 _ID);
	//刷新ID库->是否扩容（如果容量大于ID库-扩容ID）
	void UpdateEquipID(int32 _BagCount);
	//使用装备->通过ID使用->将对应绑定的装备进行装备使用
	void UseEquipByID(int32 _ID);
	//取消使用装备->通过ID号对改ID对应的装备进行装备取消
	void CancelEquipByID(int32 _ID);
	//添加装备到角色背包【只会注册ID号和存储库】
	bool AddEquipmentToPlayerBag(FEquipmentBase& _WeaponData);
	//将当前装备从背包移除【只会销毁ID和存储库】
	void RemoveEquipmentFromPlayerBag(FEquipmentBase& _WeaponData);
	/*---------------------------------ID库操作结束---------------------------------*/

	/*------------------------------玩家武器数据功能开始 ->针对主武器-副武器-超级武器------------------------------*/
	//ID查询-武器库
	bool SearchPlayerWeaponDataByID(int32 _WeaponID);
	//通过ID查询武器库对应位置(传入ID查询，返回武器在库中的位置) 返回ID值 如果不等于-1 表示ID有效
	int32 SearchPlayerWeaponDataByID_A(int32 _WeaponID, int32& _WeaponIndex);
	//添加武器库-数据
	void AddPlayerWeaponData(FEquipmentBase& _WeaponData);
	/*------------------------------玩家武器数据功能结束------------------------------*/

	/*------------------------------玩家武器数据功能开始------------------------------*/
	//添加宝石库-数据
	void AddPlayerWepaonGemData(FEquipmentBase& _WeaponData);
	/*------------------------------玩家武器数据功能结束------------------------------*/


	/*------------------------------玩家特殊的装备道具查询【模板】------------------------------*/

	//查询特殊数据存储库的对应ID号
	template<class TEquipType>
	static int32 SerachEquipDataByID(int32 _ID, const TArray<TEquipType>& _Datas)
	{
		int32 LIndex = 0;

		//if (UFVMGameInstance::GetDebug())
		//	UGameSystemFunction::FVMLog(__FUNCTION__, FString(TEXT("传入ID：") + FString::FromInt(_ID)));

		for (const auto& Data : _Datas)
		{
			//	if (UFVMGameInstance::GetDebug())
			//	{
			//		UGameSystemFunction::FVMLog(__FUNCTION__, FString(FString(Data.ItemName) + TEXT("ID：") + FString::FromInt(Data.M_ItemID)));
			//	}

			if (Data.M_ItemID == _ID)
			{
				//		if (UFVMGameInstance::GetDebug())
				//			UGameSystemFunction::FVMLog(__FUNCTION__, TEXT("道具：") + Data.ItemName + TEXT("ID查询成功"));

				return LIndex;
			}

			LIndex++;
		}

		//if (UFVMGameInstance::GetDebug())
		//	UGameSystemFunction::FVMLog(__FUNCTION__, TEXT("武器查询失败，并没有找到对应ID"));

		return -1;
	}

	//保存数据
	static void Save();

	//移除道具->ID不会移除
	template <class ItemStruct>
	static void Remove_Item(TArray<ItemStruct>& _Arrays)
	{
		//获取没有被标记的道具
		TArray<ItemStruct> Type;

		for (TIndexedContainerIterator<const TArray<ItemStruct>, const ItemStruct, int32> PP = _Arrays.CreateConstIterator(); PP; PP++)
			if (!(*PP).bWaitRemove)
			{
				Type.Emplace((*PP));
			}

		_Arrays.Empty();

		//复制新的卡片数据
		_Arrays = Type;

		//保存数据
		UPlayerStructManager::Save();
		//UGameSystemFunction::SaveCurrentPlayerData();
	}
};

//货币计算
USTRUCT(BlueprintType)
struct FPlayerCoinAdd
{
	GENERATED_USTRUCT_BODY()
public:
	static bool _Buy(int32& _Coin, const int32& _Num)
	{
		if (_Coin < 0)
			_Coin = 0;

		if (_Coin < _Num)
			return false;

		_Coin -= _Num;
		return true;
	}
	static bool _Sell(int32& _Coin, const int32& _Num)
	{
		if (_Coin < 0)
			_Coin = 0;

		_Coin += _Num;

		if (_Coin > 1000000000)
			_Coin = 1000000000;

		return true;
	}
public:
	//获取货币名称
	static TArray<FItemBase> GetCoinNames()
	{
		return { FItemBase(TEXT("金币")),FItemBase(TEXT("礼券")),FItemBase(TEXT("点券")),FItemBase(TEXT("威望")) };
	}
public:
	FPlayerCoinAdd() {}

	static bool Buy(UPlayerStructManager* _Player, const int32& _Num, const uint8& _CoinGrade) {
		if (!_Player)
			return false;

		return _Buy(_Player->M_FPlayerCoin.GetCoinRef(_CoinGrade), _Num);
	}

	static bool Sell(UPlayerStructManager* _Player, const int32& _Num, const uint8& _CoinGrade) {
		if (!_Player)
			return false;

		return _Sell(_Player->M_FPlayerCoin.GetCoinRef(_CoinGrade), _Num);
	}
};
