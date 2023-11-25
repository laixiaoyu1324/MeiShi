// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GameSystem/Item/Material/MaterialBaseStruct.h"
#include "CardChangeJobMaterialStruct.generated.h"

//卡片转职序号
UENUM(BlueprintType)
enum class ECardChangeType : uint8
{
	E_A UMETA(DisplayName = "A"),
	E_B UMETA(DisplayName = "B"),
	E_C UMETA(DisplayName = "C"),
};

//材料->卡片转职材料
USTRUCT(BlueprintType)
struct FCardChangeJobMaterial : public FMaterialBase {
	GENERATED_USTRUCT_BODY()
public:
	//转职目标卡片
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString M_TargetCardName = "";
	//转职的类型
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		ECardChangeType M_CardChangeType = ECardChangeType::E_A;
};

UCLASS()
class FVM_API UCardChangeJobMaterialStruct : public UObject
{
	GENERATED_BODY()
public:
	static FString DataTablePath;
};
