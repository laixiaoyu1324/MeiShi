// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetBase.h"

#include "Game/UI/Synthesis/SynModel_Structures.h"

#include "GameSystem/Tools/GameSystemFunction.h"
#include "GameSystem/PlayerStructManager.h"
#include "GameSystem/Tools/ItemLoadManager.h"
#include "GameSystem/Item/ItemStruct.h"

#include <Components/UniformGridPanel.h>
#include "UI_PlayerSynthesis.generated.h"

class USynModel_WepaonGems;
class UUI_PlayerBagCardGrid;
class UUI_PlayerBagMaterialGrid;
class UUniformGridPanel;
class UScrollBox;
class UTextBlock;
class UButton;
class UCanvasPanel;
class UImage;

UCLASS()
class FVM_API UUI_PlayerSynthesis : public UWidgetBase
{
	GENERATED_BODY()
public:
	//初始化
	virtual bool Initialize() override;
public:
	//显示界面
	UFUNCTION()
		void ShowPanel(UCanvasPanel* _UCanvasPanel, uint8 _Index);
	//关闭界面
	UFUNCTION(BlueprintCallable)
		void ClearPanel();
public:
	//-----------------------------------【合成屋大选项卡】宝石强化-选项卡-----------------------------------------
	//显示制作卡片
	UFUNCTION(BlueprintCallable)
		void ShowMakeCard();
	//显示强化卡片
	UFUNCTION()
		void ShowUpCard();
	//显示卡片转职
	UFUNCTION()
		void ShowChangeCard();
	//显示武器开槽镶嵌界面
	UFUNCTION()
		void ShowWeaponGem();
	//显示宝石强化界面
	UFUNCTION()
		void ShowGemUp();
	//显示宝石分解界面
	UFUNCTION()
		void ShowGemSplit();
	/*-------------------------------------------合成屋-界面-成员-共享的-------------------------------------------------*/
public:
	UPROPERTY()
		TArray<float> M_UpGradeRate_0_Arrays_2_Low = { 0.f,0.f, 48.0, 32.f,   19.f,    16.f,    10.f,    8.f,    4.f,  1.0f,   1.0f,  1.0f,  1.0f ,  1.0f, 1.0f, 1.0f };
	UPROPERTY()
		TArray<float> M_UpGradeRate_0_Arrays_2_Mid = { 0.f,0.f, 51.0, 36.f,   22.f,    18.f,    12.f,    9.f,    5.f,  2.f,    1.0f,  1.0f,  1.0f ,  1.0f, 1.0f, 1.0f };
	UPROPERTY()
		TArray<float> M_UpGradeRate_0_Arrays_2_Hei = { 0.f,0.f, 60.0, 42.90f, 24.20f,  20.10f,  13.20f,  10.6f,  6.0f, 2.20f,  1.80f, 1.70f, 1.60f , 1.40f,1.3f, 1.0f };

	UPROPERTY()
		TArray<float> M_UpGradeRate_0_Arrays_1_Low = { 0.f,72.0f, 56.0f, 48.0f,  32.0f,  27.0f, 21.0f,  17.0f, 10.0f,   3.0f,  3.0f,   3.0f,   3.0f ,  2.0f ,  2.0f,  2.0f,  1.0f ,0.0f };
	UPROPERTY()
		TArray<float> M_UpGradeRate_0_Arrays_1_Mid = { 0.f,80.0f, 72.0f, 50.0f,  36.0f,  30.0f, 24.0f,  19.0f, 5.f,     12.0f, 4.0f,   3.0f,   3.0f,   3.0f ,  2.0f,  2.0f,  2.0f ,0.0f };
	UPROPERTY()
		TArray<float> M_UpGradeRate_0_Arrays_1_Hei = { 0.f,88.0f, 79.2f, 55.0f,  40.3f,  33.0f, 26.40f, 21.2f, 13.2f,   4.50f, 4.60f,  4.3f,   4.0f ,  4.0f,   3.40f, 3.0f , 0.0f ,0.0f };

