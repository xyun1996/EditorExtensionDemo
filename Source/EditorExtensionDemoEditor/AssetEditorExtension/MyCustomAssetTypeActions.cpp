// Copyright Epic Games, Inc. All Rights Reserved.

#include "MyCustomAssetTypeActions.h"
#include "MyCustomAssetEditor.h"
#include "CustomAsset/MyCustomAsset.h"

#include "AssetToolsModule.h"

#define LOCTEXT_NAMESPACE "MyCustomAssetTypeActions"

TArray<FText> FMyCustomAssetTypeActions::CachedSubMenus;

FMyCustomAssetTypeActions::FMyCustomAssetTypeActions(uint32 InAssetCategory)
	: AssetCategory(InAssetCategory)
{
}

FText FMyCustomAssetTypeActions::GetName() const
{
	return LOCTEXT("AssetTypeActions_MyCustomAsset", "Custom Asset");
}

FColor FMyCustomAssetTypeActions::GetTypeColor() const
{
	return FColor(128, 0, 128); // Purple
}

UClass* FMyCustomAssetTypeActions::GetSupportedClass() const
{
	return UMyCustomAsset::StaticClass();
}

uint32 FMyCustomAssetTypeActions::GetCategories()
{
	return AssetCategory;
}

void FMyCustomAssetTypeActions::OpenAssetEditor(const TArray<UObject*>& InObjects, TSharedPtr<IToolkitHost> EditWithinLevelEditor)
{
	// Create toolkit for each asset
	for (UObject* Object : InObjects)
	{
		if (UMyCustomAsset* CustomAsset = Cast<UMyCustomAsset>(Object))
		{
			TSharedRef<FMyCustomAssetEditorToolkit> EditorToolkit = MakeShareable(new FMyCustomAssetEditorToolkit());
			EditorToolkit->Initialize(CustomAsset, EditWithinLevelEditor);
		}
	}
}

const TArray<FText>& FMyCustomAssetTypeActions::GetSubMenus() const
{
	if (CachedSubMenus.Num() == 0)
	{
		CachedSubMenus.Add(LOCTEXT("AssetEditorExtensionSubMenu", "Editor Extension Demo"));
	}
	return CachedSubMenus;
}

bool FMyCustomAssetTypeActions::CanFilter()
{
	return true;
}

bool FMyCustomAssetTypeActions::ShouldForceWorldCentric()
{
	return false;
}

#undef LOCTEXT_NAMESPACE
