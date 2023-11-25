// Fill out your copyright notice in the Description page of Project Settings.


#include "GameSystem/PlayerStructManager.h"
#include "GameSystem/Tools/GameSystemFunction.h"


int32 UPlayerStructManager::GetBagNum(int32 _Index)
{
	switch (_Index)
	{
	case 0:return this->M_PlayerItems_Equipment.Num();
	case 1:return this->M_PlayerItems_Card.Num();
	case 2:return this->M_PlayerMails.Num();
	}
	return 0;
}

int32 UPlayerStructManager::GetBagMaxCount(int32 _Index)
{
	return this->M_PlayerBagGirdGroup.GetBagCount(_Index);
}

FEquipmentBase* UPlayerStructManager::GetEquipmentByName(const FString& _Name)
{
	return UItemBaseStruct::GetMetaByName<FEquipmentBase>(this->M_PlayerItems_Equipment, _Name);
}

FItemCard* UPlayerStructManager::GetCardByName(const FString& _Name)
{
	return UItemBaseStruct::GetMetaByName<FItemCard>(this->M_PlayerItems_Card, _Name);
}

FMaterialBase* UPlayerStructManager::GetMaterialByName(const FString& _Name)
{
	return UItemBaseStruct::GetMetaByName<FMaterialBase>(this->M_PlayerItems_Material, _Name);
}

FString UPlayerStructManager::GetGradeImagePath()
{
	FString _Grade_Path = TEXT("Texture2D'/Game/Resource/Texture/PlayerGrade/") + FString::FromInt(this->M_FPlayerEx.M_PlayerGrade) + TEXT(".") + FString::FromInt(this->M_FPlayerEx.M_PlayerGrade) + TEXT("'");
	return _Grade_Path;
}

bool UPlayerStructManager::AddCoin(int32 _Num, uint8 _Type)
{
	return FPlayerCoinAdd::Sell(this, _Num, _Type);
}

bool UPlayerStructManager::ReduceCoin(int32 _Num, uint8 _Type)
{
	return FPlayerCoinAdd::Buy(this, _Num, _Type);
}

void UPlayerStructManager::AddPlayerEx(float _ExValue)
{
	UPlayerStructManager* PlayerData = UFVMGameInstance::GetPlayerStructManager_Static();

	//如果角色60级则不在增加经验
	if (PlayerData->M_FPlayerEx.M_PlayerGrade >= 60)
	{
		PlayerData->M_FPlayerEx.M_CurrentEx = PlayerData->M_FPlayerEx.M_CurrentExTop;
		return;
	}


	//叠加当前经验值
	PlayerData->M_FPlayerEx.M_CurrentEx += _ExValue;

	//将溢出的经验值保留，并且等级提升一级
	if (PlayerData->M_FPlayerEx.M_CurrentEx >= PlayerData->M_FPlayerEx.M_CurrentExTop)
	{
		//最高等级限制 60
		if (PlayerData->M_FPlayerEx.M_PlayerGrade >= 60)
		{
			PlayerData->M_FPlayerEx.M_CurrentEx = PlayerData->M_FPlayerEx.M_CurrentExTop;
			return;
		}

		PlayerData->M_FPlayerEx.M_CurrentEx -= PlayerData->M_FPlayerEx.M_CurrentExTop;
		PlayerData->M_FPlayerEx.M_PlayerGrade++;

		//下一次所需经验值提升  100 + 10 * 0.4
		PlayerData->M_FPlayerEx.M_CurrentExTop += (PlayerData->M_FPlayerEx.M_CurrentExTop + PlayerData->M_FPlayerEx.M_ExUp) * PlayerData->M_FPlayerEx.M_ExUpRate;
	}

	//如果当前经验值依旧大于所需经验值-则递归
	if (PlayerData->M_FPlayerEx.M_CurrentEx >= PlayerData->M_FPlayerEx.M_CurrentExTop)
		UPlayerStructManager::AddPlayerEx(0);
}

void UPlayerStructManager::AddPlayerLevelScore(const FString& _LevelName, const FPlayerLevelGrade& _Grade)
{
	UPlayerStructManager* PlayerData = UFVMGameInstance::GetPlayerStructManager_Static();

	const FPlayerLevelGrade* CurLevel = PlayerData->M_PlayerLevelScore.Find(_LevelName);
	if (CurLevel)
	{
		FPlayerLevelGrade CurGrade = *CurLevel;
		CurGrade.Init(_Grade.Time, _Grade.Score, _Grade.BadCardCount);
		CurGrade.InitScoreLevel(_Grade.BadCardCount);
		PlayerData->M_PlayerLevelScore.Emplace(_LevelName, CurGrade);
	}
	else {
		PlayerData->M_PlayerLevelScore.Emplace(_LevelName, _Grade);
	}
}

