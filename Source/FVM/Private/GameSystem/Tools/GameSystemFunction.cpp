// Fill out your copyright notice in the Description page of Project Settings.

#include "GameSystem/Tools/GameSystemFunction.h"
#include <Components/Button.h>
#include "UI/WidgetBase.h"

#include "GameStart/Flipbook/SpriteActor.h"
#include "GameStart/Flipbook/GameActorFlipbookBase.h"

#include "Paper2D/Classes/PaperFlipbook.h"

#include "GameStart/VS/GameMapInstance.h"
#include "GameStart/VS/Components/MesheControllComponent.h"
#include "GameStart/VS/MapMeshe.h"

#include <Kismet/KismetSystemLibrary.h>
#include <Kismet/KismetMathLibrary.h>

FDateTime UGameSystemFunction::GameSystem_Time;
FDateTime UGameSystemFunction::GameSystem_Time_Now;
FRandomStream UGameSystemFunction::Random_Now;
bool UGameSystemFunction::bEnable = false;

void UGameSystemFunction::UpdateTime()
{
	if (UGameSystemFunction::bEnable)
	{
		UGameSystemFunction::GameSystem_Time_Now = UGameSystemFunction::GameSystem_Time.Now();
		UGameSystemFunction::Random_Now.Initialize(UGameSystemFunction::GameSystem_Time_Now.GetHour() + UGameSystemFunction::GameSystem_Time_Now.GetMinute() + UGameSystemFunction::GameSystem_Time_Now.GetSecond() + UGameSystemFunction::GameSystem_Time_Now.GetMillisecond());
	}
}

void UGameSystemFunction::OpenUpdate()
{
	UGameSystemFunction::bEnable = true;
}

void UGameSystemFunction::CloseUpdate()
{
	UGameSystemFunction::bEnable = false;
}

void UGameSystemFunction::ClearTimeClip(FTimerManager& _TimeManager, FTimeClip& _Clip)
{
	_TimeManager.ClearTimer(_Clip.M_FTimerHandle);
	_Clip.M_Count = 0;
}

void UGameSystemFunction::SaveCurrentPlayerData()
{
	if (UFVMGameInstance::GetDebug())
		UGameSystemFunction::FVMLog(__FUNCTION__, TEXT("角色数据已保存"));

	UFVMGameInstance::GetFVMGameInstance()->SavePlayerStructConfig(UFVMGameInstance::GetFVMGameInstance()->GetGameConfigManager()->M_CurrentPlayerName);
}

void UGameSystemFunction::LoadCurrentPlayerData()
{
	if (UFVMGameInstance::GetDebug())
		UGameSystemFunction::FVMLog(__FUNCTION__, TEXT("角色数据已加载"));

	UFVMGameInstance::GetFVMGameInstance()->LoadPlayerStructConfig(UFVMGameInstance::GetFVMGameInstance()->GetGameConfigManager()->M_CurrentPlayerName);
}

void UGameSystemFunction::SaveCurrentGameConfigData()
{
	if (UFVMGameInstance::GetDebug())
		UGameSystemFunction::FVMLog(__FUNCTION__, TEXT("游戏配置数据已保存"));

	UFVMGameInstance::GetFVMGameInstance()->SaveDataToGameConfigManager();
}

void UGameSystemFunction::SendMailToCurrentPlayer(FMail& _Mail)
{
	//默认赋值ID从0 开始
	int32 _ID_Count = 0;
	for (FMail& Mail : UFVMGameInstance::GetFVMGameInstance()->GetPlayerStructManager()->M_PlayerMails)
	{
		//如果遇到相同的则叠加
		Mail.M_Mail_ID = _ID_Count;
		_ID_Count++;
	}

	_Mail.M_Mail_ID = _ID_Count + 1;

	UFVMGameInstance::GetFVMGameInstance()->GetPlayerStructManager()->M_PlayerMails.Emplace(_Mail);

	UGameSystemFunction::SaveCurrentPlayerData();
	UGameSystemFunction::LoadCurrentPlayerData();
}

FMaterialBase* UGameSystemFunction::GetMaterialDataFromBag(TArray<FMaterialBase>& _Arrays, const FString& _ItemName)
{
	for (FMaterialBase& Items : _Arrays)
	{
		if (Items.ItemName.Equals(_ItemName))
			return &Items;
	}
	return nullptr;
}

void UGameSystemFunction::GetMaterialsArrayByType(TArray<FMaterialBase>& _ArraysSource, const TArray<EMaterialType>& _Type, TArray<FMaterialBase*>& OutArrays)
{
	for (FMaterialBase& Item : _ArraysSource)
	{
		for (const auto& LType : _Type)
		{
			if (Item.M_MaterialType == LType)
			{
				OutArrays.Emplace(&Item);

				break;
			}
		}
	}
}

