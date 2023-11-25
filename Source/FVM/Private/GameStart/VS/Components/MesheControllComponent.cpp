#include "GameStart\VS\Components\MesheControllComponent.h"

#include "GameStart/VS/Animal/VSAnimalBase.h"

#include "GameSystem/GameMapStructManager.h"
#include "GameSystem/FVMGameInstance.h"

#include "GameStart/Flipbook/SpriteActor.h"
#include "GameStart/VS/MapMeshe.h"
#include "GameStart/MapLauncher/MapLauncherBase.h"


UMesheControllComponent::UMesheControllComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

UGameMapStructManager* UMesheControllComponent::SetGameMapStructManager(UGameMapStructManager* _GameMapStructManager)
{
	if (!_GameMapStructManager)
		return nullptr;

	this->M_GameMapStructManager = _GameMapStructManager;

	this->InitGameMapMeshe();

	return _GameMapStructManager;
}

UGameMapStructManager* const UMesheControllComponent::GetGameMapStructManager()
{
	return this->M_GameMapStructManager;
}

void UMesheControllComponent::ClearAllMeshes()
{
	this->M_CurMeshes.Empty();
	this->M_CurMeshesLocation.Empty();
	this->M_CurMapMouseMesheManager.Empty();
}


void UMesheControllComponent::SpawnMeshes()
{

	this->ClearAllMeshes();

	//初始化网格地图
	this->InitMeshesMap(this->M_GameMapStructManager->GameMapStruct.M_Meshe.Num(), this->M_GameMapStructManager->GameMapStruct.M_Meshe[0].M_Col.Num());

	//生成第一个网格位置
	const FVector2D& FristMeshPosition = this->M_GameMapStructManager->GameMapStruct.M_FirstRowMesheLocation;

	//网格位置
	FTransform CurrentMesheLocation;

	float RowPadding = 0.f;
	for (int32 Row = 0; Row < this->M_GameMapStructManager->GameMapStruct.M_Meshe.Num(); Row++)
	{
		//得到网格层渲染优先级【第Row层的优先级】
		int32 CurMesheTranslucency = GetRangeLayer((Row * this->M_GameMapStructManager->GameMapStruct.M_Meshe[Row].M_Col.Num() * 20), TranslucencyConst::ETranslucency::EMeshe);

		for (int32 Col = 0; Col < this->M_GameMapStructManager->GameMapStruct.M_Meshe[Row].M_Col.Num(); Col++)
		{
			//设置坐标
			CurrentMesheLocation.SetLocation(
				FVector(550.f,
					FristMeshPosition.X + Col *
					this->M_GameMapStructManager->GameMapStruct.M_Meshe[Row].M_Col[Col].M_PaddingCol,
					FristMeshPosition.Y - RowPadding));

			//老鼠网格线路
			{
				//得到老鼠网格管理器
				AMapMouseMesheManager* CurMapMouseMesheManager = this->GetWorld()->SpawnActor<AMapMouseMesheManager>(
					LoadClass<AMapMouseMesheManager>(0,
						TEXT("Blueprint'/Game/Resource/BP/GameStart/VS/AMapMouseManager.AMapMouseManager_C'")),
					CurrentMesheLocation);
				//设置线路
				CurMapMouseMesheManager->SetLine(FLine(Row, Col));
				//缓存
				this->M_CurMapMouseMesheManager.Emplace(CurMapMouseMesheManager);
			}

			//地图网格线路
			{
				//得到地图网格
				AMapMeshe* M_TempMapMesh = this->GetWorld()->SpawnActor<AMapMeshe>(
					LoadClass<AMapMeshe>(0, TEXT("Blueprint'/Game/Resource/BP/GameStart/VS/AMapMeshe.AMapMeshe_C'")),
					CurrentMesheLocation);

				//设置UI的旋转
				M_TempMapMesh->SetUUI_MapMeshRotatonPitchY(180.f);
				//设置行列
				M_TempMapMesh->SetLine(FLine(Row, Col));
				//设置网格类型
				M_TempMapMesh->SetLineType(this->M_GameMapStructManager->GameMapStruct.M_Meshe[Row].M_Col[Col].M_ELineType);
				//添加Actor到列表
				this->InitMesh(M_TempMapMesh, CurrentMesheLocation.GetLocation());

				//初始化网格UI
				M_TempMapMesh->InitUUI_MapMesh(this->M_GameMapStructManager->GameMapStruct.M_Meshe[Row].M_Col[Col].M_ELineType, CurMesheTranslucency);
				//设置位置变化
				M_TempMapMesh->SetWidgetComponentTransform(CurrentMesheLocation);

				//创建背景
				if (this->M_GameMapStructManager->GameMapStruct.M_Meshe[Row].M_Col[Col].M_MeshBG.M_bEnableBG)
				{
					//创建背景
					ASpriteActor* Bg = this->GetWorld()->SpawnActor<ASpriteActor>(LoadClass<ASpriteActor>(0, TEXT("Class'/Script/FVM.SpriteActor'")));
					if (Bg)
					{
						//设置精灵
						Bg->SetSprite(LoadObject<UPaperSprite>(0, *this->M_GameMapStructManager->GameMapStruct.M_Meshe[Row].M_Col[Col].M_MeshBG.M_BG_FullPath.ToString()));
						//设置偏移
						Bg->SetSpriteOffset(this->M_GameMapStructManager->GameMapStruct.M_Meshe[Row].M_Col[Col].M_MeshBG.M_Offset);
						//设置坐标
						Bg->SetSpriteLocation(CurrentMesheLocation.GetLocation(), true);
						//设置渲染优先级
						Bg->SetTranslucency(GetRangeLayer(0, TranslucencyConst::ETranslucency::EBgBlock));
						//将背景与网格关联
						M_TempMapMesh->SetMesheBg(Bg);
					}
				}

				//最后一格的网格位置
				if (Col + 1 == this->M_GameMapStructManager->GameMapStruct.M_Meshe[Row].M_Col.Num())
				{
					//最后一格网格的位置
					this->M_LineLastLocation.Emplace(CurrentMesheLocation.GetLocation());
				}

			}

			//每列渲染优先级递增20
			CurMesheTranslucency += 20;
		}
		RowPadding += this->M_GameMapStructManager->GameMapStruct.M_Meshe[Row].M_PaddingRow;
	}

	//创建发射器
	AMapLauncherBase* TMapLauncher = Cast<AMapLauncherBase>(
		UGameplayStatics::BeginDeferredActorSpawnFromClass(this->GetWorld(),
			TSubclassOf<AMapLauncherBase>(
				LoadClass<AMapLauncherBase>(
					0,
					*this->M_GameMapStructManager->LevelConfig.LevelLauncherPath.ToString())
				), FTransform()));

	if (TMapLauncher)
	{
		//设置关卡配置
		TMapLauncher->SetLevelConfig(this->M_GameMapStructManager->LevelConfig);
		//设置地图配置
		TMapLauncher->SetMapConfig(this->M_GameMapStructManager->GameMapStruct);
		//设置地图网格
		TMapLauncher->SetMapMesheControllComponent(this);
	}

	//完成生成
	AMapLauncherBase* MapLauncher = Cast<AMapLauncherBase>(UGameplayStatics::FinishSpawningActor(TMapLauncher, FTransform()));

	//设置地图发射器
	this->M_MapLauncher = MapLauncher;

	for (int32 Row = 0; Row < this->M_GameMapStructManager->GameMapStruct.M_Meshe.Num(); Row++)
	{
		for (int32 Col = 0; Col < this->M_GameMapStructManager->GameMapStruct.M_Meshe[Row].M_Col.Num(); Col++)
		{
			//当前网格是否支持移动
			if (this->M_GameMapStructManager->GameMapStruct.M_Meshe[Row].M_Col[Col].M_bMoveActive)
			{
				const FMesheCol& _Meshe = this->M_GameMapStructManager->GameMapStruct.M_Meshe[Row].M_Col[Col];
				this->GetMapMesh(Row, Col)->SetMesheMove(_Meshe.M_MoveingCloudDownTime, _Meshe.M_MoveingTime, _Meshe.M_MoveOffset, this->GetMapMesh(_Meshe.M_MoveOffset.Row, _Meshe.M_MoveOffset.Col)->GetActorLocation());
			}
		}
	}

	//生成动物
	int32 AnimalPathIndex = 0;
	for (const TSoftClassPtr<class AVSAnimalBase>& Animal : this->M_GameMapStructManager->GameMapStruct.RowAnimalClass)
	{
		if (AnimalPathIndex < this->M_GameMapStructManager->GameMapStruct.M_Meshe.Num())
		{
			FTransform LocalTrans = this->GetMapMeshCurrentLeft(AnimalPathIndex)->GetActorTransform();
			LocalTrans.SetLocation(LocalTrans.GetLocation() + FVector(0.f, -60.f, 0.f));

			if (!Animal.Get())
			{
				Animal.LoadSynchronous();
			}

			AVSAnimalBase* CurAnim = this->GetWorld()->SpawnActor<AVSAnimalBase>(Animal.Get());

			if (!CurAnim)
			{
				AnimalPathIndex++;
				continue;
			}

			CurAnim->SetRow(AnimalPathIndex);
			CurAnim->SetActorTransform(LocalTrans);
			CurAnim->InitRotation();
			CurAnim->SetTranslucentSortPriority(this->GetMapMeshCurrentRight(AnimalPathIndex)->GetTranslucency());
		}

		AnimalPathIndex++;
	}
}