int32 UPlayerStructManager::GetEquipID()
{
	if (this->M_PlayerItems_Equip_ID.Num() > 0)
	{
		//获取当前可以使用的ID
		int32 LocalID = this->M_PlayerItems_Equip_ID[0];

		//移出当前ID
		this->M_PlayerItems_Equip_ID.RemoveAt(0);

		//将当前ID占用
		this->M_PlayerItems_Equip_IDUse.Emplace(LocalID);

		if (UFVMGameInstance::GetDebug())
		{
			UGameSystemFunction::FVMLog(__FUNCTION__, FString(TEXT("ID被标记为使用，被标记的ID:") + FString::FromInt(LocalID)));
		}

		return LocalID;
	}

	return -1;
}

void UPlayerStructManager::DestroyEquipID(int32 _ID)
{
	int32 Index = 0;
	bool bResult = false;
	for (auto& Id : this->M_PlayerItems_Equip_IDUse)
	{
		if (Id == _ID)
		{
			bResult = true;
			break;
		}
		Index++;
	}

	if (bResult)
	{
		if (UFVMGameInstance::GetDebug())
		{
			UGameSystemFunction::FVMLog(__FUNCTION__, FString(TEXT("开始销毁ID，当前已经被销毁的ID：") + FString::FromInt(this->M_PlayerItems_Equip_IDUse[Index])));
		}

		//将当前占用的ID 取消还原
		this->M_PlayerItems_Equip_ID.Emplace(this->M_PlayerItems_Equip_IDUse[Index]);
		//移出原来被占用的ID
		this->M_PlayerItems_Equip_IDUse.RemoveAt(Index);
	}
}

void UPlayerStructManager::UpdateEquipID(int32 _BagCount)
{
	//获取当前ID库总量
	int32 Total = this->M_PlayerItems_Equip_ID.Num() + this->M_PlayerItems_Equip_IDUse.Num();

	//需要添加多个ID库
	if (Total < _BagCount)
	{
		int32 NeedTotal = _BagCount - Total;
		while (NeedTotal > 0)
		{
			if (UFVMGameInstance::GetDebug())
			{
				UGameSystemFunction::FVMLog(__FUNCTION__, FString(TEXT("ID库更新，当前加入的新ID：") + FString::FromInt(this->M_PlayerItems_Equip_ID.Num() + this->M_PlayerItems_Equip_IDUse.Num())));
			}

			//添加ID
			this->M_PlayerItems_Equip_ID.Emplace(this->M_PlayerItems_Equip_ID.Num() + this->M_PlayerItems_Equip_IDUse.Num());

			NeedTotal--;
		}
	}
}

void UPlayerStructManager::UseEquipByID(int32 _ID)
{
	for (auto& Equip : this->M_PlayerItems_Equipment)
	{
		if (Equip.M_ItemID == _ID)
		{
			Equip.M_Used = true;

			break;
		}
	}
}

void UPlayerStructManager::CancelEquipByID(int32 _ID)
{
	for (auto& Equip : this->M_PlayerItems_Equipment)
	{
		if (Equip.M_ItemID == _ID)
		{
			Equip.M_Used = false;

			break;
		}
	}
}

bool UPlayerStructManager::AddEquipmentToPlayerBag(FEquipmentBase& _WeaponData)
{
	if (_WeaponData.M_ItemID != -1)
		return false;

	//申请ID
	int32 ID = this->GetEquipID();

	if (ID != -1)//ID申请成功
		_WeaponData.M_ItemID = ID;
	else
		return false;//ID 申请失败直接退出

	switch (_WeaponData.M_EquipmentType)
	{
		//武器
	case EEquipment::E_PlayerWeaponFirst:
	case EEquipment::E_PlayerWeaponSecond:
	case EEquipment::E_PlayerWeaponSuper: {this->AddPlayerWeaponData(_WeaponData); }break;
		//宝石
	case EEquipment::E_WeaponGem:this->AddPlayerWepaonGemData(_WeaponData); break;

	default:
		return true;
	}

	return true;
}

