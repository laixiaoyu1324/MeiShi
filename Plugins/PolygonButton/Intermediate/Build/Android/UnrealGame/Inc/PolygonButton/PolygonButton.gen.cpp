// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "PolygonButton/Public/PolygonButton.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodePolygonButton() {}
// Cross Module References
	POLYGONBUTTON_API UClass* Z_Construct_UClass_UPolygonButton_NoRegister();
	POLYGONBUTTON_API UClass* Z_Construct_UClass_UPolygonButton();
	UMG_API UClass* Z_Construct_UClass_UButton();
	UPackage* Z_Construct_UPackage__Script_PolygonButton();
	COREUOBJECT_API UScriptStruct* Z_Construct_UScriptStruct_FVector2D();
	POLYGONBUTTON_API UScriptStruct* Z_Construct_UScriptStruct_FPolygon2D();
// End Cross Module References
	DEFINE_FUNCTION(UPolygonButton::execGetPolygonVertices)
	{
		P_FINISH;
		P_NATIVE_BEGIN;
		*(TArray<FVector2D>*)Z_Param__Result=P_THIS->GetPolygonVertices();
		P_NATIVE_END;
	}
	DEFINE_FUNCTION(UPolygonButton::execSetPolygonVertices)
	{
		P_GET_TARRAY_REF(FVector2D,Z_Param_Out_VertexList);
		P_FINISH;
		P_NATIVE_BEGIN;
		P_THIS->SetPolygonVertices(Z_Param_Out_VertexList);
		P_NATIVE_END;
	}
	void UPolygonButton::StaticRegisterNativesUPolygonButton()
	{
		UClass* Class = UPolygonButton::StaticClass();
		static const FNameNativePtrPair Funcs[] = {
			{ "GetPolygonVertices", &UPolygonButton::execGetPolygonVertices },
			{ "SetPolygonVertices", &UPolygonButton::execSetPolygonVertices },
		};
		FNativeFunctionRegistrar::RegisterFunctions(Class, Funcs, UE_ARRAY_COUNT(Funcs));
	}
	struct Z_Construct_UFunction_UPolygonButton_GetPolygonVertices_Statics
	{
		struct PolygonButton_eventGetPolygonVertices_Parms
		{
			TArray<FVector2D> ReturnValue;
		};
		static const UECodeGen_Private::FStructPropertyParams NewProp_ReturnValue_Inner;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_ReturnValue_MetaData[];
#endif
		static const UECodeGen_Private::FArrayPropertyParams NewProp_ReturnValue;
		static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[];
#endif
		static const UECodeGen_Private::FFunctionParams FuncParams;
	};
	const UECodeGen_Private::FStructPropertyParams Z_Construct_UFunction_UPolygonButton_GetPolygonVertices_Statics::NewProp_ReturnValue_Inner = { "ReturnValue", nullptr, (EPropertyFlags)0x0000000000000000, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, 1, 0, Z_Construct_UScriptStruct_FVector2D, METADATA_PARAMS(nullptr, 0) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_UPolygonButton_GetPolygonVertices_Statics::NewProp_ReturnValue_MetaData[] = {
		{ "NativeConst", "" },
	};
#endif
	const UECodeGen_Private::FArrayPropertyParams Z_Construct_UFunction_UPolygonButton_GetPolygonVertices_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000582, UECodeGen_Private::EPropertyGenFlags::Array, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(PolygonButton_eventGetPolygonVertices_Parms, ReturnValue), EArrayPropertyFlags::None, METADATA_PARAMS(Z_Construct_UFunction_UPolygonButton_GetPolygonVertices_Statics::NewProp_ReturnValue_MetaData, UE_ARRAY_COUNT(Z_Construct_UFunction_UPolygonButton_GetPolygonVertices_Statics::NewProp_ReturnValue_MetaData)) };
	const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UPolygonButton_GetPolygonVertices_Statics::PropPointers[] = {
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UPolygonButton_GetPolygonVertices_Statics::NewProp_ReturnValue_Inner,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UPolygonButton_GetPolygonVertices_Statics::NewProp_ReturnValue,
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_UPolygonButton_GetPolygonVertices_Statics::Function_MetaDataParams[] = {
		{ "Category", "PolygonButton" },
		{ "ModuleRelativePath", "Public/PolygonButton.h" },
	};
#endif
	const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UPolygonButton_GetPolygonVertices_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UPolygonButton, nullptr, "GetPolygonVertices", nullptr, nullptr, sizeof(Z_Construct_UFunction_UPolygonButton_GetPolygonVertices_Statics::PolygonButton_eventGetPolygonVertices_Parms), Z_Construct_UFunction_UPolygonButton_GetPolygonVertices_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UPolygonButton_GetPolygonVertices_Statics::PropPointers), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04020401, 0, 0, METADATA_PARAMS(Z_Construct_UFunction_UPolygonButton_GetPolygonVertices_Statics::Function_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UFunction_UPolygonButton_GetPolygonVertices_Statics::Function_MetaDataParams)) };
	UFunction* Z_Construct_UFunction_UPolygonButton_GetPolygonVertices()
	{
		static UFunction* ReturnFunction = nullptr;
		if (!ReturnFunction)
		{
			UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UPolygonButton_GetPolygonVertices_Statics::FuncParams);
		}
		return ReturnFunction;
	}
	struct Z_Construct_UFunction_UPolygonButton_SetPolygonVertices_Statics
	{
		struct PolygonButton_eventSetPolygonVertices_Parms
		{
			TArray<FVector2D> VertexList;
		};
		static const UECodeGen_Private::FStructPropertyParams NewProp_VertexList_Inner;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_VertexList_MetaData[];
#endif
		static const UECodeGen_Private::FArrayPropertyParams NewProp_VertexList;
		static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[];
#endif
		static const UECodeGen_Private::FFunctionParams FuncParams;
	};
	const UECodeGen_Private::FStructPropertyParams Z_Construct_UFunction_UPolygonButton_SetPolygonVertices_Statics::NewProp_VertexList_Inner = { "VertexList", nullptr, (EPropertyFlags)0x0000000000000000, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, 1, 0, Z_Construct_UScriptStruct_FVector2D, METADATA_PARAMS(nullptr, 0) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_UPolygonButton_SetPolygonVertices_Statics::NewProp_VertexList_MetaData[] = {
		{ "NativeConst", "" },
	};
#endif
	const UECodeGen_Private::FArrayPropertyParams Z_Construct_UFunction_UPolygonButton_SetPolygonVertices_Statics::NewProp_VertexList = { "VertexList", nullptr, (EPropertyFlags)0x0010000008000182, UECodeGen_Private::EPropertyGenFlags::Array, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(PolygonButton_eventSetPolygonVertices_Parms, VertexList), EArrayPropertyFlags::None, METADATA_PARAMS(Z_Construct_UFunction_UPolygonButton_SetPolygonVertices_Statics::NewProp_VertexList_MetaData, UE_ARRAY_COUNT(Z_Construct_UFunction_UPolygonButton_SetPolygonVertices_Statics::NewProp_VertexList_MetaData)) };
	const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UPolygonButton_SetPolygonVertices_Statics::PropPointers[] = {
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UPolygonButton_SetPolygonVertices_Statics::NewProp_VertexList_Inner,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UPolygonButton_SetPolygonVertices_Statics::NewProp_VertexList,
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_UPolygonButton_SetPolygonVertices_Statics::Function_MetaDataParams[] = {
		{ "Category", "PolygonButton" },
		{ "ModuleRelativePath", "Public/PolygonButton.h" },
	};
#endif
	const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UPolygonButton_SetPolygonVertices_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UPolygonButton, nullptr, "SetPolygonVertices", nullptr, nullptr, sizeof(Z_Construct_UFunction_UPolygonButton_SetPolygonVertices_Statics::PolygonButton_eventSetPolygonVertices_Parms), Z_Construct_UFunction_UPolygonButton_SetPolygonVertices_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UPolygonButton_SetPolygonVertices_Statics::PropPointers), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04420401, 0, 0, METADATA_PARAMS(Z_Construct_UFunction_UPolygonButton_SetPolygonVertices_Statics::Function_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UFunction_UPolygonButton_SetPolygonVertices_Statics::Function_MetaDataParams)) };
	UFunction* Z_Construct_UFunction_UPolygonButton_SetPolygonVertices()
	{
		static UFunction* ReturnFunction = nullptr;
		if (!ReturnFunction)
		{
			UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UPolygonButton_SetPolygonVertices_Statics::FuncParams);
		}
		return ReturnFunction;
	}
	IMPLEMENT_CLASS_NO_AUTO_REGISTRATION(UPolygonButton);
	UClass* Z_Construct_UClass_UPolygonButton_NoRegister()
	{
		return UPolygonButton::StaticClass();
	}
	struct Z_Construct_UClass_UPolygonButton_Statics
	{
		static UObject* (*const DependentSingletons[])();
		static const FClassFunctionLinkInfo FuncInfo[];
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[];
#endif
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_NormalizedPolygon_MetaData[];
#endif
		static const UECodeGen_Private::FStructPropertyParams NewProp_NormalizedPolygon;
		static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
		static const FCppClassTypeInfoStatic StaticCppClassTypeInfo;
		static const UECodeGen_Private::FClassParams ClassParams;
	};
	UObject* (*const Z_Construct_UClass_UPolygonButton_Statics::DependentSingletons[])() = {
		(UObject* (*)())Z_Construct_UClass_UButton,
		(UObject* (*)())Z_Construct_UPackage__Script_PolygonButton,
	};
	const FClassFunctionLinkInfo Z_Construct_UClass_UPolygonButton_Statics::FuncInfo[] = {
		{ &Z_Construct_UFunction_UPolygonButton_GetPolygonVertices, "GetPolygonVertices" }, // 1149027513
		{ &Z_Construct_UFunction_UPolygonButton_SetPolygonVertices, "SetPolygonVertices" }, // 2030195003
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UPolygonButton_Statics::Class_MetaDataParams[] = {
		{ "Comment", "/**\n * UMG button that use a polygon to determine whether or not to response mouse events\n * Polygon is normalized\n */" },
		{ "IncludePath", "PolygonButton.h" },
		{ "ModuleRelativePath", "Public/PolygonButton.h" },
		{ "ToolTip", "UMG button that use a polygon to determine whether or not to response mouse events\nPolygon is normalized" },
	};
#endif
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UPolygonButton_Statics::NewProp_NormalizedPolygon_MetaData[] = {
		{ "Category", "Interaction" },
		{ "DisplayName", "Polygon" },
		{ "ModuleRelativePath", "Public/PolygonButton.h" },
	};
#endif
	const UECodeGen_Private::FStructPropertyParams Z_Construct_UClass_UPolygonButton_Statics::NewProp_NormalizedPolygon = { "NormalizedPolygon", nullptr, (EPropertyFlags)0x0010000000000001, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(UPolygonButton, NormalizedPolygon), Z_Construct_UScriptStruct_FPolygon2D, METADATA_PARAMS(Z_Construct_UClass_UPolygonButton_Statics::NewProp_NormalizedPolygon_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UPolygonButton_Statics::NewProp_NormalizedPolygon_MetaData)) }; // 3207878370
	const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UClass_UPolygonButton_Statics::PropPointers[] = {
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UPolygonButton_Statics::NewProp_NormalizedPolygon,
	};
	const FCppClassTypeInfoStatic Z_Construct_UClass_UPolygonButton_Statics::StaticCppClassTypeInfo = {
		TCppClassTypeTraits<UPolygonButton>::IsAbstract,
	};
	const UECodeGen_Private::FClassParams Z_Construct_UClass_UPolygonButton_Statics::ClassParams = {
		&UPolygonButton::StaticClass,
		nullptr,
		&StaticCppClassTypeInfo,
		DependentSingletons,
		FuncInfo,
		Z_Construct_UClass_UPolygonButton_Statics::PropPointers,
		nullptr,
		UE_ARRAY_COUNT(DependentSingletons),
		UE_ARRAY_COUNT(FuncInfo),
		UE_ARRAY_COUNT(Z_Construct_UClass_UPolygonButton_Statics::PropPointers),
		0,
		0x00B000A0u,
		METADATA_PARAMS(Z_Construct_UClass_UPolygonButton_Statics::Class_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UClass_UPolygonButton_Statics::Class_MetaDataParams))
	};
	UClass* Z_Construct_UClass_UPolygonButton()
	{
		if (!Z_Registration_Info_UClass_UPolygonButton.OuterSingleton)
		{
			UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_UPolygonButton.OuterSingleton, Z_Construct_UClass_UPolygonButton_Statics::ClassParams);
		}
		return Z_Registration_Info_UClass_UPolygonButton.OuterSingleton;
	}
	template<> POLYGONBUTTON_API UClass* StaticClass<UPolygonButton>()
	{
		return UPolygonButton::StaticClass();
	}
	DEFINE_VTABLE_PTR_HELPER_CTOR(UPolygonButton);
	struct Z_CompiledInDeferFile_FID_FVM_Plugins_PolygonButton_Source_PolygonButton_Public_PolygonButton_h_Statics
	{
		static const FClassRegisterCompiledInInfo ClassInfo[];
	};
	const FClassRegisterCompiledInInfo Z_CompiledInDeferFile_FID_FVM_Plugins_PolygonButton_Source_PolygonButton_Public_PolygonButton_h_Statics::ClassInfo[] = {
		{ Z_Construct_UClass_UPolygonButton, UPolygonButton::StaticClass, TEXT("UPolygonButton"), &Z_Registration_Info_UClass_UPolygonButton, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(UPolygonButton), 2435551728U) },
	};
	static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_FVM_Plugins_PolygonButton_Source_PolygonButton_Public_PolygonButton_h_764869667(TEXT("/Script/PolygonButton"),
		Z_CompiledInDeferFile_FID_FVM_Plugins_PolygonButton_Source_PolygonButton_Public_PolygonButton_h_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_FVM_Plugins_PolygonButton_Source_PolygonButton_Public_PolygonButton_h_Statics::ClassInfo),
		nullptr, 0,
		nullptr, 0);
PRAGMA_ENABLE_DEPRECATION_WARNINGS