	UPROPERTY()
		TArray<float> M_UpGradeRate_0_Arrays_0_Low = { 100.0f,  100.0f,  80.0f,   56.0f,    40.0f,    32.0f,   26.0f,    21.0f,     16.0f,    11.0f,     10.0f,     9.0f,     8.0f ,    8.0f ,     7.0f,  6.0f ,  0.0f };
	UPROPERTY()
		TArray<float> M_UpGradeRate_0_Arrays_0_Mid = { 100.0f,  100.0f,  88.0f,   62.0f,    45.0f,    36.0f,   29.0f,    24.0f,     20.0f,    12.0f,     11.0f,     10.0f,    9.0f,     9.0f ,     8.0f,  7.0f ,  0.0f };
	UPROPERTY()
		TArray<float> M_UpGradeRate_0_Arrays_0_Hei = { 100.0f,   100.0f, 96.80f,  68.60f,   49.50f,   39.60f,  31.90f,   26.40f,    22.0f,    13.50f,    12.50f,    11.60f,   10.7f,    10.1f ,    9.5f,  8.80f , 0.0f };

	UPROPERTY()
		TArray<float> M_UpGradeRate_0_Arrays_P1_Low = { 100.0f,  100.0f, 100.0f,   72.0f,    72.0f,    72.0f,   72.0f,   72.0f,    72.0f,    72.0f,    72.0f,    72.0f,   72.0f ,   72.0f ,    72.0f,  72.0f ,72.0f };
	UPROPERTY()
		TArray<float> M_UpGradeRate_0_Arrays_P1_Mid = { 100.0f,  100.0f, 100.0f,   80.0f,    80.0f,    80.0f,   80.0f,   80.0f,    80.0f,    80.0f,    80.0f,    80.0f,   80.0f,    80.0f,     80.0f,  80.0f ,80.0f };
	UPROPERTY()
		TArray<float> M_UpGradeRate_0_Arrays_P1_Hei = { 100.0f,   100.0f, 100.0f,   88.0f,   88.0f,     88.0f,   88.0f,   88.0f,    88.0f,    88.0f,    88.0f,    88.0f,   88.0f,    88.0f,    88.0f,  88.0f, 88.0f };

	//保险金金额
	UPROPERTY()
		TArray<float> M_CardChangeJob_Insurance = { 5,10,30,50,70,90,110,190,270,500,1000,2000,5000,20000,50000,100000,120000 };
	//卡片强化卡片等级需要的保险金金额
	UPROPERTY()
		TArray<int32> LInsurance_CoinNum = { 0,20,40,80,100,200,500,500,600,1000,3000,10000,18000,32000,45000,50000 ,100000 };
private:
	//(宝石卡片，神级进化)等界面   选项卡切换功能(宝石卡片，神级进化)
	UPROPERTY()
		TArray<UWidget*> M_TabSelectPanels;
public:
	//金币
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
		FString M_Coin_0_Text = FString(TEXT("0"));
	//点券
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
		FString M_Coin_2_Text = FString(TEXT("0"));
	//保险金需要的点券
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
		FString M_Insurance_Coin_2_Text = FString(TEXT("0"));
private:
	//-------------------------卡片界面显示与隐藏-------------------------
	//卡片界面1[主容器(包含：卡片界面，装备界面)]
	UPROPERTY()
		UWidget* M_CardsPanel_1 = nullptr;
	//[卡片界面主容器的子界面【卡片界面，装备界面】]
	UPROPERTY()
		TArray<UWidget*> M_TabWidgets_Panels;
	//卡片界面2---保险金
	UPROPERTY()
		UWidget* M_CardsPanel_2 = nullptr;
	//强化四叶草界面---四叶草界面
	UPROPERTY()
		UWidget* M_ColoverPanel = nullptr;
	//-------------------------------------------------------------------
		//制作卡片
	UPROPERTY()
		UButton* M_MakeCard_Butt = nullptr;
	//强化
	UPROPERTY()
		UButton* M_UpCard_Butt = nullptr;
	//转职
	UPROPERTY()
		UButton* M_ChangeCard_Butt = nullptr;
	//制作界面 
	UPROPERTY()
		UCanvasPanel* M_MakeCardP = nullptr;
	//强化界面
	UPROPERTY()
		UCanvasPanel* M_UpCardP = nullptr;
	//转职界面
	UPROPERTY()
		UCanvasPanel* M_ChangeCardP = nullptr;
	//材料显示界面
	UPROPERTY()
		UUniformGridPanel* M_Materials_Box = nullptr;
	//卡片显示界面
	UPROPERTY()
		UUniformGridPanel* M_Cards_Box = nullptr;

