// Fill out your copyright notice in the Description page of Project Settings.


#include "GameStart/VS/Components/MouseManagerComponent.h"

#include "GameStart/Flipbook/GameActor/MouseActor.h"

#include "GameSystem/FVMGameInstance.h"

#include "GameStart/VS/PlayerView.h"
#include "GameStart/VS/GameMapInstance.h"
#include "GameStart/VS/MapMeshe.h"

#include "GameStart/VS/Components/MesheControllComponent.h"
#include "GameStart/VS/Components/VSManagerComponent.h"

#include "GameStart/VS/UI/UI_MouseRoundUpTip.h"
#include "GameStart/VS/UI/UI_MouseRound.h"
#include "GameStart/VS/UI/UI_MouseBoss_Slider.h"

#include <Blueprint/UserWidget.h>
#include <Kismet/GameplayStatics.h>
#include <Math/RandomStream.h>
#include <Kismet/KismetMathLibrary.h>


void UMouseLineManager::AddMouse(AMouseActor* _CurMouse)
{
	if (IsValid(_CurMouse))
	{
		switch (_CurMouse->GetMouseLineType())
		{
		case ELineType::Sky://添加到空中
			this->CurMouseSky.Emplace(_CurMouse);
			break;
		default://添加到陆地【默认老鼠行】
			this->CurMouseGround.Emplace(_CurMouse);
			break;
		}
	}
}

void UMouseLineManager::RemoveMouse(AMouseActor* _CurMouse)
{
	if (IsValid(_CurMouse))
	{
		switch (_CurMouse->GetMouseLineType())
		{
		case ELineType::Sky://从空中移除
			this->CurMouseSky.Remove(_CurMouse);
			break;
		default://从陆地移除【默认老鼠行】
			this->CurMouseGround.Remove(_CurMouse);
			break;
		}
	}
}

void UMouseLineManager::SortMouseByTick(const float& _Tick)
{
	if (CurTime < this->UpdateTickTime)
	{
		CurTime += _Tick;
	}
	else {
		CurTime = 0;

		//update

		//排序老鼠位置
		this->CurMouseGroundTop = this->SortMouseTopLocation(this->CurMouseGround);
		if (!IsValid(this->CurMouseGroundTop))
		{
			this->CurMouseGround.Remove(this->CurMouseGroundTop);
		}

		this->CurMouseUnderGroundTop = this->SortMouseTopLocation(this->CurMouseUnderGround);
		if (!IsValid(this->CurMouseUnderGroundTop))
		{
			this->CurMouseUnderGround.Remove(this->CurMouseUnderGroundTop);
		}

		this->CurMouseSkyTop = this->SortMouseTopLocation(this->CurMouseSky);
		if (!IsValid(this->CurMouseSkyTop))
		{
			this->CurMouseSky.Remove(this->CurMouseSkyTop);
		}
	}
}

AMouseActor* UMouseLineManager::GetMouseTopByGorund()
{
	return this->CurMouseGroundTop;
}

AMouseActor* UMouseLineManager::GetMouseTopByUnderGorund()
{
	return this->CurMouseUnderGroundTop;
}

AMouseActor* UMouseLineManager::GetMouseTopBySky()
{
	return this->CurMouseSkyTop;
}

AMouseActor* UMouseLineManager::GetMouseTopByAllType()
{
	//得到所有类型最前【忽略地下，因为目前没有任何卡片可以直接追踪地下，妨碍大部分卡片，追踪打不到的地下】

	if (!IsValid(this->CurMouseSkyTop) && !IsValid(this->CurMouseGroundTop))
	{
		return nullptr;
	}

	//如果陆地有效
	if (IsValid(this->CurMouseGroundTop) && !IsValid(this->CurMouseSkyTop))
	{
		return this->CurMouseGroundTop;
	}

	//如果空中有效
	if (IsValid(this->CurMouseSkyTop) && !IsValid(this->CurMouseGroundTop))
	{
		return this->CurMouseSkyTop;
	}

	//判断空中和陆地谁最前【相同则陆地优先】
	if (IsValid(this->CurMouseSkyTop) && IsValid(this->CurMouseGroundTop))
	{
		if (this->CurMouseSkyTop->GetActorLocation().Y < this->CurMouseGroundTop->GetActorLocation().Y)
		{
			return this->CurMouseSkyTop;
		}
		else {
			return this->CurMouseGroundTop;
		}
	}

	return nullptr;
}

