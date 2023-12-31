// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除


#include "Data/MapData/Editor/MapTab/GameMapUI_MapViewCardItem.h"
#include <Components/Button.h>

bool UGameMapUI_MapViewCardItem::Initialize()
{
	Super::Initialize();

	this->CurCardBut = Cast<UButton>(this->GetWidgetFromName(FName(TEXT("Card"))));

	return true;
}

void UGameMapUI_MapViewCardItem::Init(const FString& CardName, const FSoftClassPath& ClassPath, const FSoftObjectPath& Texture2dHead)
{
	this->MCardName = CardName;
	this->MClassPath = ClassPath;
	this->MTexture2dHead = Texture2dHead;


	FButtonStyle Style;
	FLinearColor Color = FLinearColor(FVector(1.f, 1.f, 1.f));
	Style.Normal.SetResourceObject(Texture2dHead.TryLoad());
	Style.Normal.TintColor = Color;
	Style.Hovered.SetResourceObject(Texture2dHead.TryLoad());
	Style.Hovered.TintColor = Color * 0.7f;
	Style.Pressed.SetResourceObject(Texture2dHead.TryLoad());
	Style.Pressed.TintColor = Color * 0.5;
	Style.Disabled.SetResourceObject(Texture2dHead.TryLoad());

	this->CurCardBut->SetStyle(Style);
}

const FString& UGameMapUI_MapViewCardItem::GetCardName() const
{
	return this->MCardName;
}

const FSoftClassPath& UGameMapUI_MapViewCardItem::GetCardClassPath() const
{
	return this->MClassPath;
}

const FSoftObjectPath& UGameMapUI_MapViewCardItem::GetCardHeadPath() const
{
	return this->MTexture2dHead;
}