FString UGameSystemFunction::GetCardGradeImagePath(int32 _Grade)
{
	// Texture2D'/Game/Resource/Texture/CardGrade/1.1'

	if (_Grade == 0)
		return FString();

	FString _GradePath = "Texture2D'/Game/Resource/Texture/CardGrade/" + FString::FormatAsNumber(_Grade) + "." + FString::FormatAsNumber(_Grade) + "'";
	return _GradePath;
}

bool UGameSystemFunction::GetRange(int32 _ValueMaxRange, int32 _EndMaxRange)
{
	int32 _Result = UGameSystemFunction::Random_Now.RandRange(1, _EndMaxRange);;

	//UGameSystemFunction::FVMLog(__FUNCTION__, FString::FromInt(_ValueMaxRange) + TEXT(" <= ") + FString::FromInt(_Result));

	if (_ValueMaxRange >= _Result)
		return true;

	return false;
}

int32 UGameSystemFunction::GetRange_Int(int32 _ValueMaxRange, int32 _EndMaxRange)
{
	return UGameSystemFunction::Random_Now.RandRange(1, _EndMaxRange);
}

int32 UGameSystemFunction::GetRandomRange(int32 Random_Min, int32 RandomNums)
{
	return UGameSystemFunction::Random_Now.RandRange(Random_Min, RandomNums);
}

float UGameSystemFunction::GetRandomRangeFloat(float Random_Min, float RandomNums)
{
	return UGameSystemFunction::Random_Now.FRandRange(Random_Min, RandomNums);
}

void UGameSystemFunction::TabSelect(const TArray<UButton*>& _ButtonArrays, const  FString& _UniformPath, const  TArray<FString>& _DefButtonStyle, const TArray<FString>& _ClickButtonStyle, int32 _Index)
{
	//确保索引相同
	if (_ButtonArrays.Num() == _DefButtonStyle.Num() && _DefButtonStyle.Num() == _ClickButtonStyle.Num())
	{
		int32 Count = _ButtonArrays.Num();

		if (_Index < 0 || _Index >= Count)
			return;

		//先全部使用默认样式
		for (int32 i = 0; i < Count; i++)
		{
			FString Path = FString(_UniformPath + _DefButtonStyle[i] + TEXT(".") + _DefButtonStyle[i] + TEXT("'"));
			UWidgetBase::SetButtonStyle(_ButtonArrays[i], Path, true);
			_ButtonArrays[i]->SetVisibility(ESlateVisibility::Visible);
		}

		//激活指定的选项卡
		FString Path = FString(_UniformPath + _ClickButtonStyle[_Index] + TEXT(".") + _ClickButtonStyle[_Index] + TEXT("'"));
		UWidgetBase::SetButtonStyle(_ButtonArrays[_Index], Path, true);
		_ButtonArrays[_Index]->SetVisibility(ESlateVisibility::HitTestInvisible);
	}
}

void UGameSystemFunction::FVMLog(const FString& _FuncName, const FString& _Content)
{
	UE_LOG(LogTemp, Error, TEXT("%s %s"), *_FuncName, *_Content);
}

void UGameSystemFunction::LoadMap()
{
	if (UPlayerStructManager* Player = UFVMGameInstance::GetPlayerStructManager_Static())
	{
		FString MapPah = TEXT("");
		switch (Player->M_PlayerInMap.M_InMap)
		{
		case EGameVsMap::VM_Main:
			MapPah = TEXT("WidgetBlueprint'/Game/Resource/BP/Game/GameMap/MainMap.MainMap_C'");
			break;
		case EGameVsMap::VM_Map_1:
			MapPah = TEXT("WidgetBlueprint'/Game/Resource/BP/Game/GameMap/Map_000.Map_000_C'");
			break;
		case EGameVsMap::VM_Map_2:
			MapPah = TEXT("WidgetBlueprint'/Game/Resource/BP/Game/GameMap/Map_001.Map_001_C'");
			break;
		case EGameVsMap::VM_Map_3:
			MapPah = TEXT("WidgetBlueprint'/Game/Resource/BP/Game/GameMap/Map_003.Map_003_C'");
			break;
		case EGameVsMap::VM_Map_4:
			MapPah = TEXT("WidgetBlueprint'/Game/Resource/BP/Game/GameMap/Map_004.Map_004_C'");
			break;
		case EGameVsMap::VM_Map_5:
			MapPah = TEXT("WidgetBlueprint'/Game/Resource/BP/Game/GameMap/Map_005.Map_005_C'");
			break;
		}

		//设置地图
		UFVMGameInstance::GetFVMGameInstance()->SetWorldMapWidget(CreateWidget<UWidgetBase>(UFVMGameInstance::GetFVMGameInstance()->GetWorld(), LoadClass<UWidgetBase>(0, *MapPah)));

		UFVMGameInstance::CreateMainFramePanel();
	}

}

