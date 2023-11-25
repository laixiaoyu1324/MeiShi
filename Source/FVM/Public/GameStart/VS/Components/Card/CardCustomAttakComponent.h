// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameStart/VS/Components/Card/CardLauncherComponent.h"
#include "CardCustomAttakComponent.generated.h"

class AOld_ATKCardActor;
class UPaperFlipbook;
class AFlyItemActor;

//炭烧海星发射器

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class FVM_API UCardCustomAttakComponent : public UCardLauncherComponent
{
	GENERATED_BODY()
private:
	UPROPERTY()
		AOld_ATKCardActor* M_CardActor = nullptr;
	UPROPERTY()
		AFlyItemActor* M_AFlyItemActor = nullptr;
	UPROPERTY()
		UPaperFlipbook* M_Idle = nullptr;
	UPROPERTY()
		UPaperFlipbook* M_Attack = nullptr;
private:
	float _tick_count = 0.f;
public:
	// Sets default values for this component's properties
	UCardCustomAttakComponent();
protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	//生成投射物并且设置属性
	virtual void Spawn() override;
	//播放攻击动画
	virtual void PlayAttackAnimation() override;
	//播放默认动画
	virtual void PlayIdleAnimation() override;
public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
public:
	//资源加载
	UFUNCTION(BlueprintCallable)
		virtual void LoadResource() override;
};
