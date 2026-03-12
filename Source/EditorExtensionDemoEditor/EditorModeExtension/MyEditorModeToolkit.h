// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Toolkits/BaseToolkit.h"

class FMyEditorMode;

/**
 * Toolkit/UI for the custom editor mode
 */
class FMyEditorModeToolkit : public FModeToolkit
{
public:
	FMyEditorModeToolkit();
	virtual ~FMyEditorModeToolkit();

	// FModeToolkit interface
	virtual void Init(const TSharedPtr<IToolkitHost>& InitToolkitHost) override;
	virtual FName GetToolkitFName() const override;
	virtual FText GetBaseToolkitName() const override;
	virtual FString GetWorldCentricTabPrefix() const override;
	virtual FLinearColor GetWorldCentricTabColorScale() const override;
	virtual TSharedPtr<SWidget> GetInlineContent() const override;

private:
	// Create the mode UI widget
	TSharedPtr<SWidget> CreateModePanel();

	// Spawn the main tab
	TSharedRef<SDockTab> SpawnTab_ModePanel(const FSpawnTabArgs& Args);

	// Command handlers
	void OnToolButtonClick(int32 ToolIndex);
	bool IsToolButtonSelected(int32 ToolIndex) const;

	// Reset the mode
	FReply OnResetMode();

private:
	// The mode we belong to
	TWeakPtr<FMyEditorMode> EditorMode;

	// Tab ID
	static const FName ModePanelTabId;

	// Available tools
	TArray<FName> AvailableTools;
};