	//卡片显示界面2（集合）
	UPROPERTY()
		TArray<UUniformGridPanel*> M_Cards2_Box;

	//材料显示集合
	UPROPERTY()
		UUniformGridPanel* M_Materials_2_BoxArr = nullptr;

	//----------------------------------
	//当前角色背包容量
	UPROPERTY()
		int32 M_CardBagCount = 0;
public:
	//材料函数绑定
	//绑定函数的名称
	TArray<FMaterialsSerachTypeBind> M_BindFunctionName_Materials;
	//----------------------------------
public:
	//选择加载宝石卡片界面
	UFUNCTION(BlueprintCallable)
		void Select_GemCardPanel();
	//选择加载金卡进化界面
	UFUNCTION(BlueprintCallable)
		void Select_GoldCardUpPanel();
	//更新金币文字
	UFUNCTION(BlueprintCallable)
		void Update_CoinText();
	//显示卡片界面或者保险金界面
	UFUNCTION(BlueprintCallable)
		void ShowCardPanelOrInsurancePanel(ESlateVisibility _CardPanel_State, uint8 _CardPanel_ShowIndex, ESlateVisibility _InsurancePanel_State2, ESlateVisibility _ColoverPanel_State = ESlateVisibility::Collapsed);
public:
	//设置概率文字
	UFUNCTION(BlueprintCallable)
		void SetRateText(const FString& _Text);
	//设置保险金额文字
	UFUNCTION(BlueprintCallable)
		void SetInsuranceText(const FString& _Text);
public:
	//是否勾选保险金
	UFUNCTION(BlueprintCallable)
		bool GetSelectInsurance();
public:
	//材料加载器 
	UPROPERTY()
		UItemLoadManager* M_UItemLoadManager_Item_Material = nullptr;
	//卡片强化界面加载器(共用于 卡片强化，1卡片转职，2金卡进化等等)
	UPROPERTY()
		UItemLoadManager* M_UItemLoadManager_CardUpGrade = nullptr;
private:
	//加载器初始化
	UFUNCTION(BlueprintCallable)
		void Loader_Init();

	//----------------------------------四叶草+香料+转职材料【材料区域】--------------------------------------

	//卡片制作滑动界面
	UPROPERTY()
		UScrollBox* M_UScrollBox_Item_Material = nullptr;

	//创建材界面
	UFUNCTION()
		UWidget* WidgetCreate_InitMaterial(UItemDataTable* _Data, int32 _Index);
	//刷新界面
	UFUNCTION()
		void WidgetRefresh_UpdateMaterial(UItemDataTable* _Data, int32 _Index, UWidget* _UWidget);

	//设置->材料数据->四叶草-转职道具-等【材料区域的统一材料设置】
	UFUNCTION()
		void SetMaterialsData(UUI_PlayerBagMaterialGrid* _Grid, UItemDataTable* _CardData, int32 _Index, const TArray<FMaterialsSerachTypeBind>& _BindFuncName);

	//----------------------------------四叶草+香料+转职材料【材料区域】--------------------------------------


	//----------------------------------（强化，转职，等）统一共用滑动界面【卡片区域】--------------------------------------
	//卡片（强化，转职，等）统一共用滑动界面
	UPROPERTY()
		UScrollBox* M_UScrollBox_Card_Uniform = nullptr;
	//卡片（强化，转职，等）统一共用格子界面
	UPROPERTY()
		UUniformGridPanel* M_Uniform_CardGrid = nullptr;
	//界面加载标识(0 卡片强化，1卡片转职，2金卡进化)
	UPROPERTY()
		int32 M_Uniform_LoadCardData_Panel_Index = 0;