void UGameSystemFunction::LoadLastMap()
{
	if (UPlayerStructManager* Player = UFVMGameInstance::GetPlayerStructManager_Static())
	{
		FString MapPah = TEXT("");
		switch (Player->M_PlayerLastInMap.M_InMap)
		{
			//美味镇不能是上一次
		case EGameVsMap::VM_Main:
		case EGameVsMap::VM_Map_1:
			MapPah = TEXT("WidgetBlueprint'/Game/Resource/BP/Game/GameMap/Map_000.Map_000_C'");
			break;
		case EGameVsMap::VM_Map_2:
			MapPah = TEXT("WidgetBlueprint'/Game/Resource/BP/Game/GameMap/Map_001.Map_001_C'");
			break;
		case EGameVsMap::VM_Map_3:
			MapPah = TEXT("WidgetBlueprint'/Game/Resource/BP/Game/GameMap/Map_003.Map_003_C'");
			break;
		case EGameVsMap::VM_Map_4:
			MapPah = TEXT("WidgetBlueprint'/Game/Resource/BP/Game/GameMap/Map_004.Map_004_C'");
			break;
		case EGameVsMap::VM_Map_5:
			MapPah = TEXT("WidgetBlueprint'/Game/Resource/BP/Game/GameMap/Map_005.Map_005_C'");
			break;
		}

		//设置地图
		UFVMGameInstance::GetFVMGameInstance()->SetWorldMapWidget(CreateWidget<UWidgetBase>(UFVMGameInstance::GetFVMGameInstance()->GetWorld(), LoadClass<UWidgetBase>(0, *MapPah)));

		UFVMGameInstance::CreateMainFramePanel();
	}
}

void UGameSystemFunction::SetMap(EGameVsMap _Map)
{
	if (UPlayerStructManager* Player = UFVMGameInstance::GetPlayerStructManager_Static())
	{
		Player->M_PlayerInMap.M_InMap = _Map;

		UGameSystemFunction::SaveCurrentPlayerData();
	}
}

void UGameSystemFunction::SetLastMap(EGameVsMap _Map)
{
	if (UPlayerStructManager* Player = UFVMGameInstance::GetPlayerStructManager_Static())
	{
		Player->M_PlayerLastInMap.M_InMap = _Map;

		UGameSystemFunction::SaveCurrentPlayerData();
	}
}

FString UGameSystemFunction::GetPlayerGradeImagePath()
{
	UPlayerStructManager* Player = UFVMGameInstance::GetFVMGameInstance()->GetPlayerStructManager();
	FString _Grade_Path = TEXT("Texture2D'/Game/Resource/Texture/PlayerGrade/") + FString::FromInt(Player->M_FPlayerEx.M_PlayerGrade) + TEXT(".") + FString::FromInt(Player->M_FPlayerEx.M_PlayerGrade) + TEXT("'");
	return _Grade_Path;
}

EGameVsMap UGameSystemFunction::GetPlayerInMap()
{
	if (UPlayerStructManager* Player = UFVMGameInstance::GetPlayerStructManager_Static())
	{
		return Player->M_PlayerLastInMap.M_InMap;
	}

	return EGameVsMap::VM_Main;
}


EGameVsMap UGameSystemFunction::GetPlayerCurrentMap()
{
	if (UPlayerStructManager* Player = UFVMGameInstance::GetPlayerStructManager_Static())
	{
		return Player->M_PlayerInMap.M_InMap;
	}

	return EGameVsMap::VM_Main;
}

EGameVsMap UGameSystemFunction::GetPlayerLastInMap()
{
	return EGameVsMap::VM_Map_1;
}


void UGameSystemFunction::ResetDataTable(class UDataTable* _datatable)
{
	for (const auto& Name : _datatable->GetRowNames())
	{
		FItemCard* CardData = nullptr;

		FCard_Spawn_Data* CurRow = _datatable->FindRow<FCard_Spawn_Data>(Name, TEXT("Card"));
		if (CurRow)
			CardData = &CurRow->M_FItemCard;

		if (!CardData)
		{
			FCard_Defence_Data* CurRowA = _datatable->FindRow<FCard_Defence_Data>(Name, TEXT("Card"));
			if (CurRowA)
				CardData = &CurRowA->M_FItemCard;
		}

		if (!CardData)
		{
			FCard_RangeATK_Data* CurRowB = _datatable->FindRow<FCard_RangeATK_Data>(Name, TEXT("Card"));
			if (CurRowB)
				CardData = &CurRowB->M_FItemCard;
		}

		if (!CardData)
		{
			FCard_ATK_Data* CurRowC = _datatable->FindRow<FCard_ATK_Data>(Name, TEXT("Card"));
			if (CurRowC)
				CardData = &CurRowC->M_FItemCard;
		}

		if (CardData)
		{
			//加载Actor资源
			//FString Ref = FString("Blueprint'/Game/Resource/BP/GameStart/Item/Card/" + CardData->ItemName + "." + CardData->ItemName + "_C'");
			//CardData->CardActorResource = FSoftClassPath(Ref);
		//	CardData->CardActorHead = FSoftObjectPath(FString(TEXT("Texture2D'/Game/Resource/Texture/Sprite/VS/Sprite/Card/CardBg2/T") + CardData->ItemName + ".T" + CardData->ItemName + "'"));
		}
	}

	_datatable->MarkPackageDirty();
}

