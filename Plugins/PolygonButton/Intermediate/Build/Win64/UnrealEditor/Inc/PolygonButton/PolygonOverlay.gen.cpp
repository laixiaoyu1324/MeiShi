// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "PolygonButton/Public/PolygonOverlay.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodePolygonOverlay() {}
// Cross Module References
	POLYGONBUTTON_API UClass* Z_Construct_UClass_UPolygonOverlay_NoRegister();
	POLYGONBUTTON_API UClass* Z_Construct_UClass_UPolygonOverlay();
	UMG_API UClass* Z_Construct_UClass_UPanelWidget();
	UPackage* Z_Construct_UPackage__Script_PolygonButton();
	UMG_API UClass* Z_Construct_UClass_UWidget_NoRegister();
	POLYGONBUTTON_API UClass* Z_Construct_UClass_UPolygonOverlaySlot_NoRegister();
// End Cross Module References
	DEFINE_FUNCTION(UPolygonOverlay::execAddChildToOverlay)
	{
		P_GET_OBJECT(UWidget,Z_Param_Content);
		P_FINISH;
		P_NATIVE_BEGIN;
		*(UPolygonOverlaySlot**)Z_Param__Result=P_THIS->AddChildToOverlay(Z_Param_Content);
		P_NATIVE_END;
	}
	void UPolygonOverlay::StaticRegisterNativesUPolygonOverlay()
	{
		UClass* Class = UPolygonOverlay::StaticClass();
		static const FNameNativePtrPair Funcs[] = {
			{ "AddChildToOverlay", &UPolygonOverlay::execAddChildToOverlay },
		};
		FNativeFunctionRegistrar::RegisterFunctions(Class, Funcs, UE_ARRAY_COUNT(Funcs));
	}
	struct Z_Construct_UFunction_UPolygonOverlay_AddChildToOverlay_Statics
	{
		struct PolygonOverlay_eventAddChildToOverlay_Parms
		{
			UWidget* Content;
			UPolygonOverlaySlot* ReturnValue;
		};
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_Content_MetaData[];
#endif
		static const UECodeGen_Private::FObjectPropertyParams NewProp_Content;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_ReturnValue_MetaData[];
#endif
		static const UECodeGen_Private::FObjectPropertyParams NewProp_ReturnValue;
		static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[];
#endif
		static const UECodeGen_Private::FFunctionParams FuncParams;
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_UPolygonOverlay_AddChildToOverlay_Statics::NewProp_Content_MetaData[] = {
		{ "EditInline", "true" },
	};
#endif
	const UECodeGen_Private::FObjectPropertyParams Z_Construct_UFunction_UPolygonOverlay_AddChildToOverlay_Statics::NewProp_Content = { "Content", nullptr, (EPropertyFlags)0x0010000000080080, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(PolygonOverlay_eventAddChildToOverlay_Parms, Content), Z_Construct_UClass_UWidget_NoRegister, METADATA_PARAMS(Z_Construct_UFunction_UPolygonOverlay_AddChildToOverlay_Statics::NewProp_Content_MetaData, UE_ARRAY_COUNT(Z_Construct_UFunction_UPolygonOverlay_AddChildToOverlay_Statics::NewProp_Content_MetaData)) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_UPolygonOverlay_AddChildToOverlay_Statics::NewProp_ReturnValue_MetaData[] = {
		{ "EditInline", "true" },
	};
#endif
	const UECodeGen_Private::FObjectPropertyParams Z_Construct_UFunction_UPolygonOverlay_AddChildToOverlay_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000080588, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(PolygonOverlay_eventAddChildToOverlay_Parms, ReturnValue), Z_Construct_UClass_UPolygonOverlaySlot_NoRegister, METADATA_PARAMS(Z_Construct_UFunction_UPolygonOverlay_AddChildToOverlay_Statics::NewProp_ReturnValue_MetaData, UE_ARRAY_COUNT(Z_Construct_UFunction_UPolygonOverlay_AddChildToOverlay_Statics::NewProp_ReturnValue_MetaData)) };
	const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UPolygonOverlay_AddChildToOverlay_Statics::PropPointers[] = {
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UPolygonOverlay_AddChildToOverlay_Statics::NewProp_Content,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UPolygonOverlay_AddChildToOverlay_Statics::NewProp_ReturnValue,
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_UPolygonOverlay_AddChildToOverlay_Statics::Function_MetaDataParams[] = {
		{ "Category", "Widget" },
		{ "Comment", "/**  */" },
		{ "ModuleRelativePath", "Public/PolygonOverlay.h" },
	};
#endif
	const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UPolygonOverlay_AddChildToOverlay_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UPolygonOverlay, nullptr, "AddChildToOverlay", nullptr, nullptr, sizeof(Z_Construct_UFunction_UPolygonOverlay_AddChildToOverlay_Statics::PolygonOverlay_eventAddChildToOverlay_Parms), Z_Construct_UFunction_UPolygonOverlay_AddChildToOverlay_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UPolygonOverlay_AddChildToOverlay_Statics::PropPointers), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04020401, 0, 0, METADATA_PARAMS(Z_Construct_UFunction_UPolygonOverlay_AddChildToOverlay_Statics::Function_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UFunction_UPolygonOverlay_AddChildToOverlay_Statics::Function_MetaDataParams)) };
	UFunction* Z_Construct_UFunction_UPolygonOverlay_AddChildToOverlay()
	{
		static UFunction* ReturnFunction = nullptr;
		if (!ReturnFunction)
		{
			UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UPolygonOverlay_AddChildToOverlay_Statics::FuncParams);
		}
		return ReturnFunction;
	}
	IMPLEMENT_CLASS_NO_AUTO_REGISTRATION(UPolygonOverlay);
	UClass* Z_Construct_UClass_UPolygonOverlay_NoRegister()
	{
		return UPolygonOverlay::StaticClass();
	}
	struct Z_Construct_UClass_UPolygonOverlay_Statics
	{
		static UObject* (*const DependentSingletons[])();
		static const FClassFunctionLinkInfo FuncInfo[];
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[];
#endif
		static const FCppClassTypeInfoStatic StaticCppClassTypeInfo;
		static const UECodeGen_Private::FClassParams ClassParams;
	};
	UObject* (*const Z_Construct_UClass_UPolygonOverlay_Statics::DependentSingletons[])() = {
		(UObject* (*)())Z_Construct_UClass_UPanelWidget,
		(UObject* (*)())Z_Construct_UPackage__Script_PolygonButton,
	};
	const FClassFunctionLinkInfo Z_Construct_UClass_UPolygonOverlay_Statics::FuncInfo[] = {
		{ &Z_Construct_UFunction_UPolygonOverlay_AddChildToOverlay, "AddChildToOverlay" }, // 3398621310
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UPolygonOverlay_Statics::Class_MetaDataParams[] = {
		{ "Comment", "/**\n * Similar to UOverlay except that it dispatches event to UPolygonButton\n */" },
		{ "IncludePath", "PolygonOverlay.h" },
		{ "ModuleRelativePath", "Public/PolygonOverlay.h" },
		{ "ToolTip", "Similar to UOverlay except that it dispatches event to UPolygonButton" },
	};
#endif
	const FCppClassTypeInfoStatic Z_Construct_UClass_UPolygonOverlay_Statics::StaticCppClassTypeInfo = {
		TCppClassTypeTraits<UPolygonOverlay>::IsAbstract,
	};
	const UECodeGen_Private::FClassParams Z_Construct_UClass_UPolygonOverlay_Statics::ClassParams = {
		&UPolygonOverlay::StaticClass,
		nullptr,
		&StaticCppClassTypeInfo,
		DependentSingletons,
		FuncInfo,
		nullptr,
		nullptr,
		UE_ARRAY_COUNT(DependentSingletons),
		UE_ARRAY_COUNT(FuncInfo),
		0,
		0,
		0x00B000A0u,
		METADATA_PARAMS(Z_Construct_UClass_UPolygonOverlay_Statics::Class_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UClass_UPolygonOverlay_Statics::Class_MetaDataParams))
	};
	UClass* Z_Construct_UClass_UPolygonOverlay()
	{
		if (!Z_Registration_Info_UClass_UPolygonOverlay.OuterSingleton)
		{
			UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_UPolygonOverlay.OuterSingleton, Z_Construct_UClass_UPolygonOverlay_Statics::ClassParams);
		}
		return Z_Registration_Info_UClass_UPolygonOverlay.OuterSingleton;
	}
	template<> POLYGONBUTTON_API UClass* StaticClass<UPolygonOverlay>()
	{
		return UPolygonOverlay::StaticClass();
	}
	DEFINE_VTABLE_PTR_HELPER_CTOR(UPolygonOverlay);
	struct Z_CompiledInDeferFile_FID_FVM_Plugins_PolygonButton_Source_PolygonButton_Public_PolygonOverlay_h_Statics
	{
		static const FClassRegisterCompiledInInfo ClassInfo[];
	};
	const FClassRegisterCompiledInInfo Z_CompiledInDeferFile_FID_FVM_Plugins_PolygonButton_Source_PolygonButton_Public_PolygonOverlay_h_Statics::ClassInfo[] = {
		{ Z_Construct_UClass_UPolygonOverlay, UPolygonOverlay::StaticClass, TEXT("UPolygonOverlay"), &Z_Registration_Info_UClass_UPolygonOverlay, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(UPolygonOverlay), 4276238328U) },
	};
	static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_FVM_Plugins_PolygonButton_Source_PolygonButton_Public_PolygonOverlay_h_2046257954(TEXT("/Script/PolygonButton"),
		Z_CompiledInDeferFile_FID_FVM_Plugins_PolygonButton_Source_PolygonButton_Public_PolygonOverlay_h_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_FVM_Plugins_PolygonButton_Source_PolygonButton_Public_PolygonOverlay_h_Statics::ClassInfo),
		nullptr, 0,
		nullptr, 0);
PRAGMA_ENABLE_DEPRECATION_WARNINGS
