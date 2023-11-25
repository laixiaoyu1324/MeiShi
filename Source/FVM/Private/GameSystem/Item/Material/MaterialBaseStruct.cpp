// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除


#include "GameSystem/Item/Material/MaterialBaseStruct.h"
#include "GameSystem/GlobalDatas.h"

//--------------------------------------------------------------------------------------------------
//                       CPP全局材料转换函数【可以自由转换数据表的数据】
//--------------------------------------------------------------------------------------------------


class SpawnTypeSerachResultManager;

//材料数据结构转换
template<class T = FMaterial_CardSynthesisBlueprint_Data>
static void FMaterialBaseTransform(TArray<FMaterialBase*>& _SourceDatas, TArray<T>& _TransromDatas)
{
	for (auto& Data : _TransromDatas)
	{
		_SourceDatas.Emplace(&Data.M_FMaterial);
	}
}

//获取数据表所有数据
template <class DataTableRowType, class MaterailType = FMaterialBase>
static void GetMaterailRowDatas(TArray<DataTableRowType>& _Item_Rows, TArray<MaterailType>& _OutArrays, int32 _UniformCount = 1)
{
	DataTableRowType LocalTemp;
	for (auto Item = _Item_Rows.CreateConstIterator(); Item; ++Item)
	{
		LocalTemp = (*Item);
		LocalTemp.M_FMaterial.M_Count = _UniformCount;
		_OutArrays.Emplace(*((MaterailType*)(&LocalTemp.M_FMaterial)));
	}
}

//将材料数据结构表转换成数据组【根据指定类型的数据表继续转换->M_FMaterial】
template<class MaterialDataStructType, class TargetType>
static void MaterialsDataStructTransform(TArray<MaterialDataStructType>& Datas, TArray<TargetType>& _OutMaterials)
{
	for (auto& LData : Datas)
	{
		_OutMaterials.Emplace((*(TargetType*)(&LData.M_FMaterial)));
	}
}

//通过类型搜索结果
template<class TargetType = FMaterial_CardSynthesisBlueprint_Data>
void TypeSerachResult(const FString& _MaterailName, FMaterialBase& OutputData, bool _SelectType, EMaterialType _Material, TArray<FMaterialBase>& _Items, TArray<TargetType>& _SourceArrays, const EMaterialType& _MaterialConst, bool& _Result)
{
	//如果没有查询成功->才可以继续查询
	if (!_Result)
		if (!_SelectType || _SelectType && _Material == _MaterialConst)
		{
			MaterialsDataStructTransform<TargetType>(_SourceArrays, _Items);
			UMaterialBaseStruct::GetMaterialArraysData<FMaterialBase>(_MaterailName, _Items, OutputData, _Result);
		}
}

//生成一个资源获取管理器【通过名称查询所有的类型直到找到数据为止】
class SpawnTypeSerachResultManager {
private:
	FMaterialBase& M_OutputData;
	bool& M_Result;

	const FString& M_MaterailName;
	bool M_SelectType;
	EMaterialType M_Material;
private:
	TArray<FMaterialBase> M_Items;
public:
	SpawnTypeSerachResultManager() = delete;
	SpawnTypeSerachResultManager(const FString& _MaterailName, FMaterialBase& _OutputData, bool _SelectType, const EMaterialType& _Material, bool& _Result) :M_MaterailName(_MaterailName), M_OutputData(_OutputData), M_Result(_Result), M_SelectType(_SelectType), M_Material(_Material)
	{}
public:
	//搜寻结果
	template<class TargetType = FMaterial_CardSynthesisBlueprint_Data>
	void SpawnTypeSerach(TArray<TargetType>& _SourceArrays, const EMaterialType& _MaterialConst)
	{
		this->M_Items.Empty();
		TypeSerachResult<TargetType>(this->M_MaterailName, this->M_OutputData, this->M_SelectType, this->M_Material, this->M_Items, _SourceArrays, _MaterialConst, this->M_Result);
	}
};

//获取材料数据表所有数据【如果有新的材料加入->需要更新】
template <class MaterailType = FMaterialBase>
void GetMaterialTypeData(TArray<MaterailType>& _Item, int32 _UniformCount)
{
	TArray<FMaterialBase*> LocalDatas;

	for (uint8 LocalDefType = 0u; LocalDefType <= (uint8)EMaterialType::E_Max; LocalDefType++)
	{
	  UMaterialBaseStruct::GetSourceData(LocalDatas, (const EMaterialType&)LocalDefType);
	}

	for (auto Data : LocalDatas)
		_Item.Emplace(*Data);

}
//-------------------------------------------------------------------------------------------------------------------