bool UGameSystemFunction::AddSphereTraceMulti(AActor* M_Owner, TArray<FHitResult>& OutActors, const FVector& _BeginOffset, const FVector& _EndOffset, float _Radius, ETraceTypeQuery _Type)
{

	if (!M_Owner)
		return false;

	if (M_Owner && !M_Owner->IsPendingKillPending())
	{
		TArray<AActor*> Actors;
		if (UKismetSystemLibrary::SphereTraceMulti(M_Owner->GetWorld(), M_Owner->GetActorLocation() + _BeginOffset, M_Owner->GetActorLocation() + _EndOffset, _Radius, _Type, false, Actors, EDrawDebugTrace::ForDuration, OutActors, false))
		{
			return true;
		}

		return false;
	}

	return false;
}

bool UGameSystemFunction::AddSphereTrace(AActor* M_Owner, FHitResult& OutActor, const FVector& _BeginOffset, const FVector& _EndOffset, float _Radius, ETraceTypeQuery _Type)
{
	if (!M_Owner)
		return false;

	if (M_Owner && !M_Owner->IsPendingKillPending())
	{
		TArray<AActor*> Actors;
		if (UKismetSystemLibrary::SphereTraceSingle(M_Owner->GetWorld(), M_Owner->GetActorLocation() + _BeginOffset, M_Owner->GetActorLocation() + _EndOffset, _Radius, _Type, false, Actors, EDrawDebugTrace::ForDuration, OutActor, false))
		{
			return true;
		}
	}

	return false;
}

AActor* UGameSystemFunction::CreateIceBlock(AActor* M_Owner, const FVector& _Offset, const int32& _Layer, float _Time)
{
	if (M_Owner && !M_Owner->IsPendingKillPending())
	{
		//创建冰块图层
		ASpriteActor* Obj = M_Owner->GetWorld()->SpawnActor<ASpriteActor>(LoadClass<ASpriteActor>(0, TEXT("Class'/Script/FVM.SpriteActor'")));
		//Obj->SetTranslucency(8000);
		Obj->SetLifeSpan(_Time);
		Obj->SetSprite(LoadObject<UPaperSprite>(0, TEXT("PaperSprite'/Game/Resource/Texture/Sprite/VS/Sprite/Items/19/t/T_public_Sprite.T_public_Sprite'")));
		Obj->SetSpriteOffset(_Offset);
		Obj->SetSpriteLocation(M_Owner->GetActorLocation(), true);
		Obj->SetTranslucency(_Layer);
		return Obj;
	}
	return nullptr;
}

void UGameSystemFunction::CreateMapMatrix(FLine _Line, int32 _RowCount, int32 _ColCount, FVector _Offset, FString _FlipBookAnimPath)
{
	UPaperFlipbook* Anim = LoadObject<UPaperFlipbook>(0, *_FlipBookAnimPath);

	if (!Anim)
		return;

	int32 MaxRows = AGameMapInstance::GetGameMapInstance()->GetMesheControllComponent()->GetMapMeshRowAndCol().Row;
	int32 MaxCols = AGameMapInstance::GetGameMapInstance()->GetMesheControllComponent()->GetMapMeshRowAndCol().Col;

	//确定初始索引(确定偏移)
	int32 LInitRow = _Line.Row - _RowCount / 2;
	for (int32 i = 0; i < _ColCount; i++, LInitRow++)
	{
		//确定初始索引(确定偏移)
		int32 LInitCol = _Line.Col - _ColCount / 2;
		for (int32 j = 0; j < _ColCount; j++, LInitCol++)
		{
			if (LInitRow >= 0 && LInitCol >= 0)
				if (LInitRow < MaxRows && LInitCol < MaxCols)
				{

					AMapMeshe* Mesh = AGameMapInstance::GetGameMapInstance()->
						GetMesheControllComponent()->GetMapMesh(LInitRow, LInitCol);

					FVector Location = AGameMapInstance::GetGameMapInstance()->
						GetMesheControllComponent()->GetMapMeshLocation(LInitRow, LInitCol);

					AGameActorFlipbookBase* AAnims = Mesh->GetWorld()->SpawnActor<AGameActorFlipbookBase>(
						LoadClass<AGameActorFlipbookBase>(0,
							TEXT("Class'/Script/FVM.GameActorFlipbookBase'")));
					AAnims->SetTranslucentSortPriority(8000);
					AAnims->InitRotation();
					AAnims->GetMyActor()->SetFlipbook(Anim);
					AAnims->SetAnimationPlayEndDestroy();
					AAnims->SetActorLocation(Location + _Offset);
				}
		}
	}
}