bool UMouseLineManager::GetMouseExist()
{
	//当全部数组为0 则表示当前线路没有任何老鼠
	if (
		this->CurMouseGround.Num() == 0
		&&
		this->CurMouseUnderGround.Num() == 0
		&&
		this->CurMouseSky.Num() == 0
		)
	{
		return false;
	}

	return true;
}

void UMouseLineManager::KillAllMouse()
{
	TArray<AMouseActor*> CurMouses;
	CurMouses.Append(this->CurMouseGround);
	CurMouses.Append(this->CurMouseUnderGround);
	CurMouses.Append(this->CurMouseSky);

	for (AMouseActor* const CurMouse : CurMouses)
	{
		if (IsValid(CurMouse))
		{
			CurMouse->Destroy();
		}
	}

	this->CurMouseGround.Empty();
	this->CurMouseUnderGround.Empty();
	this->CurMouseSky.Empty();
}

AMouseActor* UMouseLineManager::SortMouseTopLocation(TArray<AMouseActor*>& _Mouses)
{
	//当前最前面的老鼠
	AMouseActor* CurTop = nullptr;

	for (AMouseActor* CurMouse : _Mouses)
	{
		//如果当前老鼠无效，则直接为最前老鼠
		if (!IsValid(CurTop))
		{
			CurTop = CurMouse;

			continue;
		}


		//排序
		if (IsValid(CurMouse)
			&&
			CurMouse->GetCurrentHP() > 0.f //当前老鼠生命值不为0
			&&
			CurMouse->GetActorLocation().Y < CurTop->GetActorLocation().Y//当前老鼠坐标小于当前最小坐标
			)
		{
			CurTop = CurMouse;
		}
	}

	return CurTop;
}
//-----------------------------------------------------------------------------------------------------------------------------


void UMouseTimeManager::Reset()
{
	this->bEnable = false;
	this->Minute = 0;
	this->Second = 0;
	this->CurTime = 0.f;
	this->AllTime = 0.f;
	this->TimeNodeIndex = 0;
	this->TimeNodeRoundIndex = 0;
	this->RoundIndex = 0;
}