AMapMeshe* UMesheControllComponent::GetMapMesh(int32 _Row, int32 _Col)
{
	return this->GetMesh(_Row, _Col);
}

AMapMeshe* UMesheControllComponent::GetMapMeshCurrentTop(int32 _Col)
{
	return this->GetMesh(0, _Col);
}

AMapMeshe* UMesheControllComponent::GetMapMeshCurrentBottom(int32 _Col)
{
	return this->GetMesh(this->GetMapMeshRowAndCol().Row - 1, _Col);
}

AMapMeshe* UMesheControllComponent::GetMapMeshCurrentLeft(int32 Row)
{
	return this->GetMesh(Row, 0);
}

AMapMeshe* UMesheControllComponent::GetMapMeshCurrentRight(int32 Row)
{
	return this->GetMesh(Row, this->GetMapMeshRowAndCol().Col - 1);
}

AMapMouseMesheManager* UMesheControllComponent::GetMapMouseMesh(int32 _Row, int32 _Col)
{
	if (_Row < 0 || _Col < 0 || _Row >= this->GetMapMeshRowAndCol().Row || _Col >= this->GetMapMeshRowAndCol().Col)
	{
		return nullptr;
	}

	return this->M_CurMapMouseMesheManager[_Row * this->M_CurMapLine.Col + _Col];
}