bool UGameSystemFunction::HitMouse(float _HitValue, AMouseActor* _Mouse, FItem_Buff& _Buff)
{
	if (_Mouse && !_Mouse->IsPendingKillPending())
	{
		//设置老鼠状态->被击中
		_Mouse->SetbIsHurt(true);
		//被命中传入伤害数值
		_Mouse->BeHit(_HitValue);
		//解析Buff信息
		_Mouse->ParseBuff_Information(_Buff);
		return true;
	}
	return false;
}

ETraceTypeQuery UGameSystemFunction::GetMouseCollisionTraceType(EMouseCollisionType _EMouseCollisionType)
{
	switch (_EMouseCollisionType)
	{
	case EMouseCollisionType::MouseGround:
		return ETraceTypeQuery::TraceTypeQuery4;
	case EMouseCollisionType::MouseSky:
		return ETraceTypeQuery::TraceTypeQuery5;
	case EMouseCollisionType::MouseUnder:
		return ETraceTypeQuery::TraceTypeQuery6;
	default:
		return ETraceTypeQuery::TraceTypeQuery11;
	}
	return ETraceTypeQuery::TraceTypeQuery11;
}

ETraceTypeQuery UGameSystemFunction::GetCardCollisionTraceType(ECardCollisionType _ECardCollisionType)
{
	switch (_ECardCollisionType)
	{
	case ECardCollisionType::E_CardActor2:
		return ETraceTypeQuery::TraceTypeQuery9;

	case ECardCollisionType::E_CardActor3:
		return ETraceTypeQuery::TraceTypeQuery10;
	default:
		return ETraceTypeQuery::TraceTypeQuery3;
	}
	return ETraceTypeQuery::TraceTypeQuery3;
}

ECollisionChannel UGameSystemFunction::GetCardCollisionBoxType(ECardCollisionType _ECardCollisionType)
{
	switch (_ECardCollisionType)
	{
	case ECardCollisionType::E_CardActor2:
		return ECollisionChannel::ECC_GameTraceChannel7;

	case ECardCollisionType::E_CardActor3:
		return ECollisionChannel::ECC_GameTraceChannel8;
	default:
		return ECollisionChannel::ECC_GameTraceChannel1;
	}
	return ECollisionChannel::ECC_GameTraceChannel1;
}

void UGameSystemFunction::CalculateLineTracePosition(const FVector& _Location, const FLine& _Line, UUI_MapMeshe* _UI_Meshe, const FLineTraceSetting& _LineTraceSetting, TArray<FLineTraceSetting>& _LineTraceSettingOut)
{
	//基本坐标 
	FVector BaseLocation = _Location;
	BaseLocation.X -= 5.f;

	float TargetLY = _UI_Meshe->GetMapMeshCurrentLeft(_Line.Row).Y - 55.f;
	float TargetRY = _UI_Meshe->GetMapMeshCurrentRight(_Line.Row).Y + 55.f;
	FVector Top = _UI_Meshe->GetMapMeshCurrentTop(_Line.Col);
	FVector Bottom = _UI_Meshe->GetMapMeshCurrentBottom(_Line.Col);
	float GridCountMax = _LineTraceSetting.M_GridCount * (55.f);

	FLineTraceSetting _Item;

	switch (_LineTraceSetting.M_ELineTracePosition)
	{
	case ELineTracePosition::E_Left:
	{
		FVector LocationEnd = FVector(BaseLocation.X, TargetLY, BaseLocation.Z);
		_Item.M_BeginLocation = BaseLocation;
		_Item.M_EndLocation = LocationEnd;
	}
	break;
	case ELineTracePosition::E_Right:
	{
		FVector LocationEnd = FVector(BaseLocation.X, TargetRY, BaseLocation.Z);

		_Item.M_BeginLocation = BaseLocation;

		_Item.M_EndLocation = _LineTraceSetting.M_bEnableSettingGridCount ? BaseLocation + FVector(0.f, GridCountMax, 0.f) : LocationEnd;

		if (_Item.M_EndLocation.Y > LocationEnd.Y)
			_Item.M_EndLocation = LocationEnd;

	}
	break;
	case ELineTracePosition::E_LeftRight_M:
	{
		FVector LocationBegin = FVector(BaseLocation.X, TargetLY, BaseLocation.Z);
		FVector LocationEnd = FVector(BaseLocation.X, TargetRY, BaseLocation.Z);
		_Item.M_BeginLocation = LocationBegin;
		_Item.M_EndLocation = LocationEnd;
	}
	break;
	case ELineTracePosition::E_Top:
	{
		Top.X = BaseLocation.X;
		Top.Y = BaseLocation.Y;
		Top.Z += 45.f;
		_Item.M_BeginLocation = BaseLocation;
		_Item.M_EndLocation = Top;
	}
	break;
	case ELineTracePosition::E_Bottom:
	{
		Bottom.X = BaseLocation.X;
		Bottom.Y = BaseLocation.Y;
		Bottom.Z -= 45.f;
		_Item.M_BeginLocation = BaseLocation;
		_Item.M_EndLocation = Bottom;
	}
	break;
	case ELineTracePosition::E_TopBottom_M:
	{
		Top.X = BaseLocation.X;
		Top.Y = BaseLocation.Y;
		Top.Z += 45.f;
		Bottom.X = BaseLocation.X;
		Bottom.Y = BaseLocation.Y;
		Bottom.Z -= 45.f;
		_Item.M_BeginLocation = Top;
		_Item.M_EndLocation = Bottom;
	}
	break;
	}

	//加上偏移量
	_Item.M_BeginLocation += _LineTraceSetting.M_LineTraceOffset;
	_Item.M_EndLocation += _LineTraceSetting.M_LineTraceOffset;

	//UE_LOG(LogTemp, Error, TEXT("%.2f  %.2f %.2f"), _LineTraceSetting.M_LineTraceOffset.X, _LineTraceSetting.M_LineTraceOffset.Y, _LineTraceSetting.M_LineTraceOffset.Z);

	_LineTraceSettingOut.Emplace(_Item);
}