void UMouseTimeManager::UpdateTime(const float& _tick, UMouseManagerComponent* const MouseManagerComponent)
{
	this->AllTime += _tick;

	//当所有老鼠全部生成完毕并且进入最后一个回合的末尾
	if (this->bCurAllMouseSpawnEnd)
	{
		return;
	}

	//检测当前回合是否还有老鼠存在【开启条件都是在当前回合最后子节点末尾】
	if (this->bEnableCheckMouseExist)
	{
		//当还有老鼠存在，则什么都不做
		if (MouseManagerComponent->IsMouseExist())
		{
			return;
		}

		//当没有任何老鼠存在时，进入下个回合
		this->bEnableCheckMouseExist = false;

		//获取当前回合的配置
		FRondWidthMouseConfig RoundConfig;
		if (MouseManagerComponent->M_UMouseStructManager->GetMouseRound(this->GetTimeRound(), RoundConfig))
		{
			//检测是否配置钥匙或者徽章
			if (RoundConfig.RoundKey.Enable)
			{
				//创建徽章或者钥匙交换UI

				//禁用触摸
				APlayerView::GetPlayerViewInstance()->DisEnableTouch();
				//暂停游戏
				AGameMapInstance::GetVSManagerComponent_Static()->SetGamePause();
				//创建UI
				UUI_MouseRoundUpTip* LUI_MouseRoundUpTip =
					CreateWidget<UUI_MouseRoundUpTip>(MouseManagerComponent->GetWorld(),
						LoadClass<UUI_MouseRoundUpTip>(0,
							TEXT("WidgetBlueprint'/Game/Resource/BP/GameStart/VS/UI_Player/Tips/UI_MouseRoundUpTips.UI_MouseRoundUpTips_C'")
							));
				LUI_MouseRoundUpTip->AddToViewport();

				return;
			}
			else {
				//进入下个回合
				this->InNextRound(RoundConfig.InNextRoundTime);
			}
		}

		return;
	}

	//开启进入下一个回合的计时
	if (this->bEnableNextRound)
	{
		//检测当前回合是否是最后一个回合
		if (this->bEnableCheckNextRoundIsEnd)
		{
			//判断当前回合是否是最后一个回合
			if (this->GetTimeRound() + 1 == MouseManagerComponent->M_UMouseStructManager->GetRoundTotal())
			{
				this->bCurRoundIsEnd = true;
			}
			else {
				this->bCurRoundIsEnd = false;
			}
		}

		//如果当前是最后一个回合，并且要求进入下一个回合，将不会在更新老鼠生成器，表示需要生成的老鼠已经全部生成
		if (this->bCurRoundIsEnd)
		{
			this->bCurAllMouseSpawnEnd = true;
			return;
		}

		//更新进入下一个会的时间
		if (this->CurNextRoundTime < this->CurNextRoundTotalTime)
		{
			this->CurNextRoundTime += _tick;
		}
		else {
			//关闭进入下一个回合的计时
			this->bEnableNextRound = false;
			//清除当前计时
			this->CurNextRoundTime = 0.f;
			//重置最小子节点
			this->TimeNodeRoundIndex = 0;
			//重置当前回合子节点
			this->TimeNodeIndex = 0;
			//更新回合索引
			this->RoundIndex++;
			//继续进行节点更新
			this->bUpdateCurNode = true;
			//当节点发生变换时，触发
			this->OnRoundNodeChanged.ExecuteIfBound();
			//当进入下一个回合时，触发
			this->OnInNextRound.ExecuteIfBound();

			return;
		}
	}
	else {

		//不会更新当前回合的节点
		if (!this->bUpdateCurNode)
			return;

		if (this->CurTime < 1.f)
		{
			this->CurTime += _tick;
		}
		else {
			this->CurTime = 0;

			//得到当前回合的子节点数量
			int32 CurRoundNode = MouseManagerComponent->M_UMouseStructManager->GetRoundNodeCountByRoundIndex(this->RoundIndex);
			if (CurRoundNode != 0)
			{
				//更新最小子节点【必须跑满19 即20秒】
				this->TimeNodeIndex++;
				if (this->TimeNodeIndex > 19)
				{
					this->TimeNodeIndex = 0;


					//更新最小大节点【每20秒必须进1】，必须跑满当前回合数的子节点最大数量
					this->TimeNodeRoundIndex++;
					if (this->TimeNodeRoundIndex == CurRoundNode)
					{
						//不再继续更新当前节点
						this->bUpdateCurNode = false;
						//回合末尾
						this->OnInCurRoundEnd.ExecuteIfBound();
					}
					else {
						//当节点发生变换时，触发[>20 Round++ != CurRoundNode]
						this->OnRoundNodeChanged.ExecuteIfBound();
					}
				}
				else {
					//当节点发生变换时，触发[<= 20]
					this->OnRoundNodeChanged.ExecuteIfBound();
				}
			}
			else {
				//不再继续更新当前节点
				this->bUpdateCurNode = false;
				//回合末尾
				this->OnInCurRoundEnd.ExecuteIfBound();
			}
		}
	}

}

void UMouseTimeManager::InNextRound(const float& _DelayTime)
{
	//开始最后一个的回合检测
	this->bEnableCheckNextRoundIsEnd = true;
	//开启进入下一个回合
	this->bEnableNextRound = true;
	//重置进入下一个会的当前时间
	this->CurNextRoundTime = 0.f;
	//不再更新子节点
	this->bUpdateCurNode = false;
	//设置进入下一个回合的延迟
	this->CurNextRoundTotalTime = _DelayTime;
}

void UMouseTimeManager::InNextRound()
{
	//检测老鼠是否存在
	this->bEnableCheckMouseExist = true;
}

bool UMouseTimeManager::GetCurRoundEnd()
{
	return this->bUpdateCurNode ? false : true;
}