AMapMouseMesheManager* UMesheControllComponent::GetMapMouseMeshCurrentTop(int32 _Col)
{
	return this->M_CurMapMouseMesheManager[_Col];
}

AMapMouseMesheManager* UMesheControllComponent::GetMapMouseMeshCurrentBottom(int32 _Col)
{
	return this->M_CurMapMouseMesheManager[this->GetMapMeshRowAndCol().Row - 1 * this->M_CurMapLine.Col + _Col];
}

AMapMouseMesheManager* UMesheControllComponent::GetMapMouseMeshCurrentLeft(int32 Row)
{
	return this->M_CurMapMouseMesheManager[Row * this->M_CurMapLine.Col];
}

AMapMouseMesheManager* UMesheControllComponent::GetMapMouseMeshCurrentRight(int32 Row)
{
	return this->M_CurMapMouseMesheManager[Row * this->M_CurMapLine.Col + this->GetMapMeshRowAndCol().Col - 1];
}

AMapLauncherBase* UMesheControllComponent::GetMapLauncher()
{
	return this->M_MapLauncher;
}

// Called when the game starts
void UMesheControllComponent::BeginPlay()
{
	Super::BeginPlay();

	//加载(地形)Actor
	this->M_AMapMeshe = LoadClass<AMapMeshe>(0, TEXT("Blueprint'/Game/Resource/BP/GameStart/VS/AMapMeshe.AMapMeshe_C'"));

	//如果加载失败则返回
	if (!this->M_AMapMeshe)
	{
		UE_LOG(LogTemp, Error, TEXT("[%s]M_AMapMeshe is a nullptr"), __FUNCTION__);
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, *FString(__FUNCTION__ + FString("M_AMapMeshe is a nullptr")));
	}
}

void UMesheControllComponent::InitGameMapMeshe()
{
	this->ClearAllMeshes();

	this->SpawnMeshes();
}


// Called every frame
void UMesheControllComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UMesheControllComponent::InitMeshesMap(const int32& _Row, const int32& _Col)
{
	this->M_CurMapLine = FLine(_Row, _Col);
}

void UMesheControllComponent::InitMesh(AActor* _Mesh, const FVector& _MeshLocation)
{
	this->M_CurMeshes.Emplace(_Mesh);
	this->M_CurMeshesLocation.Emplace(_MeshLocation);
}

AMapMeshe* UMesheControllComponent::GetMesh(const int32& _Row, const int32& _Col)
{
	return Cast<AMapMeshe>(this->M_CurMeshes[_Row * this->M_CurMapLine.Col + _Col]);
}

FVector UMesheControllComponent::GetMapMeshLocation(const int32& _Row, const int32& _Col)
{
	return this->M_CurMeshesLocation[_Row * this->M_CurMapLine.Col + _Col];
}

FLine UMesheControllComponent::GetMapMeshRowAndCol()
{
	return this->M_CurMapLine;
}