void UGameSystemFunction::SetGlobalGameTime(const UObject* WorldContextObject, float _TimeRate)
{
	UGameplayStatics::SetGlobalTimeDilation(WorldContextObject, _TimeRate);
}

//发送补偿邮件
void UGameSystemFunction::SendVersionCompensate()
{
	//获取存档
	if (UPlayerStructManager* Player = UFVMGameInstance::GetFVMGameInstance()->GetPlayerStructManager())
	{
		TArray<FPlayerVersionCompensate_Data*> Datas;
		//获取数据表
		UGameSystemFunction::GetDataTableRows<FPlayerVersionCompensate_Data>(TEXT("DataTable'/Game/Resource/BP/Data/EquipmentData/GameVersionCompensate.GameVersionCompensate'"), Datas);

		//获取总数量
		int32 DataCount = Datas.Num();
		//获取背包存储数量
		int32 Count = Player->M_GameVersionCompensate.Num();

		if (UFVMGameInstance::GetDebug())
			UGameSystemFunction::FVMLog(__FUNCTION__, FString("----------------------------------------------------------------"));

		if (UFVMGameInstance::GetDebug())
			UGameSystemFunction::FVMLog(__FUNCTION__, FString(TEXT("当前邮件数量：") + FString::FromInt(Count)) + TEXT("总量：") + FString::FromInt(DataCount));

		//防止出现版本更新时-已经领取的补偿-重复领取
		for (auto& Item : Player->M_GameVersionCompensate)
		{
			Item.M_SendState = true;
		}

		//计算需要添加多少补偿
		int32 Result = DataCount - Count;

		if (Result > 0)
		{

			if (UFVMGameInstance::GetDebug())
				UGameSystemFunction::FVMLog(__FUNCTION__, FString(TEXT("需要补发数量：") + FString::FromInt(Result)));

			for (int i = Count; i < DataCount; i++)
			{
				FPlayerVersionCompensate Data = Datas[i]->M_FPlayerVersionCompensate;
				Player->M_GameVersionCompensate.Emplace(Data);
			}

			for (auto& Item : Player->M_GameVersionCompensate)
			{
				if (!Item.M_SendState)
				{
					if (UGameSystemFunction::SendMailToPlayerMailBox(Item.M_SendMailName))
					{
						if (UFVMGameInstance::GetDebug())
							UGameSystemFunction::FVMLog(__FUNCTION__, FString(TEXT("发送补偿方案：") + Item.M_SendMailName));

						Item.M_SendState = true;
					}
				}
				else {
					if (UFVMGameInstance::GetDebug())
						UGameSystemFunction::FVMLog(__FUNCTION__, FString(TEXT("当前补偿方案失效：") + Item.M_SendMailName));
				}
			}
		}

		if (UFVMGameInstance::GetDebug())
			UGameSystemFunction::FVMLog(__FUNCTION__, FString("----------------------------------------------------------------"));

		//保存
		UGameSystemFunction::SaveCurrentPlayerData();
	}
}