bool UMouseTimeManager::GetAllMouseSpawnEnd()
{
	return this->bCurAllMouseSpawnEnd;
}

void UMouseTimeManager::Enable()
{
	if (this->bEnable)
		return;

	this->bEnable = true;

	//当启动时间时，表示准备时间已经结束
	this->OnRoundNodeChanged.ExecuteIfBound();
}

int32 UMouseTimeManager::GetTimeNode()
{
	return this->TimeNodeIndex;
}

int32 UMouseTimeManager::GetTimeNodeRound()
{
	return this->TimeNodeRoundIndex;
}

int32 UMouseTimeManager::GetTimeRound()
{
	return this->RoundIndex;
}

void UMouseTimeManager::SetTimeRound(const int32& Index)
{
	this->RoundIndex = Index;
}

void UMouseTimeManager::SetRoundEnd()
{
	this->bEnableCheckMouseExist = false;
	this->bCurAllMouseSpawnEnd = true;
}

int32 UMouseTimeManager::GetTime()
{
	return (int32)(this->AllTime);
}

//-----------------------------------------------------------------------------------------------------------------------------

AMouseActor* UMouseSpawnManager::SpawnMouse(const FString& _MousePath, const FLine& Line, const int32& RowMax, bool DeathFroceGameWin, const TArray<FString>& Items, const FMouseBase& MouseData)
{
	AMouseActor* CurNewMouse = AGameMapInstance::GetGameMapInstance()->
		GetWorld()->SpawnActor<AMouseActor>(
			LoadClass<AMouseActor>(0, *_MousePath)
			);

	if (IsValid(CurNewMouse))
	{
		//设置老鼠死亡时，是否直接结束游戏
		CurNewMouse->SetCurMouseDeathForceGameWin(DeathFroceGameWin);
		//设置老鼠专属掉落物
		CurNewMouse->SetMouseLevelItems(Items);
		//设置老鼠基础速度
		CurNewMouse->SetMoveSpeed(MouseData.M_MouseBaseSpeed);
		//设置老鼠基础攻击力
		CurNewMouse->SetMouseATK(MouseData.M_MouseBaseATK);
		//设置老鼠基础经验值
		CurNewMouse->SetEx(MouseData.M_MouseEx);
		//设置线路
		CurNewMouse->SetMouseLine(Line);
		//设置线路类型
		CurNewMouse->SetMouseLineType(MouseData.LineType);
		//设置坐标
		CurNewMouse->SetActorLocation(
			AGameMapInstance::GetGameMapInstance()->M_MesheControllComponent->M_LineLastLocation[Line.Row]
			+ FVector(0.f, 90.f, -15.f)
		);
		//设置旋转
		CurNewMouse->InitRotation();

		//获取排序
		int32 CurSort = AGameMapInstance::GetGameMapInstance()->
			M_MesheControllComponent->
			GetMapMeshCurrentRight(Line.Row)->GetTranslucency();
		//设置排序
		CurNewMouse->SetTranslucentSortPriority(CurSort + 19);

		//设置血量
		int32 CurLevel = AGameMapInstance::GetGameMapInstance()->GetGameMapStructManager()->LevelConfig.MouseLevel <= 0
			?
			1 : AGameMapInstance::GetGameMapInstance()->GetGameMapStructManager()->LevelConfig.MouseLevel;
		//当前总生命 + 每一个等级提升 * 当前总生命的50%
		CurNewMouse->SetHP(MouseData.M_MouseBaseHP + (CurLevel - 1) * (MouseData.M_MouseBaseHP / 2.f));
	}

	return CurNewMouse;
}


//-----------------------------------------------------------------------------------------------------------------------------

//静态变量初始化
UMouseManagerComponent* UMouseManagerComponent::M_S_CurrentClass = nullptr;

// Sets default values for this component's properties
UMouseManagerComponent::UMouseManagerComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	UMouseManagerComponent::M_S_CurrentClass = this;
}


