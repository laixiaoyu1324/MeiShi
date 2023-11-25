// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameStart/VS/Components/Card/CardLauncherComponent.h"
#include "GameStart/Flipbook/GameActor/MouseActor.h"
#include "GameStart/Flipbook/GameActor/CardActor/Old_ATKCardActor.h"
#include "CardProjectionComponent.generated.h"

class AOld_ATKCardActor;
class AFlyItemActor;
class UPaperFlipbook;

UENUM(BlueprintType)
enum class FCardState : uint8 {
	_EIdle UMETA(Display = "Idle"),
	_EAttack UMETA(Display = "Attack"),
};

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class FVM_API UCardProjectionComponent : public UCardLauncherComponent
{
	GENERATED_BODY()
private:
	//默认动画
	UPROPERTY(EditInstanceOnly, Category = "ResInstance")
		UPaperFlipbook* M_UPaperFlipbook_Idle = nullptr;
	//攻击动画
	UPROPERTY(EditInstanceOnly, Category = "ResInstance")
		UPaperFlipbook* M_UPaperFlipbook_Attack = nullptr;
	//卡片的投射物体
	UPROPERTY(EditInstanceOnly, Category = "ResInstance")
		UClass* M_FlyItemActor = nullptr;
	//卡片拥有者
	UPROPERTY(EditInstanceOnly, Category = "ResInstance")
		AOld_ATKCardActor* M_Owner = nullptr;
	//卡片的状态
	UPROPERTY(EditInstanceOnly, Category = "CardState")
		FCardState M_FCardState;
public:
	//默认状态的动画
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resource")
		FString M_Anim_Idle;
	//攻击动画
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resource")
		FString M_Anim_Attack;
	//卡片投射物->Actor
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resource")
		FString M_CardActor_Projection_Item;
	//卡片可以攻击的类型(可以攻击 陆地 水上 水下)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CardProperty")
		TArray<ELineType> M_CardAttackType;
public:
	// Sets default values for this component's properties
	UCardProjectionComponent();
protected:
	//生成投射物并且设置属性
	virtual void Spawn() override;
	//播放攻击动画
	virtual void PlayAttackAnimation() override;
	//播放默认动画
	virtual void PlayIdleAnimation() override;
private:
	//射线设置数据
	TArray<FLineTraceSetting> M_LineSettings;
protected:
	// Called when the game starts
	virtual void BeginPlay() override;
public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
public:
	//资源加载
	UFUNCTION(BlueprintCallable)
		virtual void LoadResource() override;
private:
	FHitResult M_Trace_Result;
	FCollisionQueryParams M_Trace_Params;
	FCollisionResponseParams M_Trace_ResParams;
	ECollisionChannel M_Trace_Target;
};