//发送邮件给玩家
bool UGameSystemFunction::SendMailToPlayerMailBox(const FString& _Name)
{
	FMail _MailData;

	TArray<FMail> Mail_;

	UEquipmentBaseStruct::GetEquipmentRowDatas<FEquipment_FMail_Data, FMail>(UGlobalDatas::Global_SourceMailData_Mail, Mail_);

	if (UEquipmentBaseStruct::GetMailArraysData(_Name, Mail_, _MailData))
	{
		UGameSystemFunction::SendMailToCurrentPlayer(_MailData);
		//保存数据
		UGameSystemFunction::SaveCurrentPlayerData();

		return true;
	}
	return false;
}

//获取指定技能书的等级
int32 UGameSystemFunction::GetPlayerSkillBooksLv(const FString& _Name)
{
	for (const auto& Item : UFVMGameInstance::GetPlayerStructManager_Static()->M_SkillBookDatas)
	{
		if (_Name.Equals(Item.M_Name))
			return Item.M_LV;
	}
	return 0;
}

//想背包发送指定的防御卡
bool UGameSystemFunction::SendCardToPlayerBag(const FString& _CardName, int32 _Lv)
{
	FItemCard CardData;
	if (UCardBaseStruct::SearchCardFromDataTable(_CardName, CardData))
	{
		CardData.M_CardGrade = _Lv;
		UFVMGameInstance::GetPlayerStructManager_Static()->M_PlayerItems_Card.Emplace(CardData);
		UGameSystemFunction::SaveCurrentPlayerData();

		if (UFVMGameInstance::GetDebug())
			UGameSystemFunction::FVMLog(__FUNCTION__, TEXT("发送防御卡：") + _CardName + TEXT("到背包"));

		return true;
	}

	if (UFVMGameInstance::GetDebug())
		UGameSystemFunction::FVMLog(__FUNCTION__, TEXT("发送防御卡失败：") + _CardName);

	return false;
}

//发送指定装备到背包
bool UGameSystemFunction::SendEquipmentToPlayerBag(const FString& _EquipmentName, int32 _Count, bool _bSave)
{
	if (_Count > 9999)
		_Count = 9999;

	if (_Count < 1)
		_Count = 1;

	FEquipmentBase EquipmentData;

	if (UEquipmentBaseStruct::SearchEquipmentFromDataTable(_EquipmentName, EquipmentData))
	{
		//当前物品是可以叠加的道具
		if (EquipmentData.M_IsOverlap)
		{
			//查询结果
			bool LResult = false;

			//背包查询->是否有重复的->如果有则直接添加对应个数
			for (auto& Item : UFVMGameInstance::GetPlayerStructManager_Static()->M_PlayerItems_Equipment)
			{
				if (_EquipmentName.Equals(Item.ItemName))
				{
					//根据类型发送背包
					if (Item.M_IsOverlap)
					{
						if (Item.M_Count + _Count > 9999)
							Item.M_Count = 9999;

						Item.M_Count += _Count;
					}
					LResult = true;
					break;
				}
			}

			if (!LResult)
			{
				//根据类型发送背包
				EquipmentData.M_Count = _Count;
				//申请ID
				UFVMGameInstance::GetPlayerStructManager_Static()->AddEquipmentToPlayerBag(EquipmentData);
				UFVMGameInstance::GetPlayerStructManager_Static()->M_PlayerItems_Equipment.Emplace(EquipmentData);
			}
		}
		else {
			for (int32 i = 0; i < _Count; i++)
			{
				//申请ID
				UFVMGameInstance::GetPlayerStructManager_Static()->AddEquipmentToPlayerBag(EquipmentData);
				UFVMGameInstance::GetPlayerStructManager_Static()->M_PlayerItems_Equipment.Emplace(EquipmentData);
			}
		}

		if (_bSave)
			UGameSystemFunction::SaveCurrentPlayerData();

		if (UFVMGameInstance::GetDebug())
			UGameSystemFunction::FVMLog(__FUNCTION__, TEXT("发送装备：") + EquipmentData.ItemName + TEXT("到背包"));

		return true;
	}

	if (UFVMGameInstance::GetDebug())
		UGameSystemFunction::FVMLog(__FUNCTION__, TEXT("发送装备失败：") + EquipmentData.ItemName);

	return false;
}