// Called when the game starts
void UMouseManagerComponent::BeginPlay()
{
	Super::BeginPlay();
}
void UMouseManagerComponent::InitMouseStructManager()
{
	UFVMGameInstance* L_UFVMGameInstance = UFVMGameInstance::GetFVMGameInstance();

	if (IsValid(L_UFVMGameInstance))
	{
		//获取地图存档
		UGameMapStructManager* _L_UGameMapStructManager = L_UFVMGameInstance->GetGameMapStructManager();

		if (!_L_UGameMapStructManager && !IsValid(_L_UGameMapStructManager))
		{
			if (UFVMGameInstance::GetDebug())
			{
				UE_LOG(LogTemp, Error, TEXT("[UMouseManagerComponent::InitMouseStructManager]:地图存档获取失败!初始化失败!"));
			}

			return;
		}

		if (IsValid(L_UFVMGameInstance->GetMouseStructManager()))
		{
			//初始化本老鼠管理器
			this->M_UMouseStructManager = L_UFVMGameInstance->GetMouseStructManager();
		}
		else {
			if (UFVMGameInstance::GetDebug())
			{
				UE_LOG(LogTemp, Error, TEXT("[UMouseManagerComponent::InitMouseStructManager]:老鼠配置加载失败!"));
			}
		}
	}
}


void UMouseManagerComponent::InitMouseLineManager()
{
	//从老鼠配置管理器中拿到当前地图的总行，生成对应的线路
	for (int32 i = 0; i < this->M_UMouseStructManager->GetRowCount(); i++)
	{
		this->MouseLineManager.Emplace(NewObject<UMouseLineManager>());

		if (UFVMGameInstance::GetDebug())
		{
			UE_LOG(LogTemp, Error, TEXT("[UMouseManagerComponent::InitMouseLineManager]:生成线路管理器![%d]"), i);
		}
	}
}

void UMouseManagerComponent::InitTimer()
{
	this->MouseTimeManager = NewObject<UMouseTimeManager>();
	this->CurGameReadyTime = 0.f;

	//绑定事件【更新一个子节点时】
	this->MouseTimeManager->OnRoundNodeChanged.BindUObject(this, &UMouseManagerComponent::OnRoundNodeChangedCallback);
	//进入当前回合末尾时
	this->MouseTimeManager->OnInCurRoundEnd.BindUObject(this, &UMouseManagerComponent::OnCurRoundEndCallback);
	//进入下一个回合时
	this->MouseTimeManager->OnInNextRound.BindUObject(this, &UMouseManagerComponent::InitNextRoundCallBack);
}

void UMouseManagerComponent::InitMouseResource()
{
	//初始化老鼠数据
	UGameSystemFunction::GetDataTableRows(TEXT("DataTable'/Game/Resource/BP/Data/MouseData/MouseData.MouseData'"), this->AllMouseData);
}

void UMouseManagerComponent::InitMouseSpawnManager()
{
	this->MouseSpawnManager = NewObject<UMouseSpawnManager>();
}

const UMouseTimeManager* const UMouseManagerComponent::GetMouseTimeManager()
{
	return this->MouseTimeManager;
}

bool UMouseManagerComponent::GetMousePathByName(const FString& MouseName, FString& OutMousePath, FMouseBase& OutData)
{
	for (const auto& Cur : this->AllMouseData)
	{
		if (Cur.M_Mouse.M_MouseName.Equals(MouseName))
		{
			OutMousePath = Cur.M_Mouse.ItemTarget_ActorFilePath.ToString();

			OutData = Cur.M_Mouse;

			return true;
		}
	}

	return false;
}

