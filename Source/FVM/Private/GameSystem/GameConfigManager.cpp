// Fill out your copyright notice in the Description page of Project Settings.


#include "GameSystem/GameConfigManager.h"

#include "GameSystem/FVMGameInstance.h"
#include "GameSystem/Tools/GameSystemFunction.h"


UGameConfigManager* UGameConfigManager::GetGameConfigManager()
{
	return  UFVMGameInstance::GetFVMGameInstance()->GetGameConfigManager();
}