//发送指定材料到背包
bool UGameSystemFunction::SendMaterialToPlayerBag(const FString& _MaterialName, int32 _Count, bool _bSave)
{
	if (_Count > 1000000000)
		_Count = 1000000000;

	if (_Count < 1)
		_Count = 1;

	FMaterialBase MaterialBaseData;
	if (UMaterialBaseStruct::SearchMaterailFromDataTable(_MaterialName, MaterialBaseData))
	{
		//查询结果
		bool LResult = false;
		//背包查询->是否有重复的->如果有则直接添加对应个数
		for (auto& Item : UFVMGameInstance::GetPlayerStructManager_Static()->M_PlayerItems_Material)
		{
			if (_MaterialName.Equals(Item.ItemName))
			{
				//个数判断
				if (Item.M_Count + _Count > 1000000000)
				{
					Item.M_Count = 1000000000;
				}
				else {
					Item.M_Count += _Count;
				}

				LResult = true;
				break;
			}
		}

		//查询失败
		if (!LResult)
		{
			//未查询到
			MaterialBaseData.M_Count = _Count;
			UFVMGameInstance::GetPlayerStructManager_Static()->M_PlayerItems_Material.Emplace(MaterialBaseData);
		}

		if (UFVMGameInstance::GetDebug())
			UGameSystemFunction::FVMLog(__FUNCTION__, TEXT("发送材料：") + MaterialBaseData.ItemName + TEXT("到背包"));

		//保存数据
		if (_bSave)
			UGameSystemFunction::SaveCurrentPlayerData();

		return true;
	}

	if (UFVMGameInstance::GetDebug())
		UGameSystemFunction::FVMLog(__FUNCTION__, TEXT("发送材料失败：") + MaterialBaseData.ItemName);

	return false;
}

//获取指向玩家存档卡片的数据(建立在加载存档之后)
void UGameSystemFunction::GetPlayerCardDatas(TArray<FItemCard*>& _datas, int32 _Count)
{
	int32 _Num_ = 0;
	for (auto& Item : UFVMGameInstance::GetPlayerStructManager_Static()->M_PlayerItems_Card)
	{
		_datas.Emplace(&Item);

		if (_Count > 0)
		{
			_Num_++;
			if (_Num_ >= _Count)
				break;
		}

	}
}

//获取玩家存档卡片数据的引用(建立在加载存档之后)
TArray<FItemCard>& UGameSystemFunction::GetPlayerCardDatasByRef()
{
	return UFVMGameInstance::GetPlayerStructManager_Static()->M_PlayerItems_Card;
}

int32 UGameSystemFunction::SerachPlayerEquipmentIndexByID(const FString& _ItemName, int32 _EquipID, uint8 _SerarchType)
{
	UPlayerStructManager* LPlayer = UFVMGameInstance::GetPlayerStructManager_Static();

	//2分查找  找出ID
	int32 LIndex = (LPlayer->M_PlayerItems_Equipment.Num() / 2) - 1;

	if (LIndex > _EquipID && _SerarchType == 0 || _SerarchType == 1u)
	{
		//通过前进行查找
		for (int32 I = 0; I <= LIndex; ++I)
		{
			if (LPlayer->M_PlayerItems_Equipment[I].M_ItemID == _EquipID && _ItemName.Equals(LPlayer->M_PlayerItems_Equipment[I].ItemName))
			{
				//查询成功
				return I;
			}
		}
	}

	if (LIndex < _EquipID && _SerarchType == 0 || _SerarchType == 2u)
	{
		//通过后面进行查找
		for (int32 I = LIndex; I < LPlayer->M_PlayerItems_Equipment.Num(); ++I)
		{
			if (LPlayer->M_PlayerItems_Equipment[I].M_ItemID == _EquipID && _ItemName.Equals(LPlayer->M_PlayerItems_Equipment[I].ItemName))
			{
				//查询成功
				return I;
			}
		}

	}

	//2分的结果刚好就是当前ID
	if (LPlayer->M_PlayerItems_Equipment[LIndex].M_ItemID == _EquipID && _ItemName.Equals(LPlayer->M_PlayerItems_Equipment[LIndex].ItemName))
	{
		//查询成功
		return LIndex;
	}

	return -1;
}

FString UGameSystemFunction::GetItemGradePath(int32 _Grade)
{
	if (_Grade >= 1 && _Grade <= 16)
		return FString(TEXT("Texture2D'/Game/Resource/Texture/CardGrade/") + FString::FromInt(_Grade) + TEXT(".") + FString::FromInt(_Grade) + TEXT("'"));

	return FString();
}

void UGameSystemFunction::AddString(FString& _CurrentString, const FString& _Content)
{
	if (!_CurrentString.Contains(_Content))
		_CurrentString += _Content;
}


//删除玩家背包中有待删除的道具->争对装备道具【会改变数组的内存地址】自动调用RemoveEquipmentFromPlayerBag
void UGameSystemFunction::ClearWaitingItemsForEquip(TArray<FEquipmentBase>& _Arrays)
{
	TArray<FEquipmentBase> _Buff;

	for (FEquipmentBase& _Items : _Arrays)
	{
		//将未标记的物品添加到缓冲区(满足条件的物品将得到保留，不需要销毁ID)
		if (!_Items.bWaitRemove)
			_Buff.Emplace(_Items);
		else {
			//销毁ID号
			UFVMGameInstance::GetPlayerStructManager_Static()->RemoveEquipmentFromPlayerBag(_Items);
		}
	}
	//清空
	_Arrays.Empty();
	//重新复制新值
	_Arrays = _Buff;
}