void UMouseManagerComponent::OnRoundNodeChangedCallback()
{
	//得到当前回合
	FRondWidthMouseConfig RoundConfig;
	if (this->M_UMouseStructManager->GetMouseRound(this->MouseTimeManager->GetTimeRound(), RoundConfig))
	{
		//得到当前回合子节点
		FTimeNodeWidthRound RoundTimeNode;
		if (this->M_UMouseStructManager->GetMouseTimeNodeByRound(RoundConfig, this->MouseTimeManager->GetTimeNodeRound(), RoundTimeNode))
		{
			//从子节点中找到分段最小子节点，并且生成老鼠
			FTimeNode TimeNode;
			if (this->M_UMouseStructManager->GetMouseTimeNodeByTimeNode(RoundTimeNode, this->MouseTimeManager->GetTimeNode(), TimeNode))
			{
				//地图行
				TArray<int32> CurRows;
				for (int32 i = 0; i < this->MouseLineManager.Num(); i++)
				{
					CurRows.Emplace(i);
				}

				//生成老鼠
				for (const auto& Cur : TimeNode.CurMouseNode)
				{
					//老鼠的资产路径
					FString CurMousePath;
					FMouseBase MouseData;
					//拿到路径并且生成老鼠
					if (this->GetMousePathByName(Cur.CurMouseName, CurMousePath, MouseData))
					{

						//当前最新的老鼠
						AMouseActor* CurMouse = nullptr;

						//临时的全行
						TArray<int32> LocalRows = CurRows;
						//排除行
						for (const int32& CurRow : Cur.IgnoreRows)
						{
							int32 CurLocalRowIndex = LocalRows.Find(CurRow);
							if (CurLocalRowIndex != -1)
							{
								LocalRows.RemoveAt(CurLocalRowIndex);
							}
						}

						//当前线路
						FLine CurMouseLine = Cur.CurMouseLine;

						//判断设置的当前线路是否被忽略,被忽略则随机线路
						if (LocalRows.Find(CurMouseLine.Row) == -1 || CurMouseLine.Row == -1)
						{
							int32 CurRowIndex = UKismetMathLibrary::RandomIntegerInRange(0, LocalRows.Num() - 1);
							CurMouseLine.Row = LocalRows[CurRowIndex];
						}

						//生成老鼠
						CurMouse = this->MouseSpawnManager->SpawnMouse(
							CurMousePath,
							CurMouseLine,
							this->MouseLineManager.Num() - 1,
							Cur.CurMouseDeathForceGameWin,
							Cur.LevelItems, MouseData
						);

						//添加到线路管理器中,实时追踪老鼠
						if (IsValid(CurMouse) && this->MouseLineManager.Num() - 1 >= CurMouse->GetMouseLine().Row)
						{
							this->MouseLineManager[CurMouse->GetMouseLine().Row]->AddMouse(CurMouse);
						}
					}
				}
			}
		}
	}

	//更新进度值
	this->UpdateRoundProgress();
}

void UMouseManagerComponent::OnCurRoundEndCallback()
{
	//获取当前回合的配置
	FRondWidthMouseConfig CurRoundConfig;
	if (this->M_UMouseStructManager->GetMouseRound(this->MouseTimeManager->GetTimeRound(), CurRoundConfig))
	{
		//如果开启了，当老鼠全部生成完毕自动进入下一个回合，直接进入下一个回合
		//并且没有配置钥匙
		if (CurRoundConfig.AllMouseSpawnFinishAutoNext && !CurRoundConfig.RoundKey.Enable)
		{
			this->MouseTimeManager->InNextRound(CurRoundConfig.InNextRoundTime);
		}
		else {
			//开启进入下一个回合的检测
			this->MouseTimeManager->InNextRound();
		}
	}
}

void UMouseManagerComponent::InitNextRoundCallBack()
{

	UFVMGameInstance::PlayBGM_S_Static(TEXT("MouseComing"), TEXT("ItemAudio"));

	//创建UI显示
	UUserWidget* MouseRoundTip = CreateWidget<UUserWidget>(this->GetWorld(), LoadClass<UUserWidget>(0, TEXT("WidgetBlueprint'/Game/Resource/BP/GameStart/VS/UI_Player/Tips/UI_MouseRoundTips.UI_MouseRoundTips_C'")));
	MouseRoundTip->AddToViewport();

	//更新进度值
	this->UpdateRoundProgress();
}

