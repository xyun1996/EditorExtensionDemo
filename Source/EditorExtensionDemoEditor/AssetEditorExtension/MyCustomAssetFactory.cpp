// Copyright Epic Games, Inc. All Rights Reserved.

#include "MyCustomAssetFactory.h"

#include "AssetToolsModule.h"
#include "CustomAsset/MyCustomAsset.h"
#include "Internationalization/Text.h"

#define LOCTEXT_NAMESPACE "MyCustomAssetFactory"

UMyCustomAssetFactory::UMyCustomAssetFactory()
	: MenuCategory(EAssetTypeCategories::Misc)
{
	bCreateNew = true;
	bEditAfterNew = true;
	SupportedClass = UMyCustomAsset::StaticClass();

	if (FModuleManager::Get().IsModuleLoaded("AssetTools"))
	{
		IAssetTools& AssetTools = FModuleManager::GetModuleChecked<FAssetToolsModule>("AssetTools").Get();
		const uint32 FoundCategory = AssetTools.FindAdvancedAssetCategory(TEXT("EditorExtensionDemo"));
		if (FoundCategory != EAssetTypeCategories::None)
		{
			MenuCategory = FoundCategory;
		}
	}

	SubMenus.Add(LOCTEXT("EditorExtensionDemoSubmenu", "Editor Extension Demo"));
}

UObject* UMyCustomAssetFactory::FactoryCreateNew(
	UClass* InClass,
	UObject* InParent,
	FName InName,
	EObjectFlags Flags,
	UObject* Context,
	FFeedbackContext* Warn,
	FName CallingContext)
{
	return NewObject<UMyCustomAsset>(InParent, InClass, InName, Flags);
}

uint32 UMyCustomAssetFactory::GetMenuCategories() const
{
	return MenuCategory;
}

FText UMyCustomAssetFactory::GetDisplayName() const
{
	return LOCTEXT("MyCustomAssetFactoryDisplayName", "Custom Asset");
}

const TArray<FText>& UMyCustomAssetFactory::GetMenuCategorySubMenus() const
{
	return SubMenus;
}

#undef LOCTEXT_NAMESPACE
