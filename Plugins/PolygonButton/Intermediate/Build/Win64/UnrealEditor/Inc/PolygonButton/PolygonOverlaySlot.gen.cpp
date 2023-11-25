// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "PolygonButton/Public/PolygonOverlaySlot.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodePolygonOverlaySlot() {}
// Cross Module References
	POLYGONBUTTON_API UClass* Z_Construct_UClass_UPolygonOverlaySlot_NoRegister();
	POLYGONBUTTON_API UClass* Z_Construct_UClass_UPolygonOverlaySlot();
	UMG_API UClass* Z_Construct_UClass_UOverlaySlot();
	UPackage* Z_Construct_UPackage__Script_PolygonButton();
// End Cross Module References
	void UPolygonOverlaySlot::StaticRegisterNativesUPolygonOverlaySlot()
	{
	}
	IMPLEMENT_CLASS_NO_AUTO_REGISTRATION(UPolygonOverlaySlot);
	UClass* Z_Construct_UClass_UPolygonOverlaySlot_NoRegister()
	{
		return UPolygonOverlaySlot::StaticClass();
	}
	struct Z_Construct_UClass_UPolygonOverlaySlot_Statics
	{
		static UObject* (*const DependentSingletons[])();
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[];
#endif
		static const FCppClassTypeInfoStatic StaticCppClassTypeInfo;
		static const UECodeGen_Private::FClassParams ClassParams;
	};
	UObject* (*const Z_Construct_UClass_UPolygonOverlaySlot_Statics::DependentSingletons[])() = {
		(UObject* (*)())Z_Construct_UClass_UOverlaySlot,
		(UObject* (*)())Z_Construct_UPackage__Script_PolygonButton,
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UPolygonOverlaySlot_Statics::Class_MetaDataParams[] = {
		{ "Comment", "/**\n * Slot for the UPolygonOverlay panel\n * Allows content to be hover above other content\n */" },
		{ "IncludePath", "PolygonOverlaySlot.h" },
		{ "ModuleRelativePath", "Public/PolygonOverlaySlot.h" },
		{ "ToolTip", "Slot for the UPolygonOverlay panel\nAllows content to be hover above other content" },
	};
#endif
	const FCppClassTypeInfoStatic Z_Construct_UClass_UPolygonOverlaySlot_Statics::StaticCppClassTypeInfo = {
		TCppClassTypeTraits<UPolygonOverlaySlot>::IsAbstract,
	};
	const UECodeGen_Private::FClassParams Z_Construct_UClass_UPolygonOverlaySlot_Statics::ClassParams = {
		&UPolygonOverlaySlot::StaticClass,
		nullptr,
		&StaticCppClassTypeInfo,
		DependentSingletons,
		nullptr,
		nullptr,
		nullptr,
		UE_ARRAY_COUNT(DependentSingletons),
		0,
		0,
		0,
		0x00B000A0u,
		METADATA_PARAMS(Z_Construct_UClass_UPolygonOverlaySlot_Statics::Class_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UClass_UPolygonOverlaySlot_Statics::Class_MetaDataParams))
	};
	UClass* Z_Construct_UClass_UPolygonOverlaySlot()
	{
		if (!Z_Registration_Info_UClass_UPolygonOverlaySlot.OuterSingleton)
		{
			UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_UPolygonOverlaySlot.OuterSingleton, Z_Construct_UClass_UPolygonOverlaySlot_Statics::ClassParams);
		}
		return Z_Registration_Info_UClass_UPolygonOverlaySlot.OuterSingleton;
	}
	template<> POLYGONBUTTON_API UClass* StaticClass<UPolygonOverlaySlot>()
	{
		return UPolygonOverlaySlot::StaticClass();
	}
	DEFINE_VTABLE_PTR_HELPER_CTOR(UPolygonOverlaySlot);
	struct Z_CompiledInDeferFile_FID_FVM_Plugins_PolygonButton_Source_PolygonButton_Public_PolygonOverlaySlot_h_Statics
	{
		static const FClassRegisterCompiledInInfo ClassInfo[];
	};
	const FClassRegisterCompiledInInfo Z_CompiledInDeferFile_FID_FVM_Plugins_PolygonButton_Source_PolygonButton_Public_PolygonOverlaySlot_h_Statics::ClassInfo[] = {
		{ Z_Construct_UClass_UPolygonOverlaySlot, UPolygonOverlaySlot::StaticClass, TEXT("UPolygonOverlaySlot"), &Z_Registration_Info_UClass_UPolygonOverlaySlot, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(UPolygonOverlaySlot), 1775215178U) },
	};
	static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_FVM_Plugins_PolygonButton_Source_PolygonButton_Public_PolygonOverlaySlot_h_3227205739(TEXT("/Script/PolygonButton"),
		Z_CompiledInDeferFile_FID_FVM_Plugins_PolygonButton_Source_PolygonButton_Public_PolygonOverlaySlot_h_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_FVM_Plugins_PolygonButton_Source_PolygonButton_Public_PolygonOverlaySlot_h_Statics::ClassInfo),
		nullptr, 0,
		nullptr, 0);
PRAGMA_ENABLE_DEPRECATION_WARNINGS
