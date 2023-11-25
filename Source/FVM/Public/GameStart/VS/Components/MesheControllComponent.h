// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameStart/VS/MapBaseType.h"
#include "Components/ActorComponent.h"
#include "MesheControllComponent.generated.h"

class UGameMapStructManager;
class AMapLauncherBase;
class AMapMouseMesheManager;
class AMapMeshe;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class FVM_API UMesheControllComponent :public UActorComponent
{
	GENERATED_BODY()

public:

	UMesheControllComponent();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	//设置游戏地图配置
	UFUNCTION(BlueprintCallable)
		UGameMapStructManager* SetGameMapStructManager(UGameMapStructManager* _GameMapStructManager);
	//获取游戏地图配置
	UFUNCTION(BlueprintCallable)
		UGameMapStructManager* const GetGameMapStructManager();
	//删除所有的网格
	UFUNCTION(BlueprintCallable)
		void ClearAllMeshes();
	//生成网格
	UFUNCTION(BlueprintCallable)
		void SpawnMeshes();

	//获取网格
	UFUNCTION(BlueprintCallable, Category = "AMapMesh")
		AMapMeshe* GetMapMesh(int32 _Row, int32 _Col);
	//获取当前网格的最顶部网格
	UFUNCTION(BlueprintCallable, Category = "AMapMesh")
		AMapMeshe* GetMapMeshCurrentTop(int32 _Col);
	//获取当前网格的最底部网格
	UFUNCTION(BlueprintCallable, Category = "AMapMesh")
		AMapMeshe* GetMapMeshCurrentBottom(int32 _Col);
	//获取当前网格的最左边的网格
	UFUNCTION(BlueprintCallable, Category = "AMapMesh")
		AMapMeshe* GetMapMeshCurrentLeft(int32 Row);
	//获取当前网格的最右边的网格
	UFUNCTION(BlueprintCallable, Category = "AMapMesh")
		AMapMeshe* GetMapMeshCurrentRight(int32 Row);


	//获取老鼠网格管理器
	UFUNCTION(BlueprintCallable, Category = "MouseMeshManager")
		AMapMouseMesheManager* GetMapMouseMesh(int32 _Row, int32 _Col);
	//获取当前网格的最顶部网格
	UFUNCTION(BlueprintCallable, Category = "MouseMeshManager")
		AMapMouseMesheManager* GetMapMouseMeshCurrentTop(int32 _Col);
	//获取当前网格的最底部网格
	UFUNCTION(BlueprintCallable, Category = "MouseMeshManager")
		AMapMouseMesheManager* GetMapMouseMeshCurrentBottom(int32 _Col);
	//获取当前网格的最左边的网格
	UFUNCTION(BlueprintCallable, Category = "MouseMeshManager")
		AMapMouseMesheManager* GetMapMouseMeshCurrentLeft(int32 Row);
	//获取当前网格的最右边的网格
	UFUNCTION(BlueprintCallable, Category = "MouseMeshManager")
		AMapMouseMesheManager* GetMapMouseMeshCurrentRight(int32 Row);

	//获取地图发射器
	UFUNCTION(BlueprintCallable)
		AMapLauncherBase* GetMapLauncher();
public:
	//初始化网格地图
	UFUNCTION()
		void InitMeshesMap(const int32& _Row, const int32& _Col);
	//初始化网格
	UFUNCTION()
		void InitMesh(AActor* _Mesh, const FVector& _MeshLocation);
	//获取网格
	UFUNCTION()
		AMapMeshe* GetMesh(const int32& _Row, const int32& _Col);
	//获取网格的坐标
	UFUNCTION()
		FVector GetMapMeshLocation(const int32& _Row, const int32& _Col);
	//获取地图网格的总行和总列
	UFUNCTION()
		FLine GetMapMeshRowAndCol();
public:
	//每一行的最后一个的位置
	UPROPERTY(EditAnywhere)
		TArray<FVector> M_LineLastLocation;
protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	//初始化地图
	void InitGameMapMeshe();
private:
	//地图配置管理器
	UPROPERTY(EditAnywhere)
		UGameMapStructManager* M_GameMapStructManager = nullptr;
	//地图的网格
	UPROPERTY(EditAnywhere)
		UClass* M_AMapMeshe = nullptr;
	//地图的发射器
	UPROPERTY(EditAnywhere)
		AMapLauncherBase* M_MapLauncher = nullptr;
private:
	//当前地图的总行和总列
	UPROPERTY()
		FLine M_CurMapLine;
	//当前所有网格合集
	UPROPERTY()
		TArray<AActor*> M_CurMeshes;
	//当前所有网格的位置
	UPROPERTY()
		TArray<FVector> M_CurMeshesLocation;
	//当前所有的老鼠网格管理器
	UPROPERTY()
		TArray<AMapMouseMesheManager*> M_CurMapMouseMesheManager;
};