	//创建材界面
	UFUNCTION()
		UWidget* WidgetCreate_InitCardUpGrade(UItemDataTable* _Data, int32 _Index);
	//刷新界面
	UFUNCTION()
		void WidgetRefresh_UpdateCardUpGrade(UItemDataTable* _Data, int32 _Index, UWidget* _UWidget);
	//设置卡片数据
	UFUNCTION()
		void SetCardData(UUI_PlayerBagCardGrid* _Grid, UItemDataTable* _Data, int32 _Index);
	//----------------------------------（强化，转职，等）统一共用滑动界面【卡片区域】--------------------------------------








	/*-------------------------------------------------卡片制作--------------------------------------------*/
public:
	//制作卡片制作按钮-开始制作
	UPROPERTY()
		UButton* M_MakeCard_Run_Butt = nullptr;
	//香料按钮
	UPROPERTY()
		UButton* M_Spices_Butt = nullptr;
	//配方按钮
	UPROPERTY()
		UButton* M_SynthesisBlueprint_Butt = nullptr;

	//材料图片
	UPROPERTY()
		UImage* M_Material_0_Image = nullptr;
	UPROPERTY()
		UImage* M_Material_1_Image = nullptr;
	UPROPERTY()
		UImage* M_Material_2_Image = nullptr;
public:
	//合成配方数据
	TArray<FMaterialBase*> M_Materials_CardMake_Arrays_Data;
	//材料数组数据
	TArray<FMaterialBase*> M_MaterialsData;
	//配方数据
	FMaterialBase* M_MaterialBlueprintData = nullptr;
	//当前选中香料的数据索引->指向背包的数据索引
	FMaterialBase* M_MakeCard_CurrentSelectData = nullptr;

	//卡片配方名称
	UPROPERTY()
		FString M_MaterialBlueprintName;
	//制作卡片的目标名称
	UPROPERTY()
		FString M_MaterialBlueprintTargetName;
	//当前选中配方的格子索引
	UPROPERTY()
		int32 M_MakeCard_CurrentSelectBlueprintData_Index = -1;

	//当进行卡片制作的时候->直接加入的位置
	UPROPERTY()
		int32 M_MakeCard_MakeIndex = -1;
	//当前要加入的卡片名称
	UPROPERTY()
		FString M_MakeCard_CardName = TEXT("");

	//选择的香料名称
	UPROPERTY()
		FString M_SelectSpicesName;
	//当前选中香料的格子索引
	UPROPERTY()
		int32 M_MakeCard_CurrentSelect_Index = -1;

	//卡片制作滑动界面
	UPROPERTY()
		UScrollBox* M_UScrollBox_MakeCard = nullptr;
	UPROPERTY()
		UScrollBox* M_UScrollBox_MakeCard_Material = nullptr;

	//----------------------------------卡片制作-----【配方材料区域-卡片区域】----------------------------------
	UPROPERTY()
		UItemLoadManager* M_UItemLoadManager_MakeCard = nullptr;
	UPROPERTY()
		UItemLoadManager* M_UItemLoadManager_MakeCard_Materials = nullptr;

	//创建材界面
	UFUNCTION()
		UWidget* WidgetCreate_InitMakeCard(UItemDataTable* _Data, int32 _Index);
	//刷新界面
	UFUNCTION()
		void WidgetRefresh_UpdateMakeCard(UItemDataTable* _Data, int32 _Index, UWidget* _UWidget);

	//创建材界面
	UFUNCTION()
		UWidget* WidgetCreate_InitMakeCard_Material(UItemDataTable* _Data, int32 _Index);
	//刷新界面
	UFUNCTION()
		void WidgetRefresh_UpdateMakeCard_Material(UItemDataTable* _Data, int32 _Index, UWidget* _UWidget);

