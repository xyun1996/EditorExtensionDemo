// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Toolkits/AssetEditorToolkit.h"
#include "Toolkits/IToolkitHost.h"
#include "Widgets/DeclarativeSyntaxSupport.h"

class UMyCustomAsset;
class IDetailsView;
class SMyAssetEditorWidget;
struct FPropertyChangedEvent;

/**
 * Custom asset editor toolkit for UMyCustomAsset
 */
class FMyCustomAssetEditorToolkit : public FAssetEditorToolkit
{
public:
	FMyCustomAssetEditorToolkit();
	virtual ~FMyCustomAssetEditorToolkit();

	// Initialize the editor
	void Initialize(UMyCustomAsset* InAsset, const TSharedPtr<IToolkitHost>& EditWithinLevelEditor);

	// FAssetEditorToolkit interface
	virtual FName GetToolkitFName() const override;
	virtual FText GetBaseToolkitName() const override;
	virtual FString GetWorldCentricTabPrefix() const override;
	virtual FLinearColor GetWorldCentricTabColorScale() const override;
	virtual void RegisterTabSpawners(const TSharedRef<class FTabManager>& TabManager) override;

	// IToolkit interface
	virtual FText GetToolkitName() const override;
	virtual FText GetToolkitToolTipText() const override;
	virtual FString GetDocumentationLink() const override;
	virtual void SaveAsset_Execute() override;

	// Called when the asset is changed
	void OnAssetChanged(const FPropertyChangedEvent& PropertyChangedEvent);

private:
	// Tab spawner for viewport tab
	TSharedRef<SDockTab> SpawnTab_AssetDetails(const FSpawnTabArgs& Args);
	TSharedRef<SDockTab> SpawnTab_PreviewViewport(const FSpawnTabArgs& Args);
	TSharedRef<SDockTab> SpawnTab_Settings(const FSpawnTabArgs& Args);

	// Bind toolbar commands
	void BindCommands();
	void ExtendToolbar();

	// Toolbar callbacks
	void OnResetToDefaults();
	bool CanResetToDefaults() const;

private:
	// The asset being edited
	TObjectPtr<UMyCustomAsset> EditedAsset;

	// Details view for the asset
	TSharedPtr<IDetailsView> DetailsView;

	// Custom widget for editing
	TSharedPtr<SMyAssetEditorWidget> EditorWidget;

	// Tab identifiers
	static const FName PropertiesTabId;
	static const FName ViewportTabId;
	static const FName SettingsTabId;

	// Toolkit name
	static const FName ToolkitName;
};