//【根据类型获取源数据】
void UMaterialBaseStruct::GetSourceData(TArray<FMaterialBase*>& _Data, const EMaterialType& _Type)
{
	switch (_Type)
	{
		//新建数据源数组获取【合成配方】
	case EMaterialType::E_Blueprint:FMaterialBaseTransform(_Data, UGlobalDatas::Global_SourceMaterialData_Blueprint); break;
		//新建数据源数组获取【合成材料】....
	case EMaterialType::E_CardSynthesisMaterial:FMaterialBaseTransform(_Data, UGlobalDatas::Global_SourceMaterialData_BlueprintMaterial); break;

	case EMaterialType::E_CardChangeJobMaterial:FMaterialBaseTransform(_Data, UGlobalDatas::Global_SourceMaterialData_ChangeJobMaterial); break;
	case EMaterialType::E_Spices:FMaterialBaseTransform(_Data, UGlobalDatas::Global_SourceMaterialData_Spices); break;
	case EMaterialType::E_Clover:FMaterialBaseTransform(_Data, UGlobalDatas::Global_SourceMaterialData_Clover); break;
	case EMaterialType::E_CardSkillBook:FMaterialBaseTransform(_Data, UGlobalDatas::Global_SourceMaterialData_CardSkillBooks); break;
	case EMaterialType::E_Ticket:FMaterialBaseTransform(_Data, UGlobalDatas::Global_SourceMaterialData_Tickets); break;
	case EMaterialType::E_Crystal:FMaterialBaseTransform(_Data, UGlobalDatas::Global_SourceMaterialData_Crystals); break;
	case EMaterialType::E_Bit:FMaterialBaseTransform(_Data, UGlobalDatas::Global_SourceMaterialData_Bits); break;
	}
}
//【根据名称，指定类型，获取道具数据】
bool UMaterialBaseStruct::SearchMaterailFromDataTable(const FString& _MaterailName, FMaterialBase& OutputData, bool _SelectType, EMaterialType _Material)
{
	bool _Result = false;

	SpawnTypeSerachResultManager LocalSpawn(_MaterailName, OutputData, _SelectType, _Material, _Result);
	//新建类型查询【合成配方】
	LocalSpawn.SpawnTypeSerach(UGlobalDatas::Global_SourceMaterialData_Blueprint, EMaterialType::E_Blueprint);
	//新建类型查询【合成材料】......
	LocalSpawn.SpawnTypeSerach(UGlobalDatas::Global_SourceMaterialData_BlueprintMaterial, EMaterialType::E_CardSynthesisMaterial);
	LocalSpawn.SpawnTypeSerach(UGlobalDatas::Global_SourceMaterialData_ChangeJobMaterial, EMaterialType::E_CardChangeJobMaterial);
	LocalSpawn.SpawnTypeSerach(UGlobalDatas::Global_SourceMaterialData_Spices, EMaterialType::E_Spices);
	LocalSpawn.SpawnTypeSerach(UGlobalDatas::Global_SourceMaterialData_Clover, EMaterialType::E_Clover);
	LocalSpawn.SpawnTypeSerach(UGlobalDatas::Global_SourceMaterialData_CardSkillBooks, EMaterialType::E_CardSkillBook);
	LocalSpawn.SpawnTypeSerach(UGlobalDatas::Global_SourceMaterialData_Tickets, EMaterialType::E_Ticket);
	LocalSpawn.SpawnTypeSerach(UGlobalDatas::Global_SourceMaterialData_Crystals, EMaterialType::E_Crystal);
	LocalSpawn.SpawnTypeSerach(UGlobalDatas::Global_SourceMaterialData_Bits, EMaterialType::E_Bit);

	return _Result;
}

void UMaterialBaseStruct::GetAllMaterial(TArray<FMaterialBase>& _Item, int32 _UniformCount)
{
	GetMaterialTypeData(_Item, _UniformCount);
}

void UMaterialBaseStruct::GetAllMaterial(TArray<FItemBase>& _Item, int32 _UniformCount)
{
	GetMaterialTypeData(_Item, _UniformCount);
}