	//----------------------------------卡片制作-----配方材料区域-卡片区域】----------------------------------
	//设置制作卡片界面UI的数据
	UFUNCTION()
		void SetMakeCardPanelData(UUI_PlayerBagCardGrid* _Grid, UItemDataTable* _CardData, int32 _Index);
	//设置制作卡片界面-配方材料UI的数据
	UFUNCTION()
		void SetMakeCard_Material_PanelData(UUI_PlayerBagMaterialGrid* _Grid, UItemDataTable* _CardData, int32 _Index);
	//加载材料-卡片制作界面
	UFUNCTION(BlueprintCallable)
		void LoadMaterialsToMakeCard();
	//加载卡片-卡片制作界面
	UFUNCTION(BlueprintCallable)
		void LoadCardsToMakeCard();
	//加载香料-卡片制作界面
	UFUNCTION(BlueprintCallable)
		void LoadSpicesToMakeCard();
	UFUNCTION()
		void CancelSelectMakeCard();
	//取消香料的选择
	UFUNCTION()
		void CancelSelectSpices();
	//完全移除香料
	UFUNCTION()
		void CancelSpicesDestroy();
	//制作卡片
	UFUNCTION()
		void MakeCard();

	//设置合成屋-合成配方
	void SetSynthesisBlueprintData(UUI_PlayerSynthesis* _PlayerSynthesis, FMaterialBase* _MaterialBaseData, int32 _Index);

	//图片设置 给UUI_PlayerBagMaterialGrid::AddSynthesisBlueprint使用的专属函数
	inline void SetImageGreyColor(FMaterialBase* _Item, UImage* _Image, const FString& _Path)
	{
		if (_Item == nullptr)
		{
			UWidgetBase::SetImageBrush(_Image, _Path, FVector(0.3f, 0.3f, 0.3f));
		}
		else
		{
			UWidgetBase::SetImageBrush(_Image, _Path);
		}
	}

	/*-------------------------------------------------卡片制作--------------------------------------------*/







public:
	/*----------------------------------------卡片强化----------------------------------------*/
	//卡片价格显示
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString _CardUpgrate_Price_0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString _CardUpgrate_Price_1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString _CardUpgrate_Price_2;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString _CardUpgrate_Price_3;
public:
	//四叶草按钮
	UPROPERTY()
		UButton* M_Clover_Butt = nullptr;
	//保险按钮
	UPROPERTY()
		UButton* M_Insurance_Butt = nullptr;
	//主卡
	UPROPERTY()
		UButton* M_UpGradeCard_Main_Butt = nullptr;
	//副卡1
	UPROPERTY()
		UButton* M_UpGradeCard_0_Butt = nullptr;
	//副卡2
	UPROPERTY()
		UButton* M_UpGradeCard_1_Butt = nullptr;
	//副卡3
	UPROPERTY()
		UButton* M_UpGradeCard_2_Butt = nullptr;
	//主卡等级
	UPROPERTY()
		UImage* M_UpGradeCard_Main_Grade = nullptr;
	//副卡0等级
	UPROPERTY()
		UImage* M_UpGradeCard_0_Grade = nullptr;
	//副卡1等级
	UPROPERTY()
		UImage* M_UpGradeCard_1_Grade = nullptr;
	//副卡2等级
	UPROPERTY()
		UImage* M_UpGradeCard_2_Grade = nullptr;
	//强化按钮
	UPROPERTY()
		UButton* M_UpGradeCard_Butt = nullptr;
	//强化卡槽->在初始化里面添加
	UPROPERTY(EditAnywhere)
		TArray<UButton*> M_UpGradeSlot;
public:
	//卡片强化文本显示
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString M_UpGradeCardRateText;
	//是否选择了保险
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool M_UpGrade_bSelectInsurance = false;
	//强化界面是否打开
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool M_IsOpenUpCardPanel = false;
	//卡片状态
	UPROPERTY()
		TArray<bool> M_UpGradeCardState = { false,false,false,false };
	//指向背包的卡片数据(对应的UI索引，对应的卡片数据)
	UPROPERTY()
		int32 M_MainCard_Data_Index = -1;
	UPROPERTY()
		TArray<int32> M_UpGradeCard_UI_Index = { -1,-1 ,-1 ,-1 };
	//四叶草的格子
	UPROPERTY()
		UUI_PlayerBagMaterialGrid* M_MakeCard_CloverGrid = nullptr;