// Called every frame
void UMouseManagerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!IsValid(this->M_UMouseStructManager))
	{
		return;
	}


	//如果游戏暂停则不会继续
	if (AGameMapInstance::GetGameMapInstance()->GetGamePause())
	{
		return;
	}


	//准备时间
	this->CurGameReadyTime += DeltaTime;
	if (this->CurGameReadyTime >= AGameMapInstance::GetGameMapInstance()->GetGameMapStructManager()->LevelConfig.GameReadyTime)
	{
		//开启计时
		if (IsValid(this->MouseTimeManager))
		{
			this->MouseTimeManager->Enable();
			this->MouseTimeManager->UpdateTime(DeltaTime, this);
		}

		//更新路线管理器
		for (const auto& CurLine : this->MouseLineManager)
		{
			if (IsValid(CurLine))
			{
				CurLine->SortMouseByTick(DeltaTime);
			}
		}

		this->CurGameReadyTime = AGameMapInstance::GetGameMapInstance()->GetGameMapStructManager()->LevelConfig.GameReadyTime;
	}
}

void UMouseManagerComponent::ShowMouseUI()
{
	this->M_UUI_MouseRound = CreateWidget<UUI_MouseRound>(this->GetWorld(), LoadClass<UUI_MouseRound>(0, TEXT("WidgetBlueprint'/Game/Resource/BP/GameStart/VS/UI_Player/UIMouseRound.UIMouseRound_C'")));
	//设置属性
	this->M_UUI_MouseRound->InitFlag(this->M_UMouseStructManager->GetRoundTotal());

	this->M_UUI_MouseRound->AddToViewport();
}

void UMouseManagerComponent::RemoveMouseUI()
{
	if (this->M_UUI_MouseRound && this->M_UUI_MouseRound->IsInViewport())
	{
		this->M_UUI_MouseRound->RemoveFromParent();
	}
}

void UMouseManagerComponent::UpdateRoundProgress()
{
	if (this->MouseTimeManager->GetTimeRound() <= this->M_UMouseStructManager->GetRoundTotal())
	{
		//得到当前回合的回合总个数【节点个数】
		int32 CurRoundNodeCount = this->M_UMouseStructManager->GetRoundNodeCountByRoundIndex(this->MouseTimeManager->GetTimeRound());

		//计算当前回合与上一个回合的0-1进度，直接当作当前回合与总回合的进度
		float CurRoundClip = (CurRoundNodeCount != 0) ? ((this->MouseTimeManager->GetTimeNodeRound() + 1.f) / CurRoundNodeCount * 1.f) : 1.f;

		//得到当前回合与下一个回合的进度
		float CurRound = this->MouseTimeManager->GetTimeRound() + CurRoundClip;

		//更新回合进度
		this->M_UUI_MouseRound->UpdateMouseRoundSlider(CurRound, this->M_UMouseStructManager->GetRoundTotal());
		this->M_UUI_MouseRound->UpdateMouseRoundText(this->MouseTimeManager->GetTimeRound() + 1);
	}
}

void UMouseManagerComponent::ForceNextRound()
{
	//开启进入下一个回合的检测
	this->MouseTimeManager->InNextRound(0.f);
}

void UMouseManagerComponent::ForceNextEndRound()
{
	//在场所有路线的老鼠全部标记死亡
	for (UMouseLineManager* const CurLineMan : this->MouseLineManager)
	{
		CurLineMan->KillAllMouse();
	}

	//检索最后一个回合，知道遇见钥匙或者末尾
	for (int32 CurRoundIndex = this->MouseTimeManager->GetTimeRound(); CurRoundIndex < this->M_UMouseStructManager->GetMouseConfig().CurRoundConfig.Num(); CurRoundIndex++)
	{
		FRondWidthMouseConfig OutCurRound;
		if (this->M_UMouseStructManager->GetMouseRound(CurRoundIndex, OutCurRound))
		{
			if (OutCurRound.RoundKey.Enable)
			{
				//设置回合索引
				this->MouseTimeManager->SetTimeRound(CurRoundIndex);

				//强行进入下一个回合
				this->ForceNextRound();

				this->UpdateRoundProgress();

				return;
			}
		}
	}

	//直接将当前回合设置为最后一个回合末尾
	this->MouseTimeManager->SetRoundEnd();
}

