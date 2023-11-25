// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除


#include "Data/MapData/Editor/EditorTab/GameMapUI_EditorTab.h"
#include "Data/MapData/Editor/FVMEditUI_GameMapEdit.h"
#include <Components/TextBlock.h>
#include <Components/Button.h>
#include <Components/VerticalBox.h>
#include <Components/EditableTextBox.h>
#include <Components/Button.h>

//---------------------------------------------------------------------------------------------------------------
// 编辑器选项卡组件：行列表项
//---------------------------------------------------------------------------------------------------------------

bool UFVMEditUI_EditRowTableItem::Initialize()
{
	Super::Initialize();

	this->Text_Title = Cast<UTextBlock>(this->GetWidgetFromName(FName(TEXT("RowNameText"))));

	return true;
}

void UFVMEditUI_EditRowTableItem::InitData(class UGameMapUI_EditorTab* _Class)
{
	this->Class = _Class;
}

void UFVMEditUI_EditRowTableItem::SetRowName(const FString& _RowName)
{
	this->RowName = FName(_RowName);

	if (this->Text_Title)
		Text_Title->SetText(FText::FromString(_RowName));
}



void UFVMEditUI_EditRowTableItem::Config()
{
	FString CurName = this->Class->GetGameMapEditor()->GetCurEditRowName().ToString();

	if (CurName.Equals(this->RowName.ToString()))
	{
		return;
	}

	this->Class->GetGameMapEditor()->SetEditorTitle(FText::FromName(this->RowName));
	this->Class->GetGameMapEditor()->InitCurEditConfig();
	this->Class->GetGameMapEditor()->LoadConfig();
}

bool UGameMapUI_EditorTab::Initialize()
{
	Super::Initialize();


	return true;
}

void UGameMapUI_EditorTab::InitEditorTab(UFVMEditUI_GameMapEdit* _Class)
{
	if (!this->FVMEditUI_GameMapEdit)
		this->FVMEditUI_GameMapEdit = _Class;

	this->EditorTab_ListViewPanel = Cast<UVerticalBox>(this->GetWidgetFromName(FName(TEXT("VerticalBox"))));
	this->EditorTab_NewRowInputText = Cast<UEditableTextBox>(this->GetWidgetFromName(FName(TEXT("EditorConfigRowName"))));
	this->EditorTab_NewRowSaveButt = Cast<UButton>(this->GetWidgetFromName(FName(TEXT("EditorTabSaveConfig"))));
}

void UGameMapUI_EditorTab::InitEditorTabListItems()
{
	if (!this->EditorTab_ListViewPanel)
		return;

	int32 LocalIndex = 0;
	for (const FName& CurRowName : this->FVMEditUI_GameMapEdit->GetGameMapRowNames())
	{
		//判断列表数量
		if (LocalIndex >= this->EditorTab_ListViewPanel->GetAllChildren().Num())
		{
			//创建UI
			UFVMEditUI_EditRowTableItem* CurRow = CreateWidget<UFVMEditUI_EditRowTableItem>(
				this,
				LoadClass<UFVMEditUI_EditRowTableItem>(0,
					TEXT("WidgetBlueprint'/Game/Resource/BP/Data/MapData/EditorTab/BP_EditorTabListItem.BP_EditorTabListItem_C'")
					)
				);
			//初始化
			CurRow->InitData(this);
			//设置数据
			CurRow->SetRowName(CurRowName.ToString());
			//添加一行数据
			this->EditorTab_ListViewPanel->AddChildToVerticalBox(CurRow);
		}
		else
		{
			//重新设置数据
			Cast<UFVMEditUI_EditRowTableItem>(this->EditorTab_ListViewPanel->GetChildAt(LocalIndex))
				->InitData(this);
			//初始化数据
			Cast<UFVMEditUI_EditRowTableItem>(this->EditorTab_ListViewPanel->GetChildAt(LocalIndex))
				->SetRowName(CurRowName.ToString());
		}

		LocalIndex++;
	}
}

UFVMEditUI_GameMapEdit* const UGameMapUI_EditorTab::GetGameMapEditor()
{
	return this->FVMEditUI_GameMapEdit;
}

void UGameMapUI_EditorTab::LoadRowDatas(FName RowName)
{

	//如果加载的行名称为空或者为相同名称则不会继续加载
	if (
		RowName.IsEqual(FName(TEXT("")))
		||
		this->FVMEditUI_GameMapEdit->GetCurEditRowName().IsEqual(RowName)
		)
	{
		return;
	}

	if (this->FVMEditUI_GameMapEdit->GetGameMapRowNames().Find(RowName) != INDEX_NONE)
	{
		//this->CurEditRowName = RowName;
		//this->CurEditData = *((FGameMapData*)(this->GameMapData->FindRow<FGameMapData>(RowName, TEXT("GameMapDatas"))));

		//初始化列表项
		//this->InitEditorTabListItems();
	}

}

void UGameMapUI_EditorTab::AddNewRow(FName NewRowName)
{
	//如果加载的行名称为空或者为相同名称则不会继续加载
	if (
		NewRowName.IsEqual(FName(TEXT("")))
		||
		this->FVMEditUI_GameMapEdit->GetCurEditRowName().IsEqual(NewRowName)
		)
	{
		return;
	}

	if (this->FVMEditUI_GameMapEdit->GetGameMapRowNames().Find(NewRowName) == INDEX_NONE)
	{
		//添加新行
		FGameMapData NewData;
		NewData.M_FLevelConfig.LevelName = NewRowName.ToString();
		this->FVMEditUI_GameMapEdit->GetGameMapData()->AddRow(NewRowName, NewData);

		//刷新行
		this->FVMEditUI_GameMapEdit->InitGameMapListItems();
		this->InitEditorTabListItems();
	}
}
