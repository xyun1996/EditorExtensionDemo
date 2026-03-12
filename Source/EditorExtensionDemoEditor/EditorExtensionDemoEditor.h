// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"
#include "IAssetTypeActions.h"
#include "AssetTypeCategories.h"

class FMyCustomAssetEditorToolkit;
class FMyCustomActorDetails;
class FMyEditorMode;
class FToolBarBuilder;
class FMenuBuilder;

/**
 * Editor module for EditorExtensionDemo
 */
class FEditorExtensionDemoEditorModule : public IModuleInterface
{
public:
	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

private:
	// Menu/Toolbar Extension
	void RegisterMenus();
	void RegisterToolbarExtensions();
	void SetupMenuEntries();

	// Commands Registration
	void RegisterCommands();
	void BindCommands();

	// Details Panel Extension
	void RegisterCustomClassLayout();
	void UnregisterCustomClassLayout();

	// Asset Editor Extension
	void RegisterAssetTypes();
	void UnregisterAssetTypes();

	// Editor Mode Extension
	void RegisterEditorMode();
	void UnregisterEditorMode();

	// Command callbacks
	void OnMenuCommandClicked();
	void OnToolbarButtonClicked();

	// Command list
	TSharedPtr<FUICommandList> PluginCommands;

	// Created asset type actions
	TArray<TSharedPtr<IAssetTypeActions>> CreatedAssetTypeActions;

	// Registered advanced asset category bit for this module.
	EAssetTypeCategories::Type AssetCategoryBit = EAssetTypeCategories::Misc;
};