bool UMouseManagerComponent::IsEndRound()
{
	if (
		//配置中的总波数
		this->M_UMouseStructManager->GetRoundTotal() - 1
		==
		//当前时间继续到的波数
		this->MouseTimeManager->GetTimeRound()
		)
	{
		return true;
	}

	return false;
}

bool UMouseManagerComponent::IsMouseSpawnEnd()
{
	if (IsValid(this->MouseTimeManager))
	{
		return this->MouseTimeManager->GetAllMouseSpawnEnd();
	}

	return false;
}

void UMouseManagerComponent::GameStart()
{
	//初始化老鼠配置管理器
	this->InitMouseStructManager();
	//初始化老鼠线路管理器
	this->InitMouseLineManager();
	//初始化时间配置
	this->InitTimer();
	//初始化老鼠资源配置
	this->InitMouseResource();
	//初始化老鼠生成器
	this->InitMouseSpawnManager();
	//初始化UI
	this->ShowMouseUI();
}


void UMouseManagerComponent::GameOver()
{
	if (UFVMGameInstance::GetDebug())
		UE_LOG(LogTemp, Warning, TEXT("[UMouseManagerComponent::GameOver]:游戏结束"));

	this->AllMouseData.Empty();
	this->MouseLineManager.Empty();
	this->MouseTimeManager = nullptr;
	this->MouseSpawnManager = nullptr;
	this->M_UMouseStructManager = nullptr;
	this->M_UUI_MouseRound = nullptr;
}

bool UMouseManagerComponent::IsMouseExist()
{
	for (UMouseLineManager* const CurLine : this->MouseLineManager)
	{
		if (CurLine->GetMouseExist())
		{
			return true;
		}
	}
	return false;
}

const int32 UMouseManagerComponent::GetLineCount() const
{
	return this->MouseLineManager.Num();
}

int32 UMouseManagerComponent::GetCurTime()
{
	if (IsValid(this->MouseTimeManager))
	{
		return this->MouseTimeManager->GetTime();
	}

	return 9999;
}

int32 UMouseManagerComponent::GetCurRound()
{
	if (IsValid(this->MouseTimeManager))
	{
		return this->MouseTimeManager->GetTimeRound();
	}

	return -1;
}

AMouseActor* const UMouseManagerComponent::GetLineTopFirstMouse(const int32 _LineIndex)
{
	if (this->MouseLineManager.Num() > 0)
	{
		return this->MouseLineManager[_LineIndex]->GetMouseTopByAllType();
	}

	return nullptr;
}

AMouseActor* const UMouseManagerComponent::GetTopFirstMouse()
{
	AMouseActor* CurTop = nullptr;
	for (UMouseLineManager* const CurLine : this->MouseLineManager)
	{
		if (!IsValid(CurTop))
		{
			CurTop = CurLine->GetMouseTopByAllType();

			continue;
		}

		AMouseActor* CurNextTop = CurLine->GetMouseTopByAllType();
		if (IsValid(CurNextTop) && CurNextTop->GetCurrentHP() > 0.f)
		{
			if (CurNextTop->GetActorLocation().Y < CurTop->GetActorLocation().Y)
			{
				CurTop = CurNextTop;
			}
		}
	}
	return CurTop;
}

void UMouseManagerComponent::RemoveMouse(AMouseActor* _CurMouse)
{
	if (IsValid(_CurMouse))
	{
		if (
			_CurMouse->GetMouseLine().Row < AGameMapInstance::GetGameMapInstance()->
			M_MouseManagerComponent->MouseLineManager.Num()
			)
		{
			AGameMapInstance::GetGameMapInstance()->M_MouseManagerComponent->
				MouseLineManager[_CurMouse->GetMouseLine().Row]->RemoveMouse(_CurMouse);
		}
	}
}

void UMouseManagerComponent::AddMouse(AMouseActor* _CurMouse, const int32& Row)
{
	if (IsValid(_CurMouse))
	{
		if (
			Row < AGameMapInstance::GetGameMapInstance()->
			M_MouseManagerComponent->MouseLineManager.Num()
			)
		{
			AGameMapInstance::GetGameMapInstance()->M_MouseManagerComponent->
				MouseLineManager[Row]->AddMouse(_CurMouse);
		}
	}
}