	TArray<FItemCard*> M_UpGradeCard_Datas = { nullptr,nullptr ,nullptr ,nullptr };
public:
	//加载卡片-卡片强化界面
	UFUNCTION(BlueprintCallable)
		void LoadCardsToCardUpGrade();
	//加载四叶草-卡片强化界面
	UFUNCTION(BlueprintCallable)
		void LoadCloversToCardUpGrade();
	//取消选择的卡片->取消强化
	UFUNCTION(BlueprintCallable)
		void CancelSelectCard(int32 _Cancel_Index, bool _bReload);
	//取消选择四叶草
	UFUNCTION(BlueprintCallable)
		void CancelSelectClover();
	//四叶草取消选择的按钮
	UFUNCTION(BlueprintCallable)
		UButton* GetCloverCancelButton();
	//获取四叶草数据
	UFUNCTION(BlueprintCallable)
		UUI_PlayerBagMaterialGrid* const GetCloverData();
public:
	//卡片强化升级
	UFUNCTION()
		void CardUpGrade();
	//选择或者取消保险
	UFUNCTION()
		void SelectInsurance();
	//获取概率最大数
	UFUNCTION()
		float GetUpGradeRate();
	//通过卡片价格和等级返回概率（价格-等级-等级差-衰减比例）
	UFUNCTION()
		float GetCardPriceGradeRate(float _Price, int32 _CardGrade, int32 MainCard_CurrentCard_Grade);
	/*----------------------------------------卡片强化----------------------------------------*/






public:
	/*----------------------------------------卡片转职----------------------------------------*/
	//转职材料图片A
	UPROPERTY()
		UImage* M_ChangeJob_0_Image = nullptr;
	//转职材料图片B
	UPROPERTY()
		UImage* M_ChangeJob_1_Image = nullptr;
	//转职材料图片C
	UPROPERTY()
		UImage* M_ChangeJob_2_Image = nullptr;
	//转职材料图片C界面
	UPROPERTY()
		UCanvasPanel* M_ChangeJob_2_P = nullptr;
	//主卡按钮
	UPROPERTY()
		UButton* M_ChangeJobMainCard_Butt = nullptr;
	//转职按钮
	UPROPERTY()
		UButton* M_ChangeJob_Butt = nullptr;
public:
	//转职卡片的概率
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
		FString M_ChangeCard_Rate = FString(TEXT("0 %"));
public:
	//转职卡片的目标名称
	FString M_CardChangeJobTargetName;
	//转职材料在背包的索引
	TArray<int32> M_CardChangeJobMaterialsIndex;
	//转职材料状态(拥有则true，未拥有则false)
	TArray<bool> M_CardChangeJobMaterialsArrays;
	//卡片数据
	FItemCard* M_CardChangeJob_Data = nullptr;
	//当前选则的转职卡片索引（主要用于将数据添加到背包的某一个位置）
	int32 M_CurrentCardChangeJobIndex = -1;
public:
	//加载卡片-卡片转职界面
	UFUNCTION(BlueprintCallable)
		void LoadCardsToCardChange();
	//加载转职材料-卡片转职界面
	UFUNCTION(BlueprintCallable)
		void LoadMaterialsToCardChange();
public:
	//取消转职选择
	UFUNCTION(BlueprintCallable)
		void CancelSelectCardChangeJobs();
	//转职
	UFUNCTION()
		void CardChangeJobs();
	/*----------------------------------------卡片转职----------------------------------------*/





public:

	/*----------------------------------------金卡进化----------------------------------------*/
	//记录卡片状态是否存放了卡片（名称为""表示没有存放卡片）
	UPROPERTY()
		TArray<FString> M_GoldCard_Name_State;
	//(二转普通卡，二转星座卡，选择与取消)
	UPROPERTY()
		TArray<UButton*> M_GoldCard_Select_Button;
	//进化凭证头像
	UPROPERTY()
		UImage* M_GoldCardUpHead = nullptr;
	//卡片进化结果头像
	UPROPERTY()
		UImage* M_GoldCardUpResultHead = nullptr;
	//进化按钮
	UPROPERTY()
		UButton* M_GoldCardUp_Butt = nullptr;
	//进化凭证（道具名称）
	UPROPERTY()
		FString M_GoldCardUp_ItemName = TEXT("");
	//进化对应的卡片（卡片名称）
	UPROPERTY()
		FString M_GoldCardUp_CardName = TEXT("");
public:
	//需要消耗掉的卡片
	UPROPERTY()
		TArray<FString> M_NeedCards;
	//需要消耗掉的卡片等级
	UPROPERTY()
		TArray<int32> M_NeedGradeCards;
	//对应卡片的UI索引
	UPROPERTY()
		TArray<int32> M_NeedCard_UI_Index;
public:
	//显示金卡进化界面
	UFUNCTION(BlueprintCallable)
		void ShowGoldCardUpPanel();
	//加载金卡进化卡片
	UFUNCTION(BlueprintCallable)
		void LoadGoldCard_Card();
	//移除卡槽卡片
	UFUNCTION(BlueprintCallable)
		void RemoveCardSlot(int32 _Index);
	//金卡进化
	UFUNCTION(BlueprintCallable)
		void GoldCardUp();
	/*----------------------------------------金卡进化----------------------------------------*/








private:
	/*----------------------------------------武器开槽镶嵌----------------------------------------*/
	//武器开槽镶嵌-选项卡按钮
	UPROPERTY()
		UButton* M_WepaonGem_TabButt = nullptr;
	//武器开槽镶嵌界面
	UPROPERTY()
		UCanvasPanel* M_WepaonGemP = nullptr;
	//武器开槽镶嵌功能
	UPROPERTY()
		USynModel_WepaonGems* M_USynModel_WepaonGems = nullptr;
public:
	//获取武器宝石界面功能
	UFUNCTION(BlueprintCallable)
		USynModel_WepaonGems* GetWepaonGemsFunction();
	/*----------------------------------------武器开槽镶嵌----------------------------------------*/

	/*----------------------------------------宝石强化----------------------------------------*/
private:
	//宝石强化-选项卡按钮
	UPROPERTY()
		UButton* M_GemUp_TabButt = nullptr;
	//宝石强化界面
	UPROPERTY()
		UCanvasPanel* M_GemUpP = nullptr;
	//宝石强化功能
	UPROPERTY()
		class USynModel_GemsUpGrade* M_USynModel_GemsUpGrade = nullptr;
public:
	//获取武器宝石界面功能
	UFUNCTION(BlueprintCallable)
		class USynModel_GemsUpGrade* GetGemsUpGradeFunction();
	/*----------------------------------------宝石强化----------------------------------------*/


	/*----------------------------------------宝石分解----------------------------------------*/
private:
	//宝石分解-选项卡按钮
	UPROPERTY()
		UButton* M_GemSplit_TabButt = nullptr;
	//宝石分解界面
	UPROPERTY()
		UCanvasPanel* M_GemSplitP = nullptr;
	//宝石分解
	UPROPERTY()
		class USynModel_GemSplit* M_USynModel_GemSplit = nullptr;
public:
	//获取武器宝石界面功能
	UFUNCTION(BlueprintCallable)
		class USynModel_GemSplit* GetGemSplitFunction();
	/*----------------------------------------宝石分解----------------------------------------*/

	/*----------------------------------------道具加载完成结束函数----------------------------------------*/
public:
	//材料加载完成
	bool M_bMaterialFinish = false;
	//卡片加载完成
	bool M_bCardFinish = false;
	//配方材料加载完成
	bool M_bCardBlueprintFinish = false;
public:
	//材料加载结束
	UFUNCTION()
		void MaterialLoadEnd();
	//卡片数据加载结束
	UFUNCTION()
		void CardsLoadEnd();
	//卡片配方加载结束
	UFUNCTION()
		void CardBlueprintsLoadEnd();
public:
	//加载 香料  四叶草 转职道具专属函数
	/*----------------------------------------指定材料加载----------------------------------------*/
	void LoadMaterials_Type(const TArray<FMaterialsSerachTypeBind>& _BindFuncName);
	/*----------------------------------------指定材料加载----------------------------------------*/
};
