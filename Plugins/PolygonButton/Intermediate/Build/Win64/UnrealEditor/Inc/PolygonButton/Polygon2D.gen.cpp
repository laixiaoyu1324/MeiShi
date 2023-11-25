// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "PolygonButton/Public/Polygon2D.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodePolygon2D() {}
// Cross Module References
	POLYGONBUTTON_API UScriptStruct* Z_Construct_UScriptStruct_FPolygon2D();
	UPackage* Z_Construct_UPackage__Script_PolygonButton();
	COREUOBJECT_API UScriptStruct* Z_Construct_UScriptStruct_FVector2D();
// End Cross Module References
	static FStructRegistrationInfo Z_Registration_Info_UScriptStruct_Polygon2D;
class UScriptStruct* FPolygon2D::StaticStruct()
{
	if (!Z_Registration_Info_UScriptStruct_Polygon2D.OuterSingleton)
	{
		Z_Registration_Info_UScriptStruct_Polygon2D.OuterSingleton = GetStaticStruct(Z_Construct_UScriptStruct_FPolygon2D, Z_Construct_UPackage__Script_PolygonButton(), TEXT("Polygon2D"));
	}
	return Z_Registration_Info_UScriptStruct_Polygon2D.OuterSingleton;
}
template<> POLYGONBUTTON_API UScriptStruct* StaticStruct<FPolygon2D>()
{
	return FPolygon2D::StaticStruct();
}
	struct Z_Construct_UScriptStruct_FPolygon2D_Statics
	{
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Struct_MetaDataParams[];
#endif
		static void* NewStructOps();
		static const UECodeGen_Private::FStructPropertyParams NewProp_Vertices_Inner;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_Vertices_MetaData[];
#endif
		static const UECodeGen_Private::FArrayPropertyParams NewProp_Vertices;
		static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
		static const UECodeGen_Private::FStructParams ReturnStructParams;
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FPolygon2D_Statics::Struct_MetaDataParams[] = {
		{ "ModuleRelativePath", "Public/Polygon2D.h" },
	};
#endif
	void* Z_Construct_UScriptStruct_FPolygon2D_Statics::NewStructOps()
	{
		return (UScriptStruct::ICppStructOps*)new UScriptStruct::TCppStructOps<FPolygon2D>();
	}
	const UECodeGen_Private::FStructPropertyParams Z_Construct_UScriptStruct_FPolygon2D_Statics::NewProp_Vertices_Inner = { "Vertices", nullptr, (EPropertyFlags)0x0000000000000000, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, 1, 0, Z_Construct_UScriptStruct_FVector2D, METADATA_PARAMS(nullptr, 0) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FPolygon2D_Statics::NewProp_Vertices_MetaData[] = {
		{ "Category", "Data" },
		{ "ModuleRelativePath", "Public/Polygon2D.h" },
	};
#endif
	const UECodeGen_Private::FArrayPropertyParams Z_Construct_UScriptStruct_FPolygon2D_Statics::NewProp_Vertices = { "Vertices", nullptr, (EPropertyFlags)0x0010000000000001, UECodeGen_Private::EPropertyGenFlags::Array, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(FPolygon2D, Vertices), EArrayPropertyFlags::None, METADATA_PARAMS(Z_Construct_UScriptStruct_FPolygon2D_Statics::NewProp_Vertices_MetaData, UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FPolygon2D_Statics::NewProp_Vertices_MetaData)) };
	const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UScriptStruct_FPolygon2D_Statics::PropPointers[] = {
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FPolygon2D_Statics::NewProp_Vertices_Inner,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FPolygon2D_Statics::NewProp_Vertices,
	};
	const UECodeGen_Private::FStructParams Z_Construct_UScriptStruct_FPolygon2D_Statics::ReturnStructParams = {
		(UObject* (*)())Z_Construct_UPackage__Script_PolygonButton,
		nullptr,
		&NewStructOps,
		"Polygon2D",
		sizeof(FPolygon2D),
		alignof(FPolygon2D),
		Z_Construct_UScriptStruct_FPolygon2D_Statics::PropPointers,
		UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FPolygon2D_Statics::PropPointers),
		RF_Public|RF_Transient|RF_MarkAsNative,
		EStructFlags(0x00000201),
		METADATA_PARAMS(Z_Construct_UScriptStruct_FPolygon2D_Statics::Struct_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FPolygon2D_Statics::Struct_MetaDataParams))
	};
	UScriptStruct* Z_Construct_UScriptStruct_FPolygon2D()
	{
		if (!Z_Registration_Info_UScriptStruct_Polygon2D.InnerSingleton)
		{
			UECodeGen_Private::ConstructUScriptStruct(Z_Registration_Info_UScriptStruct_Polygon2D.InnerSingleton, Z_Construct_UScriptStruct_FPolygon2D_Statics::ReturnStructParams);
		}
		return Z_Registration_Info_UScriptStruct_Polygon2D.InnerSingleton;
	}
	struct Z_CompiledInDeferFile_FID_FVM_Plugins_PolygonButton_Source_PolygonButton_Public_Polygon2D_h_Statics
	{
		static const FStructRegisterCompiledInInfo ScriptStructInfo[];
	};
	const FStructRegisterCompiledInInfo Z_CompiledInDeferFile_FID_FVM_Plugins_PolygonButton_Source_PolygonButton_Public_Polygon2D_h_Statics::ScriptStructInfo[] = {
		{ FPolygon2D::StaticStruct, Z_Construct_UScriptStruct_FPolygon2D_Statics::NewStructOps, TEXT("Polygon2D"), &Z_Registration_Info_UScriptStruct_Polygon2D, CONSTRUCT_RELOAD_VERSION_INFO(FStructReloadVersionInfo, sizeof(FPolygon2D), 3207878370U) },
	};
	static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_FVM_Plugins_PolygonButton_Source_PolygonButton_Public_Polygon2D_h_3681930537(TEXT("/Script/PolygonButton"),
		nullptr, 0,
		Z_CompiledInDeferFile_FID_FVM_Plugins_PolygonButton_Source_PolygonButton_Public_Polygon2D_h_Statics::ScriptStructInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_FVM_Plugins_PolygonButton_Source_PolygonButton_Public_Polygon2D_h_Statics::ScriptStructInfo),
		nullptr, 0);
PRAGMA_ENABLE_DEPRECATION_WARNINGS
