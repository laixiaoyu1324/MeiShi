// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameMapUI_MapViewCardItem.generated.h"


class UButton;

/**
 * 地图编辑器卡片选项
 */
UCLASS()
class FVM_API UGameMapUI_MapViewCardItem : public UUserWidget
{
	GENERATED_BODY()

public:
	bool Initialize() override;

public:
	UPROPERTY()
		UButton* CurCardBut = nullptr;
public:
	void Init(const FString& CardName, const FSoftClassPath& ClassPath, const FSoftObjectPath& Texture2dHead);
	const FString& GetCardName() const;
	const FSoftClassPath& GetCardClassPath() const;
	const FSoftObjectPath& GetCardHeadPath() const;
private:
	//卡片名称
	UPROPERTY()
		FString MCardName;
	//实体类
	UPROPERTY()
		FSoftClassPath MClassPath;
	//头像纹理
	UPROPERTY()
		FSoftObjectPath MTexture2dHead;
};