void UPlayerStructManager::RemoveEquipmentFromPlayerBag(FEquipmentBase& _WeaponData)
{
	_WeaponData.bWaitRemove = true;

	//不管是什么道具  -1 是永远不会出现在任何存储库里面的[可以直接删除]
	if (_WeaponData.M_ItemID == -1)
	{
		UGameSystemFunction::FVMLog(__FUNCTION__, TEXT("当前道具ID为-1：") + _WeaponData.ItemName);

		return;
	}

	//销毁道具库存档的数据
	switch (_WeaponData.M_EquipmentType)
	{
		//武器
	case EEquipment::E_PlayerWeaponFirst:
	case EEquipment::E_PlayerWeaponSecond:
	case EEquipment::E_PlayerWeaponSuper: {

		int32 LID = -1;
		if (UFVMGameInstance::GetPlayerStructManager_Static()->SearchPlayerWeaponDataByID_A(_WeaponData.M_ItemID, LID) != -1)
		{
			UGameSystemFunction::FVMLog(__FUNCTION__, TEXT("从武器库销毁一个道具：") + _WeaponData.ItemName);

			UFVMGameInstance::GetFVMGameInstance()->GetPlayerStructManager()->M_FPlayerWeaponDatas[LID].bWaitRemove = true;

			UPlayerStructManager::Remove_Item<FPlayerWeaponBase>(UFVMGameInstance::GetFVMGameInstance()->GetPlayerStructManager()->M_FPlayerWeaponDatas);
		}
		else {
			UGameSystemFunction::FVMLog(__FUNCTION__, TEXT("从武器库销毁道具失败，不存在的道具：") + _WeaponData.ItemName);
		}

	}break;
		//宝石
	case EEquipment::E_WeaponGem: {

		//映射ID->匹配宝石库
		//查询宝石数据
		int32 LID = UPlayerStructManager::SerachEquipDataByID(_WeaponData.M_ItemID, UFVMGameInstance::GetPlayerStructManager_Static()->M_FPlayerWeaponGemDatas);
		if (LID != -1)
		{
			UGameSystemFunction::FVMLog(__FUNCTION__, TEXT("从宝石库销毁一个道具：") + _WeaponData.ItemName);

			UFVMGameInstance::GetFVMGameInstance()->GetPlayerStructManager()->M_FPlayerWeaponGemDatas[LID].bWaitRemove = true;

			UPlayerStructManager::Remove_Item<FWeaponGem>(UFVMGameInstance::GetFVMGameInstance()->GetPlayerStructManager()->M_FPlayerWeaponGemDatas);
		}
		else {
			UGameSystemFunction::FVMLog(__FUNCTION__, TEXT("从宝石库销毁道具失败，不存在的道具：") + _WeaponData.ItemName);
		}

	} break;

	default:break;
	}

	//销毁ID号
	this->DestroyEquipID(_WeaponData.M_ItemID);

	//销毁道具
	//UGameSystemFunction::ClearWaitingItemsForEquip(UFVMGameInstance::GetFVMGameInstance()->GetPlayerStructManager_Static()->M_PlayerItems_Equipment);
}

bool UPlayerStructManager::SearchPlayerWeaponDataByID(int32 _WeaponID)
{
	return UPlayerStructManager::SerachEquipDataByID<FPlayerWeaponBase>(_WeaponID, this->M_FPlayerWeaponDatas) != -1 ? true : false;
}

int32 UPlayerStructManager::SearchPlayerWeaponDataByID_A(int32 _WeaponID, int32& _WeaponIndex)
{
	_WeaponIndex = SerachEquipDataByID(_WeaponID, this->M_FPlayerWeaponDatas);

	return _WeaponIndex != -1 ? _WeaponID : -1;
}

void UPlayerStructManager::AddPlayerWeaponData(FEquipmentBase& _WeaponData)
{
	//没有ID则无法入库
	if (_WeaponData.M_ItemID == -1)
		return;

	//类型添加(根据类型设置具体属性)
	FPlayerWeaponBase PlayerWeaponBaseData;
	FEquipmentBase* LData = nullptr;
	if (UEquipmentBaseStruct::SearchSourceEquipmentFromDataTable(_WeaponData.ItemName, LData, true, _WeaponData.M_EquipmentType))
	{
		PlayerWeaponBaseData = *(FPlayerWeaponBase*)(LData);

		PlayerWeaponBaseData.M_ItemID = _WeaponData.M_ItemID;

		this->M_FPlayerWeaponDatas.Emplace(PlayerWeaponBaseData);

		if (UFVMGameInstance::GetDebug())
		{
			UGameSystemFunction::FVMLog(__FUNCTION__, TEXT("将武器:") + PlayerWeaponBaseData.ItemName + TEXT("添加到武器存储库中，ID：") + FString::FromInt(PlayerWeaponBaseData.M_ItemID));
		}
	}
}

void UPlayerStructManager::AddPlayerWepaonGemData(FEquipmentBase& _WeaponData)
{
	//没有ID则无法入库
	if (_WeaponData.M_ItemID == -1)
		return;

	TArray<FEquipmentBase*> Gems;
	UEquipmentBaseStruct::GetAllEquipmentsSourceData(Gems, true, { EEquipment::E_WeaponGem });
	//初始化宝石数据
	FWeaponGem LFWeaponGem;
	for (auto& SourceData : Gems)
	{
		if ((FWeaponGem*)SourceData->ItemName.Equals(_WeaponData.ItemName))
		{
			LFWeaponGem = *(FWeaponGem*)SourceData;
			break;
		}
	}

	//ID绑定
	LFWeaponGem.M_ItemID = _WeaponData.M_ItemID;

	//设置等级
	if (LFWeaponGem.M_GemGradeMax >= _WeaponData.M_EquipmentGrade)
		LFWeaponGem.M_GemGrade = _WeaponData.M_EquipmentGrade;
	else
		LFWeaponGem.M_GemGrade = LFWeaponGem.M_GemGradeMax;

	this->M_FPlayerWeaponGemDatas.Emplace(LFWeaponGem);

	if (UFVMGameInstance::GetDebug())
	{
		UGameSystemFunction::FVMLog(__FUNCTION__, TEXT("将道具:") + LFWeaponGem.ItemName + TEXT("添加到宝石存储库中，ID：") + FString::FromInt(_WeaponData.M_ItemID));
	}
}

void UPlayerStructManager::Save()
{
	UGameSystemFunction::SaveCurrentPlayerData();
}
